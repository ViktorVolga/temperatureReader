#pragma once

#include <string>
#include <map>
#include <fstream>
#include <nlohmann/json.hpp>

#include "dataSource.hpp"

using json = nlohmann::json;

class Config
{
protected:
    bool        _storeInFile    {true};
    std::string _localPath;
    json _json;
    std::vector<std::unique_ptr<DataSource>> _dataSources;
public:
    Config(std::string path, bool storeInFile);
    ~Config();
    const std::vector<std::unique_ptr<DataSource>> & getDataSources() const;
};

