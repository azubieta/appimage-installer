// libraries
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <appimage/core/exceptions.h>
#include <appimage/utils/ResourcesExtractor.h>

// local
#include "entities/Application.h"
#include "ListCommand.h"

void ListCommand::execute() {
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

    if (applications.empty())
        out << "There is any known AppImage in your system.\n";
    else {
        out << "Downloaded AppImages:\n";
        for (const auto& application: applications) {
            out << '\n' << application.getCodeName() << '\n';
            out << '\t' << application.getDownloadUrl() << '\n';
        }
    }
    emit Command::executionCompleted();
}

QList<appimage::core::AppImage> ListCommand::listAppImages() {
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
