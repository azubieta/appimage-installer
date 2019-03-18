#pragma once

// libraries
#include <QObject>

class Command : public QObject {
Q_OBJECT
public:
    Command(QObject *parent = nullptr): QObject(parent) {}
    virtual ~Command() {}

public Q_SLOTS:
    virtual void execute() = 0;

Q_SIGNALS:
    void executionCompleted();
    void executionFailed(const QString& message);
};
