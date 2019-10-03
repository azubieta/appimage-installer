// libraries
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <appimage/appimage.h>

// local
#include "ListCommand.h"
#include "AppsLibrary.h"

void ListCommand::execute() {
    auto appImagePaths = AppsLibrary::list();

    QList<QPair<QString, QString>> applications;
    for (const auto& appImagePath: appImagePaths) {
        QString appImageId = AppsLibrary::getApplicationId(appImagePath);

        applications << QPair<QString, QString>(appImageId, appImagePath);
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
