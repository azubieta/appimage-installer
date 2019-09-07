// libraries
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <appimage/appimage.h>

// local
#include "AppsLibrary.h"
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
        auto appImagePaths = AppsLibrary::find(target);
        if (!appImagePaths.empty()) {
            auto targetPath = appImagePaths.first();
            appimage_unregister_in_system(targetPath.toLatin1().data(), false);
            QFile::remove(targetPath);

            removed = true;
        }
    }

    if (removed) {
        QTextStream out(stdout);
        out << "Application removed: " + target << "\n";

        emit Command::executionCompleted();
    } else {
        emit Command::executionFailed("Application not found: " + target);
    }
}

