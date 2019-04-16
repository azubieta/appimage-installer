// system
#include <fstream>

// library
#include <QFile>
#include <QStandardPaths>
#include <QDebug>

// local
#include "Settings.h"

Settings::Settings() {
}

QStringList Settings::getOCSProviders() {
    YAML::Node node = loadProvidersConfigFile();
    if (node.IsDefined())
        return getOCSProviders(node);
}

void Settings::setOCSProviders(QStringList ocsProviders) {
    YAML::Node providersRoot = loadProvidersConfigFile();

    setOCSProviders(providersRoot, ocsProviders);
    saveProvidersConfigFile(providersRoot);
}

/**
 * @brief load the providers configuration file.
 *
 * Load the providers configuration file from the following places:
 *  - $PWD/appimage-providers.yaml
 *  - $HOME/.config/appimage-providers.yaml
 *  - /etc/appimage-providers.yaml
 *
 * The first match is keept
 *
 * @return root node of the configuration file
 */
YAML::Node Settings::loadProvidersConfigFile() {
    if (QFile::exists("appimage-providers.yaml"))
        return YAML::LoadFile("appimage-providers.yaml");

    QStringList configLocations = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    for (const QString& location: configLocations) {
        QString filePath = location + "/appimage-providers.yaml";
        qDebug() << "trying providers configuration file: " + filePath;

        if (QFile::exists(filePath)) {
            qDebug() << "using providers configuration file: " + filePath;
            return YAML::LoadFile(filePath.toStdString());
        }
    }

    qWarning() << "No providers configuration file found";
}

void Settings::saveProvidersConfigFile(YAML::Node root) {
    QStringList configLocations = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    if (configLocations.size() > 0) {
        QString filePath = configLocations.first() + "/appimage-providers.yaml";
        std::ofstream fout(filePath.toStdString());

        fout << root;
    } else {
        qWarning() << "Providers not saved";
    }

}

/**
 * @brief read ocs providers from the configuration file
 * @param configuration file root node
 * @return list of ocs providers urls
 */
QStringList Settings::getOCSProviders(YAML::Node root) {
    QStringList foundOcsProviders;

    YAML::Node ocsProvidersNode = root["ocs"];
    if (ocsProvidersNode && ocsProvidersNode.Type() == YAML::NodeType::Sequence) {
        for (YAML::const_iterator it = ocsProvidersNode.begin(); it != ocsProvidersNode.end(); ++it)
            foundOcsProviders << QString::fromStdString(it->as<std::string>());
    }

    return std::move(foundOcsProviders);
}

void Settings::setOCSProviders(YAML::Node& root, const QStringList& ocsProviders) {
    YAML::Node ocsProvidersNode = YAML::Load("[]");
    for (const QString& url: ocsProviders)
        ocsProvidersNode.push_back(url.toStdString());

    root["ocs"] = ocsProvidersNode;
}
