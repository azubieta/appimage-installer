// libraries
#include <QFile>
#include <QStandardPaths>
#include <QDebug>

// local
#include "Config.h"

Config::Config() {
    YAML::Node node = loadProvidersConfigFile();
    if (node.IsDefined())
        ocsProviders = readOCSProviders(node);
}

YAML::Node Config::loadProvidersConfigFile() {
    if (QFile::exists("appimage-providers.yaml"))
        return YAML::LoadFile("appimage-providers.yaml");

    QStringList configLocations = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    for (const QString& location: configLocations) {
        QString filePath = location + "/appimage-providers.yaml";
        if (QFile::exists(filePath))
            return YAML::LoadFile(filePath.toStdString());
    }

    qWarning() << "No providers config file found";
}

QStringList Config::readOCSProviders(YAML::Node node) {
    QStringList foundOcsProviders;

    YAML::Node ocsProvidersNode = node["ocs"];
    if (ocsProvidersNode && ocsProvidersNode.Type() == YAML::NodeType::Sequence) {
        for (YAML::const_iterator it = ocsProvidersNode.begin(); it != ocsProvidersNode.end(); ++it)
            foundOcsProviders << QString::fromStdString(it->as<std::string>());
    }

    return std::move(foundOcsProviders);
}

QStringList Config::getOCSProviders() {
    return ocsProviders;
}
