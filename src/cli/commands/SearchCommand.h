#pragma once

// libraries
#include <QNetworkReply>
#include <Attica/ProviderManager>

// local
#include <Command.h>

class SearchCommand : public Command {
Q_OBJECT
public:
    explicit SearchCommand(const QString& query);

    void setOcsProvidersList(QStringList list);

public slots:

    void execute() override;

protected slots:

    void handleAtticaProviderAdded(const Attica::Provider& provider);

    void handleAtticaFailedToLoad(const QUrl& provider, QNetworkReply::NetworkError error);

    void handleListCategoriesJobFinished(Attica::BaseJob* job);

    void handleSearchContentsJobFinished(Attica::BaseJob* job);


protected:
    void loadCategories();

    void doSearch(QList<Attica::Category> categories);

    Attica::ProviderManager providerManagers;
    Attica::Provider provider;

    QString query;

    QStringList applicationsIds;
    QStringList ocsProvidersList;
};
