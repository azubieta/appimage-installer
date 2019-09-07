#pragma once

#include <QStringList>

/**
 * Applications management class
 */
class AppsLibrary {
public:

    /**
     * Looks for applications at $HOME/Applications
     * @return applications paths
     */
    static QStringList list();

    /**
     * The main .desktop file name is used as application id.
     *
     * @param appImagePath
     * @return application id
     */
    static QString getApplicationId(const QString& appImagePath);

    /**
     * Resolves the AppImage paths of applications with id <appId>
     *
     * @param appId
     * @return AppImage file path
     */
    static QStringList find(const QString &appId);

    /**
     * Remove an application from the system
     * @param appImagePath
     */
    static void remove(const QString& appImagePath);
};



