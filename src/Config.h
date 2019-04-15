#pragma once

#include <QStringList>
#include <yaml-cpp/yaml.h>

class Config {
public:
    Config();

    QStringList getOCSProviders();

private:
    YAML::Node loadProvidersConfigFile();
    QStringList readOCSProviders(YAML::Node node);

    QStringList ocsProviders;
};
