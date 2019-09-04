#include <QDebug>
#include <QRegExp>
#include <Attica/ListJob>
#include <Attica/Content>

// local
#include "SearchCommand.h"

SearchCommand::SearchCommand(const QString& query) : query(query) {
    connect(&providerManagers, &Attica::ProviderManager::providerAdded, this,
            &SearchCommand::handleAtticaProviderAdded);

    connect(&providerManagers, &Attica::ProviderManager::failedToLoad, this,
            &SearchCommand::handleAtticaFailedToLoad);
}

void SearchCommand::execute() {
    for (const QString& providerStr: ocsProvidersList) {
        providerManagers.addProviderFile(QUrl(providerStr));
    }
}

void SearchCommand::handleAtticaProviderAdded(const Attica::Provider& provider) {
    SearchCommand::provider = provider;

    loadCategories();
}

void SearchCommand::loadCategories() {
    auto categoriesRequestJob = provider.requestCategories();
    connect(categoriesRequestJob, &Attica::BaseJob::finished, this, &SearchCommand::handleListCategoriesJobFinished);
    categoriesRequestJob->start();
}

void SearchCommand::handleListCategoriesJobFinished(Attica::BaseJob* job) {
    auto* listJob = dynamic_cast<Attica::ListJob<Attica::Category>*>(job);
    categories = listJob->itemList();

    doSearch();
    listJob->deleteLater();
}

void SearchCommand::doSearch() {
    auto* searchContentsJob = provider.searchContents(categories, query, Attica::Provider::Alphabetical, offset, pageSize);
    connect(searchContentsJob, &Attica::BaseJob::finished, this, &SearchCommand::handleSearchContentsJobFinished);
    searchContentsJob->start();
}

void SearchCommand::handleSearchContentsJobFinished(Attica::BaseJob* job) {
    auto* searchJob = dynamic_cast<Attica::ListJob<Attica::Content>*>(job);
    auto contents = searchJob->itemList();

    searchJob->deleteLater();

    QTextStream out(stdout);
    QRegExp reg(query.toLower());
    for (const auto& content: contents) {
        // Ignore contents that don't match name
        if (reg.indexIn(content.name().toLower()) == -1)
            continue;

        out << content.id();

        if (!content.name().isEmpty())
            out << " " << content.name().trimmed();

        if (!content.version().isEmpty())
            out << " " << content.version().trimmed();

        if (!content.author().isEmpty())
            out << " by " << content.author().trimmed() << "\n";
    }

    if (contents.empty()) {
        emit Command::executionCompleted();
    } else {
        searchJob->deleteLater();

        // load next page
        offset ++;
        doSearch();
    }
}

void SearchCommand::handleAtticaFailedToLoad(const QUrl& provider, QNetworkReply::NetworkError error) {
    emit Command::executionFailed("Unable to connect to " + provider.toString());
}

void SearchCommand::setOcsProvidersList(QStringList list) {
    ocsProvidersList = list;
}

