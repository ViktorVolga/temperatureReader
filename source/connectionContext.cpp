#include "../headers/connectionContext.hpp"
#include "../headers/logger.hpp"

const std::string &MDConnectionContext::getTypeString()
{
    return m_connectionType;
}

MDConnectionContext::~MDConnectionContext()
{

}

/*to do later - return std optioanl and use boost:ec to diagnose problems*/
connectionType MDConnectionContext::getConnectionType() const
{
    if(connectionTypesMap.count(m_connectionType))
        return connectionTypesMap[m_connectionType];
    else 
        return connectionType::error;
}

i2cContext::i2cContext(i2cContext && other) 
: MDConnectionContext(std::move(other.m_connectionType))
{
    m_i2cBuss = other.m_i2cBuss;
    m_address = other.m_address;
    _devicePath = std::move(other._devicePath);
}

i2cContext::i2cContext(uint8_t buss,
                        uint8_t address,
                        const char * devicePath)
: MDConnectionContext("i2c")
{
    m_i2cBuss = buss;
    m_address = address;
    _devicePath = devicePath;
}

bool i2cContext::operator<(const i2cContext &other) const
{
    if (other.m_i2cBuss == m_i2cBuss)
        return m_address < other.m_address;
    else
        return m_i2cBuss < other.m_i2cBuss;
}

MDConnectionContextPtr ConnectionContextFabrica::getConnectionContext(const json &connectionContext, boost::system::error_code & ec)
{
    if(!connectionContext.is_object())
    {
        ec = boost::system::error_code(errorsEnum::json_is_not_object, getErrorCategory());
        return nullptr;
    }
    std::string connectionType;
    if(connectionContext.contains("type") && connectionContext["type"].is_string())
    {
        connectionType = connectionContext["type"];
    } else {
        ec = boost::system::error_code(errorsEnum::jsonDoesntHaveFieldType, getErrorCategory());
        return nullptr;
    }
    if(not connectionTypesMap.count(connectionType)){
        ec = boost::system::error_code(errorsEnum::wrongConnectionType, getErrorCategory());
        return nullptr;
    }

    switch(connectionTypesMap[connectionType]){
        case connectionType::i2c:
            if(connectionContext.contains("i2c buss") and connectionContext["i2c buss"].is_number_integer() and
                connectionContext.contains("i2c address") and connectionContext["i2c address"].is_string() and
                connectionContext.contains("i2c device path") and connectionContext["i2c device path"].is_string()){
                    uint8_t address;
                    try{
                        address = stoi(connectionContext["i2c address"].template get<std::string>(), 0, 16);
                    } catch (...) {
                        ec = boost::system::error_code(errorsEnum::wrongI2cAddress, getErrorCategory());
                        return nullptr;
                    }
                    return std::make_unique<i2cContext>(connectionContext["i2c buss"].template get<int>(), 
                                                        address,
                                                        connectionContext["i2c device path"].template get<std::string>().c_str());
            } else {
                ec = boost::system::error_code(errorsEnum::wrongI2cConnectionConfig, getErrorCategory());
                return nullptr;
            }
        default :
            ec = boost::system::error_code(errorsEnum::wrongConnectionType, getErrorCategory());
            return nullptr;
    }

    return MDConnectionContextPtr();
}