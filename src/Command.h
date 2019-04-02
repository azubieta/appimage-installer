#pragma once

// libraries
#include <QObject>

class Command : public QObject {
Q_OBJECT
public:
    explicit Command(QObject *parent = nullptr): QObject(parent) {}
    ~Command() override {}

public Q_SLOTS:
    virtual void execute() = 0;

Q_SIGNALS:
    void executionCompleted();
    void executionFailed(const QString& message);
};
