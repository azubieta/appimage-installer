// libraries
#include <QDebug>
#include <QTimer>
#include <QCommandLineParser>

// local
#include "commands/SearchCommand.h"
#include "commands/InstallCommand.h"
#include "commands/ListCommand.h"
#include "commands/RemoveCommand.h"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addPositionalArgument("command", QCoreApplication::translate("cli-main",
                                                                        "Command to be executed: search | install | list | update | remove"));
    parser.setApplicationDescription(
        QCoreApplication::translate("cli-main",
                                    "Command details\n"
                                    "   search <query>              list applications available in the store\n"
                                    "   install <STORE ID>              get the application with the given store id\n"
                                    "   list                        list applications available on your system\n"
                                    "   update <APP ID>             update if possible the given application\n"
                                    "   remove <APP ID>             remove the application from your system"));

    parser.addHelpOption();
    parser.process(app);

    QStringList args = parser.positionalArguments();
    if (args.empty())
        parser.showHelp(0);

    QTextStream out(stdout);

    Command* command = nullptr;
    if (!args.isEmpty() && args.first() == "search") {
        args.pop_front();
        if (args.empty())
            out << "Missing search query. Example:\n"
                   "\tapp search firefox\n\n";
        else
            command = new SearchCommand(args.first());
    }

    if (!args.isEmpty() && args.first() == "install") {
        args.pop_front();
        if (args.empty())
            out << "Missing store id. Example:\n"
                   "\tapp get 1230\n\n";
        else
            command = new InstallCommand(args.first());
    }

    if (!args.isEmpty() && args.first() == "list")
        command = new ListCommand();

    if (!args.isEmpty() && args.first() == "remove") {
        args.pop_front();
        if (args.empty())
            out << "Missing application id. Example:\n"
                   "\tapp remove firefox\n\n";
        else
            command = new RemoveCommand(args.first());
    }

    if (!args.isEmpty() && args.first() == "update")
        out << "ERROR: Updates aren't supported yet. Use AppImageUpate in the meanwhile.\n";
    if (command) {
        QObject::connect(command, &Command::executionCompleted, &app, &QCoreApplication::quit, Qt::QueuedConnection);
        QObject::connect(command, &Command::executionFailed, &app, &QCoreApplication::quit, Qt::QueuedConnection);
        QObject::connect(command, &Command::executionFailed, [&out](const QString& message) {
            out << message << '\n';
        });

        QTimer::singleShot(0, command, &Command::execute);

        return QCoreApplication::exec();
    } else {
        out.flush();
        parser.showHelp(0);
    }
}
