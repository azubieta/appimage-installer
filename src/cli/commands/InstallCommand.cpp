// system
#include <fstream>
extern "C" {
#include <sys/ioctl.h>
}

// libraries
#include <QDir>
#include <QCoreApplication>
#include <QProcessEnvironment>
#include <Attica/ItemJob>
#include <Attica/GetJob>
#include <Attica/Content>
#include <Attica/DownloadItem>
#include <appimage/appimage.h>
#include <XdgUtils/DesktopEntry/DesktopEntry.h>
#include <XdgUtils/DesktopEntry/DesktopEntryStringsValue.h>
#include <yaml-cpp/yaml.h>
#include <QTextStream>

// local
#include <FileDownload.h>
#include "InstallCommand.h"

InstallCommand::InstallCommand(const QString& appId, QObject* parent) : Command(parent), appId(appId), out(stdout),
                                                                        fileDownload(nullptr) {

    connect(&providerManager, &Attica::ProviderManager::providerAdded, this,
            &InstallCommand::handleAtticaProviderAdded);

    connect(&providerManager, &Attica::ProviderManager::failedToLoad, this,
            &InstallCommand::handleAtticaFailedToLoad);
}

void InstallCommand::execute() {
    for (const QString& provider: ocsProvidersList) {
        providerManager.addProviderFile(QUrl(provider));
    }
}

void InstallCommand::createApplicationsDir() {
    QDir home = QDir::home();
    home.mkdir("Applications");
}

QString InstallCommand::buildTargetPath(const Attica::Content& content, const Attica::DownloadDescription& download) {
    const QDateTime contentUpdateDate = content.updated();
    QStringList postfixParts = {content.id(), content.name().replace(" ", "_"), content.version(),
                                contentUpdateDate.toString("yyyy-MM-dd-HH-mm")};
    QString fileNamePostfix = "__" + postfixParts.join("-") + ".AppImage";

    QString fileName = download.name();
    for (const QString& extension: {".AppImage", ".appimage", ".appImage"})
        fileName.replace(extension, fileNamePostfix);


    return QDir::homePath() + "/Applications/" + fileName;
}

void InstallCommand::handleDownloadProgress(qint64, qint64, const QString& message) {
    if (message.isEmpty())
        return;

    showInlineMessage(message);
}

void InstallCommand::showInlineMessage(const QString& message) {
    out << "\r" << message;

    // clear spaces in the right
    struct winsize size = {0x0};
    ioctl(1, TIOCGWINSZ, &size);
    for (int i = 0; i < size.ws_col - message.size(); i++)
        out << " ";

    out.flush();
}

void InstallCommand::handleDownloadCompleted() {
    showInlineMessage("Download completed");
    fileDownload->deleteLater();

    QFile::rename(targetPath + ".part", targetPath);

    installAppImage();
}

void InstallCommand::installAppImage() {
    showInlineMessage("Installing");

    QFile targetFile(targetPath);

    // make it executable
    auto permissions = targetFile.permissions();
    targetFile.setPermissions(permissions | QFileDevice::ReadOwner | QFileDevice::ExeOwner);

    // integrate with the desktop environment
    int res = appimage_register_in_system(targetPath.toStdString().c_str(), false);

    // add complementary actions
    char* desktopFilePath = appimage_registered_desktop_file_path(targetPath.toLocal8Bit(), nullptr, false);

    if (desktopFilePath != nullptr) {
        using namespace XdgUtils::DesktopEntry;

        std::ifstream ifstream(desktopFilePath);
        DesktopEntry entry(ifstream);

        std::string actionsString = static_cast<std::string>(entry["Desktop Entry/Actions"]);
        DesktopEntryStringsValue actions(actionsString);

        // Add uninstall action
        actions.append("Remove");
        entry.set("Desktop Entry/Actions", actions.dump());

        entry.set("Desktop Action Remove/Name", "Remove");
        entry.set("Desktop Action Remove/Icon", "application-x-trash");

        // Build Remove Action Exec Value
        auto systemEnvironment = QProcessEnvironment::systemEnvironment();

        // Use the APPIMAGE path if the app is used as an APPIMAGE
        QString rmCommand = systemEnvironment.value("APPIMAGE");

        // Otherwise use the application file path
        if (rmCommand.isEmpty())
            rmCommand = QCoreApplication::applicationFilePath();

        rmCommand += " remove " + targetPath;

        entry.set("Desktop Action Remove/Exec", rmCommand.toStdString());

        std::ofstream ofstream(desktopFilePath);
        ofstream << entry;
    }


    if (res == 0) {
        showInlineMessage("Installation completed");
    } else {
        showInlineMessage("Installation failed");
    }
    out << "\n";


    emit executionCompleted();
}

void InstallCommand::handleDownloadFailed(const QString& message) {
    showInlineMessage("Download failed: " + message);

    // Clean up
    QFile::remove(targetPath + ".part");
    fileDownload->deleteLater();

    emit executionFailed(message);
}

void InstallCommand::getDownloadLink() {
    auto getDownloadLinkJob = provider.requestContent(appId);
    connect(getDownloadLinkJob, &Attica::BaseJob::finished, this, &InstallCommand::handleGetDownloadLinkJobFinished);

    getDownloadLinkJob->start();
}

void InstallCommand::handleGetDownloadLinkJobFinished(Attica::BaseJob* job) {
    auto* downloadLinkJob = dynamic_cast<Attica::ItemJob<Attica::Content>*>(job);
    if (downloadLinkJob) {
        Attica::Content content = downloadLinkJob->result();
        downloadLinkJob->deleteLater();

        // Find the right download link
        QString architecture = QSysInfo::buildCpuArchitecture().replace("_", "-");
        qDebug() << "Looking for " << architecture << " AppImages";
        static QString archTag = "application##architecture=" + architecture;
        static QString pkgTypeTag = "application##packagetype=appimage";

        QList<Attica::DownloadDescription> compatibleDownloads;
        auto list = content.downloadUrlDescriptions();

        for (const auto& downloadDescription : list) {
            if (downloadDescription.tags().contains(pkgTypeTag) &&
                (!downloadDescription.tags().contains("application##architecture=") ||
                 downloadDescription.tags().contains(archTag))) {

                compatibleDownloads << downloadDescription;
            }
        }

        if (compatibleDownloads.isEmpty()) {
            emit executionFailed("Unable to find a download for " + appId);
        } else {

            // Default to the first compatible download
            int downloadIdx = 0;
            if (compatibleDownloads.size() > 1)
                downloadIdx = askWhichFileDownload(compatibleDownloads);

            // -1 means that the user want's to exit
            if (downloadIdx == -1) {
                emit Command::executionCompleted();
                return;
            }

            const Attica::DownloadDescription& download = compatibleDownloads.at(downloadIdx);
            targetPath = buildTargetPath(content, download);
            startFileDownload(download.link());
        }
    } else
        emit executionFailed("Unable to resolve the application download link.");
}

int InstallCommand::askWhichFileDownload(const QList<Attica::DownloadDescription>& compatibleDownloads) {
    int userPick = 0;
    out << "Available AppImage files:\n";
    for (int i = 1; i <= compatibleDownloads.size(); i++) {
        const auto& downloadDescription = compatibleDownloads.at(i - 1);
        out << i << "- " << downloadDescription.name() << "\n";
    }

    QTextStream in(stdin);
    while (userPick <= 0 || userPick > compatibleDownloads.size()) {
        out << "Enter the file number to download or 'x' to exit: ";
        out.flush();

        QString line = in.readLine();
        if (line.startsWith("x"))
            return -1;

        bool isInt = false;
        userPick = line.toInt(&isInt);
        if (!isInt || userPick <= 0 || userPick > compatibleDownloads.size())
            out << "Invalid option\n";
    }
    // Download options are presented starting at 1 but the array actually starts at 0
    return userPick - 1;
}

void InstallCommand::startFileDownload(const QUrl& downloadLink) {
    createApplicationsDir();

    fileDownload = new FileDownload(downloadLink, targetPath + ".part", this);
    fileDownload->setProgressNotificationsEnabled(true);

    connect(fileDownload, &Download::progress, this, &InstallCommand::handleDownloadProgress,
            Qt::QueuedConnection);
    connect(fileDownload, &Download::completed, this, &InstallCommand::handleDownloadCompleted,
            Qt::QueuedConnection);
    connect(fileDownload, &Download::stopped, this, &InstallCommand::handleDownloadFailed,
            Qt::QueuedConnection);

    out << "Downloading " << appId << " from " << downloadLink.toString() << "\n";
    fileDownload->start();
}

void InstallCommand::handleAtticaProviderAdded(const Attica::Provider& provider) {
    InstallCommand::provider = provider;

    getDownloadLink();
}

void InstallCommand::handleAtticaFailedToLoad(const QUrl& provider, QNetworkReply::NetworkError error) {
    emit Command::executionFailed("Unable to connect to " + provider.toString());
}

void InstallCommand::setOcsProvidersList(QStringList list) {
    ocsProvidersList = list;
}
