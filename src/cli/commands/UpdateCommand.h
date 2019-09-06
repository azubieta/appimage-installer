#pragma once

#include <QString>

#include "Command.h"

namespace AppImageUpdaterBridge {
    class AppImageDeltaRevisioner;
}

class UpdateCommand : public Command {
Q_OBJECT
public:
    explicit UpdateCommand(QString target, QObject* parent = nullptr);

    void execute() override;

private:
    QString target;
    AppImageUpdaterBridge::AppImageDeltaRevisioner* helper;

    QStringList listAppImages();
};



