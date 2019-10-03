#include <QDir>
#include <appimage/appimage.h>

#include "AppsLibrary.h"

QStringList AppsLibrary::list() {
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

QString AppsLibrary::getApplicationId(const QString& appImagePath) {
    QString id;
    char** fileList = appimage_list_files(appImagePath.toStdString().c_str());
    for (char** itr = fileList; *itr != nullptr && id.isEmpty(); itr++) {
        QString path = *itr;
        if (path.endsWith(".desktop") && !path.contains("/"))
            id = path.remove(".desktop");
    }

    appimage_string_list_free(fileList);

    return id;
}

QStringList AppsLibrary::find(const QString& targetAppId) {
    QStringList appImagePaths;
    for (const auto& appImage: list()) {
        QString currentAppId = AppsLibrary::getApplicationId(appImage);

        if (currentAppId == targetAppId)
            appImagePaths << appImage;
    }

    return appImagePaths;
}

void AppsLibrary::remove(const QString& appImagePath) {
    appimage_unregister_in_system(appImagePath.toLatin1().data(), false);
    QFile::remove(appImagePath);
}
