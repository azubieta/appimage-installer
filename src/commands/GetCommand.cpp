// system
extern "C" {
#include <sys/ioctl.h>
}

// libraries
#include <QDir>
#include <appimage/desktop_integration/IntegrationManager.h>

// local
#include <gateways/FileDownload.h>
#include "GetCommand.h"

GetCommand::GetCommand(const QString& appId, QObject* parent) : Command(parent), appId(appId),
                                                                explorer("http://apps.nxos.org/api"), out(stdout) {

    connect(&explorer, &Explorer::getApplicationCompleted, this, &GetCommand::handleGetApplicationCompleted,
            Qt::QueuedConnection);
}

void GetCommand::execute() {
    explorer.getApplication(appId + ".desktop");
}

void GetCommand::handleGetApplicationCompleted(QVariantMap app) {
    if (app.empty()) {
        emit Command::executionFailed("Application not found: " + appId);
        return;
    }

    auto releases = app.value("releases").toList();
    if (releases.empty()) {
        emit Command::executionFailed("No compatible releases found for: " + appId);
        return;
    }

    // find latest release
    QVariantMap latestRelease = releases.first().toMap();
    for (const auto& itr: releases) {
        auto map = itr.toMap();

        auto version = map.value("version", "latest").toString();
        if (latestRelease.value("version") < map.value("version"))
            latestRelease = map;
    }


    // find binary for the current arch
    QVariantMap file;
    for (const auto& itr: latestRelease.value("files").toList()) {
        auto map = itr.toMap();
        if (isCompatibleBinary(map.value("architecture").toString())) {
            file = map;
            break;
        }
    }

    if (file.empty()) {
        emit Command::executionFailed("No binaries releases found for: " + QSysInfo::currentCpuArchitecture());
        return;
    }

    Application a(latestRelease.value("id").toString(), latestRelease.value("version", "latest").toString());
    a.setArch(file.value("architecture").toString());
    a.setDownloadUrl(file.value("url").toString());

    createApplicationsDir();
    downloadFile(a);
}

bool GetCommand::isCompatibleBinary(QString arch) {
    auto normalArch = arch.replace('-', '_');
    return normalArch == QSysInfo::currentCpuArchitecture();
}

void GetCommand::downloadFile(const Application& application) {
    targetPath = buildTargetPath(application);
    fileDownload = new FileDownload(application.getDownloadUrl(), targetPath, this);
    fileDownload->setProgressNotificationsEnabled(true);

    connect(fileDownload, &Download::progress, this, &GetCommand::handleDownloadProgress, Qt::QueuedConnection);
    connect(fileDownload, &Download::completed, this, &GetCommand::handleDownloadCompleted);
    connect(fileDownload, &Download::stopped, this, &GetCommand::handleDownloadFailed);

    out << "Getting " << appId << " from " << application.getDownloadUrl() << "\n";
    fileDownload->start();
}

void GetCommand::createApplicationsDir() {
    QDir home = QDir::home();
    home.mkdir("Applications");
}

QString GetCommand::buildTargetPath(const Application& a) {
    return QDir::homePath() + "/Applications/" +
           a.getCodeName() + "-" + a.getVersion() + "-" + a.getArch() + ".AppImage";
}

void GetCommand::handleDownloadProgress(qint64 progress, qint64 total, const QString &message) {
    // Draw nice [==   ] progress bar
    out << message;
    // clear spaces in the right
    struct winsize size;
    ioctl(1, TIOCGWINSZ, &size);
    for (int i = 0; i < size.ws_col - message.size(); i++)
        out << " ";

    out << "\r";
    out.flush();
}

void GetCommand::handleDownloadCompleted() {
    QFile targetFile(targetPath);
    auto permissions = targetFile.permissions();

    // make it executable
    targetFile.setPermissions(permissions | QFileDevice::ReadOwner | QFileDevice::ExeOwner);

    // integrate with the desktop environment
    try {
        appimage::core::AppImage appImage(targetPath.toStdString());

        appimage::desktop_integration::IntegrationManager integrationManager;
        integrationManager.registerAppImage(appImage);
        integrationManager.generateThumbnails(appImage);
    } catch (const appimage::core::AppImageError& error) {
        qWarning() << "AppImage integration failed: " << QString::fromStdString(error.what());
    }
    fileDownload->deleteLater();

    emit Command::executionCompleted();
}

void GetCommand::handleDownloadFailed(const QString& message) {
    // Clean up
    QFile::remove(targetPath);
    fileDownload->deleteLater();

    emit executionFailed(message);
}
