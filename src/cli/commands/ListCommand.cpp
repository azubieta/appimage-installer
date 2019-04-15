// libraries
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <appimage/appimage.h>

// local
#include "ListCommand.h"

void ListCommand::execute() {
    auto appImages = listAppImages();

    QList<QPair<QString, QString>> applications;
    for (const auto& appImage: appImages) {
        char** fileList = appimage_list_files(appImage.toStdString().c_str());
        for (char** itr = fileList; *itr != nullptr; itr++) {
            QString path = *itr;
            if (path.endsWith(".desktop") && !path.contains('/')) {
                applications << QPair<QString, QString>(path.remove(".desktop"), appImage);
            }
        }

        appimage_string_list_free(fileList);
    }


    static QTextStream out(stdout);

    if (applications.empty())
        out << "There is any known AppImage in your system.\n";
    else {
        int maxNameLenght = 0;
        for (const auto& application: applications)
            if (application.first.size() > maxNameLenght)
                maxNameLenght = application.first.size();

        for (const auto& application: applications) {
            out << application.first;
            for (int i = 0; i < (maxNameLenght - application.first.size()); i++)
                out << " ";

            out << "  " << application.second << '\n';
        }
    }
    emit Command::executionCompleted();
}

QList<QString> ListCommand::listAppImages() {
    QList<QString> list;
    QDir dir(QDir::homePath() + "/Applications");

    auto candidates = dir.entryList({}, QDir::Files);
    for (const auto& candidate: candidates) {
        auto fullPath = dir.filePath(candidate).toLocal8Bit();
        int type = appimage_get_type(fullPath, false);
        if (type != -1)
            list << fullPath;
    }

    return list;
}

