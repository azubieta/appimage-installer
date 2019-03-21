#pragma once

// libraries
#include "Command.h"

class ListCommand : public Command {
Q_OBJECT
public:
    explicit ListCommand(QObject* parent = nullptr) : Command(parent) {}

public slots:
    void execute() override;

private:
    QList<QString> listAppImages();
};

