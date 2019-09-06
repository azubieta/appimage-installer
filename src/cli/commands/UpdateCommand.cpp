extern "C" {
#include <sys/ioctl.h>
}

#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <appimage/appimage.h>
#include <AppImageUpdaterBridge>


#include "UpdateCommand.h"

UpdateCommand::UpdateCommand(QString target, QObject* parent) : Command(parent), target(std::move(target)) {}

void UpdateCommand::execute() {
    using namespace AppImageUpdaterBridge;
    helper = new AppImageDeltaRevisioner();
    if (QFile::exists(target))
        helper->setAppImage(target);
    else {
        auto appImages = listAppImages();

        for (const auto& appImage: appImages) {
            char** fileList = appimage_list_files(appImage.toStdString().c_str());
            for (char** itr = fileList; *itr != nullptr; itr++) {
                QString path = *itr;
                if (path.remove(".desktop") == target)
                   helper->setAppImage(appImage);
            }

            appimage_string_list_free(fileList);
        }
    }


    QObject::connect(helper, &AppImageDeltaRevisioner::progress,
                     [&](int percent, qint64 br, qint64 bt, double speed, QString unit) {
                         QTextStream out(stdout);
                         out << "Downloaded " << br << unit << " of " << bt << unit << " at " << speed << unit << "\n";
                     });

    QObject::connect(helper, &AppImageDeltaRevisioner::error, [&](short ecode) {
        emit Command::executionFailed(AppImageUpdaterBridge::errorCodeToString(ecode));
    });

    QObject::connect(helper, &AppImageDeltaRevisioner::started, [&]() {
        QTextStream out(stdout);
        out << "Looking for updates\n";
    });

    QObject::connect(helper, &AppImageDeltaRevisioner::finished,
                     [&](const QJsonObject& newVersion, const QString& oldAppImagePath) {
                         QTextStream out(stdout);

                         QJsonDocument doc(newVersion);
                         out << "New Version::  " << doc.toJson() << "\n";
                         out << "Old Version AppImage Path:: " << oldAppImagePath << "\n";
                         out << "\nCompleted Delta Update!" << "\n";

                         emit Command::executionCompleted();
                     });

    helper->start();
}

QStringList UpdateCommand::listAppImages() {
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
