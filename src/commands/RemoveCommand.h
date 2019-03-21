#pragma once

// libraries
#include "Command.h"

class RemoveCommand : public Command {
Q_OBJECT
public:
    explicit RemoveCommand(QString& appId, QObject* parent = nullptr);

public slots:
    void execute() override;

private:
    QString appId;

    QList<QString> listAppImages();
};

