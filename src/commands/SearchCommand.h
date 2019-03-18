#pragma once

// local
#include <Command.h>
#include <entities/Explorer.h>

class SearchCommand : public Command {
Q_OBJECT
public:
    explicit SearchCommand(const QString& query);

public slots:
    void execute() override;

protected slots:
    void handleSearchCompleted(QList<QVariantMap> apps);

protected:
    Explorer explorer;
    QString query;

    QStringList applicationsIds;
};
