#pragma once

#include <QString>

#include "Command.h"
#include "Console.h"

namespace AppImageUpdaterBridge {
    class AppImageDeltaRevisioner;
}

class UpdateCommand : public Command {
Q_OBJECT
public:
    explicit UpdateCommand(QString target, QObject* parent = nullptr);

    void execute() override;

protected slots:

    void onUpdateStarted();

    void onUpdateError(short errorCode);

    void onUpdateAvailable(bool isUpdateAvailable, const QJsonObject& updateInfo);

    void onProgress(int percent, qint64 br, qint64 bt, double speed, const QString& unit);

    void onDownloadFinished(const QJsonObject& newVersion, const QString& oldAppImagePath);

private:
    QString target;
    Console console;
    AppImageUpdaterBridge::AppImageDeltaRevisioner* helper;

    QString getTargetAppImagePath();
};



