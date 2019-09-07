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
    QString appImagePath = getTargetAppImagePath();

    if (appImagePath.isEmpty()) {
        emit executionFailed(tr("Application not found: ") + target);
    } else {
        using namespace AppImageUpdaterBridge;

        helper = new AppImageDeltaRevisioner(appImagePath, true, this);
        QObject::connect(helper, &AppImageDeltaRevisioner::progress, this, &UpdateCommand::onProgress);
        QObject::connect(helper, &AppImageDeltaRevisioner::started, this, &UpdateCommand::onUpdateStarted);
        QObject::connect(helper, &AppImageDeltaRevisioner::error, this, &UpdateCommand::onUpdateError);
        QObject::connect(helper, &AppImageDeltaRevisioner::updateAvailable, this, &UpdateCommand::onUpdateAvailable);
        QObject::connect(helper, &AppImageDeltaRevisioner::finished, this, &UpdateCommand::onDownloadFinished);

        helper->start();
    }
}

QString UpdateCommand::getTargetAppImagePath() {
    QString appImagePath;

    if (QFile::exists(target))
        appImagePath = target;
    else {
        auto appImages = AppsLibrary::find(target);

        if (!appImages.isEmpty()) {
            int choice = console.doEnumeratedChoicesQuestion(tr("Which file would you like to update?"), appImages);
            appImagePath = appImages.at(choice);
        }
    }

    return appImagePath;
}

void UpdateCommand::onUpdateStarted() {
    console.writeLine("Looking for updates");
}

void UpdateCommand::onUpdateError(short errorCode) {
    QString errorMessage;

    if (errorCode == AppImageUpdaterBridge::EmptyUpdateInformation)
        errorMessage = tr("This application desn't provide any update information");

    if (errorMessage.isEmpty())
        errorMessage = AppImageUpdaterBridge::errorCodeToString(errorCode);

    emit Command::executionFailed(errorMessage);
}

void UpdateCommand::onUpdateAvailable(bool isUpdateAvailable, const QJsonObject&) {
    if (isUpdateAvailable)
        console.updateLine("Update found, proceeding to download");
    else
        console.updateLine("No updates found");
}

void UpdateCommand::onProgress(int , qint64 br, qint64 bt, double speed, const QString& unit) {
    QString msg("Download %1 of %2 at %3%4");

    console.updateLine(msg.arg(Console::prettifyMemory(br)).arg(Console::prettifyMemory(bt))
                               .arg(speed).arg(unit));
}

void UpdateCommand::onDownloadFinished(const QJsonObject& newVersion, const QString& oldAppImagePath) {
    QString newFilePath = newVersion.value("AbsolutePath").toString();
    console.updateLine(tr("Update completed!"));
    console.writeLine();
    console.writeLine(
            tr("The new application is at: ") + newFilePath);
    console.writeLine();

    if (newFilePath != oldAppImagePath) {
        int choice = console.doMultipleChoicesQuestion(tr("Do you want to keep the old version?"),
                                                       {tr("Yes"), tr("No"), tr("y"), tr("n")});

        if (choice % 2 == 1) {
            AppsLibrary::remove(oldAppImagePath);
            console.writeLine(tr("Old version removed!"));
            console.writeLine();
        }
    }

    emit Command::executionCompleted();
}
