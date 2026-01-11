#include "../headers/connectionHandler.hpp"

#include "../headers/logger.hpp"


/*
    to do later
    this function in future we should break on parts
    inside should be calls
    addConnection()
    addMasterDevice()
    addBuss()
*/
void ConnectionHandler::addDataSource(const DataSource* dataSource)
{
    if(!dataSource->m_connectionContextPtr)
    {
        SHTLogger()->error("[ConnectionHandler::addDataSource]: connectionContextPtr is null");
        return;
    }

    connectionType connectionType = dataSource->m_connectionContextPtr->getConnectionType();
    switch (connectionType)
    {
        case connectionType::error :
            SHTLogger()->error("[ConnectionHandler::addDataSource]: unknown connection Type");
            break;
        case connectionType::i2c :
            i2cContext * context = dynamic_cast<i2cContext*> (dataSource->m_connectionContextPtr.get());
            if(!context){
                SHTLogger()->error("[ConnectionHandler::addDataSource]: error while dynamic_cast to i2cContext *");
                break;
            }
            if(m_i2cBusses.count(context->m_i2cBuss) == 0){
                std::string ioFileName = "/dev/SH_TEMPERATURE_READER";
                m_i2cBusses[context->m_i2cBuss] = std::make_shared<Filei2cIO>(std::move(ioFileName));
                i2cId id(context->m_i2cBuss, context->m_address);
                if(m_ds2482Map.count(id) == 0 && dataSource->m_masterDevice == "ds2482")
                {
                    m_ds2482Map[id] = std::make_shared<DS2482>(std::weak_ptr<i2cIO>(m_i2cBusses[context->m_i2cBuss]));
                }
                if(dataSource->m_type == "w1Buss" and m_w1Busses.count(id) == 0)
                {
                    m_w1Busses[id] = std::make_shared<w1Buss>(m_ds2482Map[id]);
                }
            }
            break;
    }
}

std::weak_ptr<w1Buss> ConnectionHandler::getW1Buss(uint8_t buss, uint8_t address, boost::system::error_code& ec)
{
    if(m_w1Busses.count(/*i2cId*/{buss, address}) == 0)
    {
        ec = boost::system::error_code(errorsEnum::noSuchW1Buss, getErrorCategory());
        std::weak_ptr<w1Buss> empty;
        return empty;
    }
    return m_w1Busses[{/*i2cId*/buss, address}];
}

