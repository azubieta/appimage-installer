// libraries
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <appimage/core/exceptions.h>
#include <appimage/desktop_integration/IntegrationManager.h>
#include <appimage/utils/ResourcesExtractor.h>

// local
#include "entities/Application.h"
#include "RemoveCommand.h"

RemoveCommand::RemoveCommand(QString& appId, QObject* parent) : Command(parent), appId(appId) {}

void RemoveCommand::execute() {
    auto appImages = listAppImages();

    QList<Application> applications;
    for (const auto& appImage: appImages) {
        try {
            appimage::utils::ResourcesExtractor extractor(appImage);
            QString id = QString::fromStdString(extractor.getDesktopEntryPath()).remove(".desktop");
            Application app(id, "latest");
            app.setDownloadUrl(QString::fromStdString(appImage.getPath()));

            applications << app;
        } catch (const appimage::core::AppImageError& error) {
            qWarning() << QString::fromStdString(error.what());
        }

    }


    static QTextStream out(stdout);
    appimage::desktop_integration::IntegrationManager integrationManager;

    bool removed = false;
    for (const auto& application: applications) {
        if (application.getCodeName() == appId) {
            QFile::remove(application.getDownloadUrl());
            integrationManager.unregisterAppImage(application.getDownloadUrl().toStdString());

            out << "Application removed: " << application.getCodeName() << "\n";
            out << "\t " << application.getDownloadUrl() << "\n";
            removed = true;
        }
    }

    if (removed)
        emit Command::executionCompleted();
    else
        emit Command::executionFailed("Application not found: " + appId);
}

QList<appimage::core::AppImage> RemoveCommand::listAppImages() {
    QList<appimage::core::AppImage> list;
    QDir dir(QDir::homePath() + "/Applications");

    auto candidates = dir.entryList({}, QDir::Files);
    for (const auto& candidate: candidates) {
        try {
            appimage::core::AppImage appImage(dir.filePath(candidate).toStdString());
            list << appImage;
        } catch (const appimage::core::AppImageError& error) {
            qWarning() << QString::fromStdString(error.what()) << " " << dir.filePath(candidate);
        }

    }

    return list;
}
