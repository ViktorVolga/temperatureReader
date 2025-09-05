#include "../headers/config.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "../headers/logger.hpp"
#include <boost/system.hpp>

Config::Config(std::string path, bool storeInFile = true)
: _localPath(std::move(path)), _storeInFile(storeInFile)
{
    SHTLogger()->info("Starting read configuration");
    std::ifstream inputStream(_localPath);
    _json = json::parse(inputStream);

    for (const auto & dataSourceJson : _json["Temperature Reader"]["data sources"])
    {
        SHTLogger()->info("found datasource");
        boost::system::error_code ec;
        std::unique_ptr<DataSource> dataSource = std::make_unique<DataSource>(dataSourceJson, ec);
        if(ec)
            SHTLogger()->info("filed to parse data source. error: {}", ec.message());
        else
            _dataSources.push_back(std::move(dataSource));
    }
    SHTLogger()->info("configuration succsessfull readed");
}


Config::~Config()
{

}

const std::vector<std::unique_ptr<DataSource>> &Config::getDataSources() const
{
    return _dataSources;
}
