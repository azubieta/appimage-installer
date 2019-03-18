// libraries
#include <QDebug>

// local
#include "SearchCommand.h"

SearchCommand::SearchCommand(const QString& query) : explorer("http://apps.nxos.org/api"), query(query) {
    connect(&explorer, &Explorer::searchCompleted, this, &SearchCommand::handleSearchCompleted, Qt::QueuedConnection);
}

void SearchCommand::execute() {
    explorer.search(query);
}

void SearchCommand::handleSearchCompleted(QList<QVariantMap> apps) {
    static QTextStream out(stdout);

    for (const auto app: apps) {
        out << "\n" << app.value("id").toString().remove(".desktop") << "\n";

        if (app.contains("abstract")) {
            auto abstractLocales = app.value("abstract").toMap();
            if (!abstractLocales.empty())
                out << "\t" << abstractLocales.first().toString() << "\n";
            else
                out << "\t" << "No abstract found." << "\n";
        }
    }

    emit Command::executionCompleted();
}

