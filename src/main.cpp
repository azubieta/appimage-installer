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
    parser.setApplicationDescription(
        QCoreApplication::translate("cli-main",
                                    "Command details\n"
                                    "   search <query>              list applications available in the store\n"
                                    "   get <STORE ID>              get the application with the given store id\n"
                                    "   list                        list applications available on your system\n"
                                    "   update <APP ID>             update if possible the given application\n"
                                    "   remove <APP ID>             remove the application from your system"));

    parser.addHelpOption();
    parser.process(app);

    QStringList args = parser.positionalArguments();
    if (args.empty())
        parser.showHelp(0);

    Command* command = nullptr;
    if (args.first() == "search") {
        args.pop_front();
        if (args.empty())
            qFatal("Missing search query. Example:\n\tapp search firefox");

        command = new SearchCommand(args.first());
    }

    if (args.first() == "get") {
        args.pop_front();
        if (args.empty())
            qFatal("Missing store id. Example:\n\tapp get 1230");

        command = new GetCommand(args.first());
    }

    if (args.first() == "list")
        command = new ListCommand();

    if (args.first() == "remove") {
        args.pop_front();
        if (args.empty())
            qFatal("Missing application id. Example:\n\tapp remove firefox");

        command = new RemoveCommand(args.first());
    }

    if (args.first() == "update")
        qFatal("ERROR: Updates aren't supported yet. Use AppImageUpate in the meanwhile.");

    if (command) {
        QObject::connect(command, &Command::executionCompleted, &app, &QCoreApplication::quit, Qt::QueuedConnection);
        QObject::connect(command, &Command::executionFailed, &app, &QCoreApplication::quit, Qt::QueuedConnection);
        QObject::connect(command, &Command::executionFailed, [](const QString& message) {
            qWarning() << message;
        });

        QMetaObject::invokeMethod(&app, &QCoreApplication::startingUp);
        command->execute();

        return QCoreApplication::exec();
    } else
        parser.showHelp(0);
}
