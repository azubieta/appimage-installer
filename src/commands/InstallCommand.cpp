// system
extern "C" {
#include <sys/ioctl.h>
}

// libraries
#include <QDir>
#include <QCoreApplication>
#include <Attica/ItemJob>
#include <Attica/GetJob>
#include <Attica/Content>
#include <Attica/DownloadItem>
#include <appimage/appimage.h>

// local
#include <gateways/FileDownload.h>
#include "InstallCommand.h"

InstallCommand::InstallCommand(const QString& appId, QObject* parent) : Command(parent), appId(appId), out(stdout),
                                                                        fileDownload(nullptr) {

    connect(&providerManager, &Attica::ProviderManager::providerAdded, this,
            &InstallCommand::handleAtticaProviderAdded);

    connect(&providerManager, &Attica::ProviderManager::failedToLoad, this,
            &InstallCommand::handleAtticaFailedToLoad);
}

void InstallCommand::execute() {
    providerManager.addProviderFile(QUrl("https://appimagehub.com/ocs/providers.xml"));
}

void InstallCommand::createApplicationsDir() {
    QDir home = QDir::home();
    home.mkdir("Applications");
}

QString InstallCommand::buildTargetPath(Attica::Content content) {
    QString fileName = content.id() + '-' + content.name().replace(" ", "_") + '-' + content.version() + '-' +
                       content.updated().toString("yyyyMMdd-HH:mm");
    return QDir::homePath() + "/Applications/" + fileName.toLower() + ".AppImage";
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

    showInlineMessage("Installing");
    QFile targetFile(targetPath);

    // make it executable
    auto permissions = targetFile.permissions();
    targetFile.setPermissions(permissions | QFileDevice::ReadOwner | QFileDevice::ExeOwner);

    // integrate with the desktop environment
    int res = appimage_register_in_system(targetPath.toStdString().c_str(), false);

    if (res == 0)
        showInlineMessage("Installation completed");
    else
        showInlineMessage("Installation failed");

    out << "\n";
    fileDownload->deleteLater();

    emit Command::executionCompleted();
}

void InstallCommand::handleDownloadFailed(const QString& message) {
    showInlineMessage("Download failed: " + message);

    // Clean up
    QFile::remove(targetPath);
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

        QUrl downloadLink;
        auto list = content.downloadUrlDescriptions();

        for (const auto& downloadDescription : list) {
            if (downloadDescription.tags().contains("application##packagetype=appimage") &&
                downloadDescription.tags().contains(archTag)) {

                downloadLink = downloadDescription.link();
                targetPath = buildTargetPath(content);

                break;
            }
        }

        if (downloadLink.isEmpty()) {
            emit executionFailed("Unable to find a download for " + appId);
        } else {
            createApplicationsDir();

            fileDownload = new FileDownload(downloadLink, targetPath, this);
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
    } else
        emit executionFailed("Unable to resolve the application download link.");
}

void InstallCommand::handleAtticaProviderAdded(const Attica::Provider& provider) {
    InstallCommand::provider = provider;

    getDownloadLink();
}
