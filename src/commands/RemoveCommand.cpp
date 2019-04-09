// libraries
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <appimage/appimage.h>

// local
#include "entities/Application.h"
#include "RemoveCommand.h"

RemoveCommand::RemoveCommand(QString& target, QObject* parent) : Command(parent), target(target) {}

void RemoveCommand::execute() {
    bool removed = false;

    if (QFile::exists(target)) {
        int appImageType = appimage_get_type(target.toLocal8Bit(), false);
        if (appImageType > 0) {
            removed = QFile::remove(target) &&
                      appimage_unregister_in_system(target.toLocal8Bit(), false) == 0;
        }
    } else {
        auto appImages = listAppImages();

        for (const auto& appImage: appImages) {
            char** fileList = appimage_list_files(appImage.toStdString().c_str());
            for (char** itr = fileList; *itr != nullptr; itr++) {
                QString path = *itr;
                if (path.remove(".desktop") == target) {
                    QFile::remove(appImage);
                    appimage_unregister_in_system(appImage.toStdString().c_str(), false);
                    removed = true;
                }
            }

            appimage_string_list_free(fileList);
        }
    }

    if (removed) {
        QTextStream out(stdout);
        out << "Application removed: " + target;

        emit Command::executionCompleted();
    } else
        emit Command::executionFailed("Application not found: " + target);
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
