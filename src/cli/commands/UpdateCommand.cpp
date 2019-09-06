extern "C" {
#include <sys/ioctl.h>
}

#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <appimage/appimage.h>
#include <AppImageUpdaterBridge>


#include "AppsLibrary.h"
#include "UpdateCommand.h"

UpdateCommand::UpdateCommand(QString target, QObject* parent) : Command(parent), target(std::move(target)),
                                                                helper(nullptr) {}

void UpdateCommand::execute() {
    using namespace AppImageUpdaterBridge;
    helper = new AppImageDeltaRevisioner();
    if (QFile::exists(target))
        helper->setAppImage(target);
    else {
        auto appImages = AppsLibrary::find(target);
        if (!appImages.isEmpty())
            helper->setAppImage(appImages.first());
        else {
            emit Command::executionFailed("Application not found: " + target);
            return;
        }
    }


    QObject::connect(helper, &AppImageDeltaRevisioner::progress,
                     [&](int percent, qint64 br, qint64 bt, double speed, const QString& unit) {
                         QTextStream out(stdout);
                         out << "Downloaded " << br / (1024) << "KiB of " << bt / (1024) << "KiB" <<
                             " at " << speed << unit << "\n";
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

    helper->setShowLog(true);
    helper->start();
}

