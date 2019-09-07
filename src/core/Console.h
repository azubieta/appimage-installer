#pragma once

#include <QString>
#include <QTextStream>

/**
 * Pretty printing on the terminal
 */
class Console {
public:
    Console();

    /**
     * Prompts the <question> and loops until the user inputs one of the <choices>
     * @param question
     * @param choices
     * @return selected choice index
     */
    int doMultipleChoicesQuestion(const QString& question, const QStringList& choices);

    /**
     * Prompts the <question> and enumerate the options. The user must input the number of
     * the option to select it.
     *
     * @param question
     * @param options
     * @return
     */
    int doEnumeratedChoicesQuestion(const QString& question, const QStringList& options);

    /**
     * Writes text to a new line
     * @param text
     */
    void writeLine(const QString& text = "");

    /**
     * Replaces the current line by <text>
     * @param text
     */
    void updateLine(const QString& text);

    /**
     * Remov
     * @param pos
     */
    void clearLine();

    /**
     * Get a human friendly representation of a memory value.
     *
     * @param bytes
     * @return formatted memory value
     */
    static QString prettifyMemory(uint64_t bytes);

private:

    QTextStream in;
    QTextStream out;
};



