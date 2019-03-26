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
#include "GetCommand.h"

GetCommand::GetCommand(const QString& appId, QObject* parent) : Command(parent), appId(appId), out(stdout) {

    connect(&providerManager, &Attica::ProviderManager::providerAdded, this,
            &GetCommand::handleAtticaProviderAdded);

    connect(&providerManager, &Attica::ProviderManager::failedToLoad, this,
            &GetCommand::handleAtticaFailedToLoad);
}

void GetCommand::execute() {
    providerManager.addProviderFile(QUrl("https://appimagehub.com/ocs/providers.xml"));
}

void GetCommand::createApplicationsDir() {
    QDir home = QDir::home();
    home.mkdir("Applications");
}

QString GetCommand::buildTargetPath(const QString& contentId) {
    return QDir::homePath() + "/Applications/" + contentId + ".AppImage";
}

void GetCommand::handleDownloadProgress(qint64 progress, qint64 total, const QString& message) {
    // Draw nice [==   ] progress bar
    out << message;
    // clear spaces in the right
    struct winsize size = {0x0};
    ioctl(1, TIOCGWINSZ, &size);
    for (int i = 0; i < size.ws_col - message.size(); i++)
        out << " ";

    out << "\r";
    out.flush();
}

void GetCommand::handleDownloadCompleted() {
    QFile targetFile(targetPath);
    auto permissions = targetFile.permissions();

    qInfo() << QCoreApplication::translate("cli-main", "Installing");

    // make it executable
    targetFile.setPermissions(permissions | QFileDevice::ReadOwner | QFileDevice::ExeOwner);

    // integrate with the desktop environment
    appimage_register_in_system(targetPath.toStdString().c_str(), false);

    qInfo() << QCoreApplication::translate("cli-main", "Installation completed");
    fileDownload->deleteLater();

    emit Command::executionCompleted();
}

void GetCommand::handleDownloadFailed(const QString& message) {
    // Clean up
    QFile::remove(targetPath);
    fileDownload->deleteLater();

    emit executionFailed(message);
}

void GetCommand::getDownloadLink() {
    auto getDownloadLinkJob = provider.downloadLink(appId);
    connect(getDownloadLinkJob, &Attica::BaseJob::finished, this, &GetCommand::handleGetDownloadLinkJobFinished);

    getDownloadLinkJob->start();
}

void GetCommand::handleGetDownloadLinkJobFinished(Attica::BaseJob* job) {
    auto* downloadLinkJob = dynamic_cast<Attica::ItemJob<Attica::DownloadItem>*>(job);
    if (downloadLinkJob) {
        Attica::DownloadItem contents = downloadLinkJob->result();
        downloadLinkJob->deleteLater();

        if (contents.url().isEmpty()) {
            emit executionFailed("Unable to find a download for " + appId);
        } else {
            createApplicationsDir();

            targetPath = buildTargetPath(appId);
            fileDownload = new FileDownload(contents.url().toString(), targetPath, this);
            fileDownload->setProgressNotificationsEnabled(true);

            connect(fileDownload, &Download::progress, this, &GetCommand::handleDownloadProgress, Qt::QueuedConnection);
            connect(fileDownload, &Download::completed, this, &GetCommand::handleDownloadCompleted);
            connect(fileDownload, &Download::stopped, this, &GetCommand::handleDownloadFailed);

            out << "Getting " << appId << " from " << contents.url().toString() << "\n";
            fileDownload->start();
        }
    } else
        emit executionFailed("Unable to resolve the application download link.");
}

void GetCommand::handleAtticaProviderAdded(const Attica::Provider& provider) {
    GetCommand::provider = provider;

    getDownloadLink();
}
