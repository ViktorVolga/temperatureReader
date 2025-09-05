#include "../headers/connectionContext.hpp"
#include <gtest/gtest.h>

TEST(MDConnectionContext, Constructor){
    std::string type = "w1Buss";
    MDConnectionContext connectionContext(std::move(type));
    EXPECT_STREQ(connectionContext.getTypeString().c_str() , "w1Buss");
}

TEST(i2cContext, Constructor){
    MDConnectionContextPtr connectionContext = std::make_unique<i2cContext>(1, 0x18, "/dev/SH_TEMPERATURE_READER");
    MDConnectionContext * mdConnectionContext = connectionContext.get();
    i2cContext * i2cCon = dynamic_cast<i2cContext*>(mdConnectionContext);
    EXPECT_EQ(i2cCon->m_i2cBuss, 1);
    EXPECT_EQ(i2cCon->m_address, 0x18);
    EXPECT_STREQ(i2cCon->_devicePath.c_str(), "/dev/SH_TEMPERATURE_READER");
    EXPECT_STREQ(connectionContext->m_connectionType.c_str(), "i2c");
}

class ConnectionContextFabricaTest : public testing::Test{
protected:
    json _config;
    ConnectionContextFabricaTest(){
        _config = json::parse(R"(
        {
            "type": "i2c",
            "i2c buss": 1,
            "i2c address": "0x18",
            "i2c device path": "/dev/SH_TEMPERATURE_READER"
        }
        )");
    }
    ~ConnectionContextFabricaTest() override = default;
    void SetUp() override {};
    void TearDown() override {};
};

TEST_F(ConnectionContextFabricaTest, getI2cContext){
    ConnectionContextFabrica ccf;
    boost::system::error_code ec;
    MDConnectionContextPtr connectionContext = ccf.getConnectionContext(_config, ec);
    MDConnectionContext * mdConnectionContext = connectionContext.get();
    EXPECT_NE(mdConnectionContext, nullptr);
    i2cContext * i2cConnectionContext = dynamic_cast<i2cContext*>(mdConnectionContext);
    EXPECT_NE(i2cConnectionContext, nullptr);
    EXPECT_EQ(i2cConnectionContext->m_i2cBuss, 1);
    EXPECT_EQ(i2cConnectionContext->m_address, 0x18);
    EXPECT_STREQ(i2cConnectionContext->_devicePath.c_str(), "/dev/SH_TEMPERATURE_READER");
    EXPECT_STREQ(mdConnectionContext->m_connectionType.c_str(), "i2c");
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}