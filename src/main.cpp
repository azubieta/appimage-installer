// libraries
#include <QDebug>
#include <QCommandLineParser>

// local
#include "commands/SearchCommand.h"
#include "commands/GetCommand.h"
#include "commands/ListCommand.h"
#include "commands/RemoveCommand.h"

int main(int argc, char** argv) {

    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addPositionalArgument("command", QCoreApplication::translate("cli-main",
                                                                        "Command to be executed: search | get | list | update | remove"));
    parser.process(app);

    QStringList args = parser.positionalArguments();
    if (args.size() == 0)
        parser.showHelp(0);

    Command* command = nullptr;
    if (args.first() == "search") {
        args.pop_front();
        if (args.empty())
            qFatal("Missing search query, try:\n\tapp search franz");

        command = new SearchCommand(args.first());
    }

    if (args.first() == "get") {
        args.pop_front();
        if (args.empty())
            qFatal("Missing application id, try:\n\tapp get franz");

        command = new GetCommand(args.first());
    }

    if (args.first() == "list")
        command = new ListCommand();

    if (args.first() == "remove") {
        args.pop_front();
        if (args.empty())
            qFatal("Missing application id, try:\n\tapp remove franz");

        command = new RemoveCommand(args.first());
    }

    if (command) {
        QObject::connect(command, &Command::executionCompleted, &app, &QCoreApplication::quit, Qt::QueuedConnection);
        QObject::connect(command, &Command::executionFailed, &app, &QCoreApplication::quit, Qt::QueuedConnection);
        QObject::connect(command, &Command::executionFailed, [](const QString& message) {
            qWarning() << message;
        });

        QMetaObject::invokeMethod(&app, &QCoreApplication::startingUp);
        command->execute();

        return app.exec();
    } else
        parser.showHelp(0);
}
