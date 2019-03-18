#pragma once

// libraries
#include "Command.h"
#include <appimage/core/AppImage.h>

class RemoveCommand : public Command {
Q_OBJECT
public:
    explicit RemoveCommand(QString& appId, QObject* parent = nullptr);

public slots:
    void execute() override;

private:
    QString appId;

    QList<appimage::core::AppImage> listAppImages();
};

