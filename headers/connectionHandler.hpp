#pragma once

#include "../headers/dataSource.hpp"
#include "../headers/i2c-io.hpp"
#include "../headers/ds2482.hpp"
#include "../headers/w1.hpp"
#include "../headers/errors.hpp" 

#include <map>
#include <memory>


/**
 * @brief Central coordinator that creates and reuses low-level IO objects for data sources.
 *
 * Maintains maps of I2C buses, master chips (e.g. DS2482), and 1-Wire buses.
 */
class ConnectionHandler
{
    std::map<uint8_t /*i2c-buss*/, std::shared_ptr<i2cIO>> m_i2cBusses;
    std::map<i2cId, std::shared_ptr<DS2482>> m_ds2482Map;
    std::map<i2cId, std::shared_ptr<w1Buss>> m_w1Busses;
public:
    ConnectionHandler() {};
    /**
     * @brief Add a data source and ensure the required bus/master devices exist.
     *
     * Expects the `DataSource` to contain a valid connection context. If the context
     * is null or incompatible, the function logs an error and performs no changes.
     *
     * @param dataSource Non-owning pointer to the data source to register.
     */
    void addDataSource(const DataSource* dataSource);
    std::weak_ptr<w1Buss> getW1Buss(uint8_t buss, uint8_t address, boost::system::error_code& ec);
};