extern "C" {
#include <sys/ioctl.h>
}

#include <QDebug>

#include "Console.h"

Console::Console() : in(stdin), out(stdout) {}

void Console::updateLine(const QString& text) {
    clearLine();

    out << text;
    out.flush();
}

void Console::clearLine() {
    struct winsize size = {0x0};
    ioctl(1, TIOCGWINSZ, &size);

    out << "\r";
    for (int i = 0; i < size.ws_col; i++)
        out << " ";

    out << "\r";
    out.flush();
}

QString Console::prettifyMemory(uint64_t bytes) {
    QStringList scales = {"B", "KiB", "MiB", "GiB", "TiB"};
    double value = bytes;
    int scale = 0;
    while (value > 1024 && scale + 1 < scales.size()) {
        value = value / 1024;
        scale++;
    }

    QString str;
    return str.sprintf("%.2f%s", value, scales.at(scale).toLatin1().data());
}

void Console::writeLine(const QString& text) {
    out << '\n' << text;
    out.flush();
}

int Console::doMultipleChoicesQuestion(const QString& question, const QStringList& choices) {
    int userPick = -1;
    QString choicesString = choices.join(", ");
    while (userPick < 0 || userPick >= choices.size()) {
        writeLine(question + " (" + choicesString + "): ");

        QString choice;
        in >> choice;

        userPick = choices.indexOf(choice);
        if (userPick < 0 || userPick >= choices.size())
            writeLine("Invalid choice. Please input one of: " + choicesString);
    }

    return userPick;
}

int Console::doEnumeratedChoicesQuestion(const QString& question, const QStringList& options) {
    // Don't ask anything is there is only one option
    if (options.size() == 1)
        return 0;

    int userPick = -1;

    QStringList choices;
    for (int i = 1; i <= options.size(); i++)
        choices << QString::number(i);

    while (userPick < 0 || userPick >= options.size()) {
        writeLine(question);

        for (int i = 0; i < options.size(); i++)
            writeLine(choices.at(i) + " - " + options.at(i));

        writeLine("Enter the option number to select it: ");
        QString choice;
        in >> choice;

        userPick = choices.indexOf(choice);
        if (userPick < 0 || userPick >= options.size())
            writeLine("Invalid choice \"" + choice + "\" . Please input one of: " + choices.join(", "));
    }

    return userPick;
}
