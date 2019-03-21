// libraries
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <appimage/appimage.h>

// local
#include "entities/Application.h"
#include "RemoveCommand.h"

RemoveCommand::RemoveCommand(QString& appId, QObject* parent) : Command(parent), appId(appId) {}

void RemoveCommand::execute() {
    auto appImages = listAppImages();

    bool removed = false;
    for (const auto& appImage: appImages) {
        char** fileList = appimage_list_files(appImage.toStdString().c_str());
        for (char** itr = fileList; *itr != nullptr; itr++) {
            QString path = *itr;
            if (path.remove(".desktop") == appId) {
                QFile::remove(appImage);
                appimage_unregister_in_system(appImage.toStdString().c_str(), false);
                removed = true;
            }
        }

        appimage_string_list_free(fileList);
    }

    if (removed)
        emit Command::executionCompleted();
    else
        emit Command::executionFailed("Application not found: " + appId);
}

QList<QString> RemoveCommand::listAppImages() {
    QList<QString> list;
    QDir dir(QDir::homePath() + "/Applications");

    auto candidates = dir.entryList({}, QDir::Files);
    for (const auto& candidate: candidates) {
        auto fullPath = dir.filePath(candidate).toLocal8Bit();
        int type = appimage_get_type(fullPath, false);
        if (type != -1)
            list << fullPath;
    }

    return list;
}
