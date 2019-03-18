#pragma once

// libraries
#include "Command.h"
#include <appimage/core/AppImage.h>

class ListCommand : public Command {
Q_OBJECT
public:
    ListCommand(QObject* parent = nullptr) : Command(parent) {}

public slots:
    void execute() override;

private:
    QList<appimage::core::AppImage> listAppImages();
};

