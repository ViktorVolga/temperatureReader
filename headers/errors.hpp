#pragma once

#include <boost/system.hpp>

enum errorsEnum {
    json_is_not_object = 1,
    jsonDoesntHaveFieldType = 2,
    wrongConnectionType = 3,
    wrongI2cConnectionConfig = 4,
    wrongI2cAddress = 5,
    wrongDataSourceId = 6,
    wrongDataSourceType = 7,
    wrongDataSourceMasterDevice = 8,
    connectionConfigAbsent = 9,
    noSuchW1Buss = 10,
};

class TReaderErrorCategory : public boost::system::error_category {
    public:
        const char* name() const noexcept override {
            return "Temperature reader error";
        }

        std::string message(int ev) const override {
            switch (ev) {
                case json_is_not_object:
                    return "Passed json is not an object";
                case jsonDoesntHaveFieldType:
                    return "Wrong json format: json doesn't contain field \"type\" ";
                case wrongConnectionType:
                    return "Wrong connection type in config";
                case wrongI2cConnectionConfig:
                    return "Wrong i2c connection config format";
                case wrongI2cAddress:
                    return "Wrong i2c address in config";
                case wrongDataSourceId:
                    return "DataSource id is absent in config or not integer";
                case wrongDataSourceType:
                    return "DataSource type is absent in config or not string";
                case wrongDataSourceMasterDevice:
                    return "DataSource \"master device\" option is absent in config or not a string";
                case connectionConfigAbsent:
                    return "Object with connection context is absent in config";
                case noSuchW1Buss:
                    return "Requested w1 Buss is absent";
                default:
                    return "Unknown error";
            }
        }
};

static const boost::system::error_category& getErrorCategory() {
    static TReaderErrorCategory instance;
    return instance;
}