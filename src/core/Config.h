#pragma once

#include <QStringList>
#include <yaml-cpp/yaml.h>

/**
 * Provides access to the AppImage User tools configurations
 */
class Config {
public:
    Config();

    QStringList getOCSProviders();

private:
    YAML::Node loadProvidersConfigFile();

    QStringList readOCSProviders(YAML::Node node);

    QStringList ocsProviders;
};
