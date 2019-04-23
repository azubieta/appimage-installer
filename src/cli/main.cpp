// libraries
#include <QTimer>
#include <QCommandLineParser>

// local
#include "commands/SearchCommand.h"
#include "commands/InstallCommand.h"
#include "commands/ListCommand.h"
#include "commands/RemoveCommand.h"
#include "settings/Settings.h"

Command* createCommand(QStringList& args, Settings& settings);

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addPositionalArgument("command", QCoreApplication::translate("cli-main",
                                                                        "Command to be executed: search | install | list | update | remove"));

    parser.addOption({"verbose", QCoreApplication::translate("cli-main", "Show debug messages")});
    parser.setApplicationDescription(
        QCoreApplication::translate("cli-main",
                                    "Command details\n"
                                    "   search <query>              list applications available in the store\n"
                                    "   install <STORE ID>          get the application with the given store id\n"
                                    "   list                        list applications available on your system\n"
                                    "   update <APP ID>             update if possible the given application\n"
                                    "   remove <APP ID>             remove the application from your system"));

    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    QStringList args = parser.positionalArguments();
    if (args.empty())
        parser.showHelp(0);

    QTextStream out(stdout);

    static const bool verboseMode = parser.isSet("verbose");
    // Logger
    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext& context, const QString& msg) {
        QByteArray localMsg = msg.toLocal8Bit();
        const char* file = context.file ? context.file : "";
        const char* function = context.function ? context.function : "";
        const char* msgPrefix;

        // Ignore debug unless in verbose mode
        if (!verboseMode && type < QtInfoMsg)
            return;

        switch (type) {
            case QtDebugMsg:
                msgPrefix = "Debug";
                break;
            case QtInfoMsg:
                msgPrefix = "Info";
                break;
            case QtWarningMsg:
                msgPrefix = "Warning";
                break;
            case QtCriticalMsg:
                msgPrefix = "Critical";
                break;
            case QtFatalMsg:
                msgPrefix = "Fatal";
                break;
        }

        fprintf(stderr, "%s: %s", msgPrefix, localMsg.constData());
        if (type != QtInfoMsg)
            fprintf(stderr, "(%s:%u, %s)", file, context.line, function);
        fprintf(stderr, "\n");
    });


    Settings settings;
    Command* abstractCommand = createCommand(args, settings);

    if (abstractCommand) {
        QObject::connect(abstractCommand, &Command::executionCompleted, &app, &QCoreApplication::quit,
                         Qt::QueuedConnection);
        QObject::connect(abstractCommand, &Command::executionFailed, &app, &QCoreApplication::quit,
                         Qt::QueuedConnection);
        QObject::connect(abstractCommand, &Command::executionFailed, [&out](const QString& message) {
            out << message << '\n';
        });

        QTimer::singleShot(0, abstractCommand, &Command::execute);

        return QCoreApplication::exec();
    } else {
        out.flush();
        parser.showHelp(0);
    }
}

Command* createCommand(QStringList& args, Settings& settings) {
    if (!args.isEmpty() && args.first() == "search") {
        args.pop_front();
        if (args.empty())
            qInfo() << "Missing search query. Example:\n"
                       "\tapp search firefox\n\n";
        else {
            QStringList ocsProvidersList = settings.getOCSProviders();
            if (!ocsProvidersList.empty()) {
                auto command = new SearchCommand(args.first());
                command->setOcsProvidersList(ocsProvidersList);

                return command;
            } else {
                return nullptr;
            }
        }
    }

    if (!args.isEmpty() && args.first() == "install") {
        args.pop_front();
        if (args.empty())
            qInfo() << "Missing store id. Example:\n"
                       "\tapp get 1230\n\n";
        else {
            QStringList ocsProvidersList = settings.getOCSProviders();
            if (!ocsProvidersList.empty()) {
                auto command = new InstallCommand(args.first());
                command->setOcsProvidersList(ocsProvidersList);
                return command;
            } else {
                return nullptr;
            }
        }
    }

    if (!args.isEmpty() && args.first() == "list")
        return new ListCommand();

    if (!args.isEmpty() && args.first() == "remove") {
        args.pop_front();
        if (args.empty())
            qInfo() << "Missing application id. Example:\n"
                       "\tapp remove firefox\n\n";
        else
            return new RemoveCommand(args.first());
    }

    if (!args.isEmpty() && args.first() == "update")
        qCritical() << "Updates aren't supported yet. Use AppImageUpate in the meanwhile.";

    return nullptr;
}
