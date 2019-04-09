#pragma once

// libraries
#include "Command.h"

class RemoveCommand : public Command {
Q_OBJECT
public:
    explicit RemoveCommand(QString& target, QObject* parent = nullptr);

public slots:
    void execute() override;

private:
    QString target;

    QList<QString> listAppImages();
};

