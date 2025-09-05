#include "../headers/dataSource.hpp"
#include <gtest/gtest.h>

class DataSourceTest : public testing::Test{
protected:
    json _config;
    DataSourceTest(){
        _config = json::parse(R"(
        {
            "id": 1,
            "type": "w1Buss",
            "master device": "ds2482",
            "connection with master": {
                "type": "i2c",
                "i2c buss": 1,
                "i2c address": "0x18",
                "i2c device path": "/dev/SH_TEMPERATURE_READER"
            }
        }
        )");
    }
    ~DataSourceTest() override = default;
    void SetUp() override {};
    void TearDown() override {};
};

TEST_F(DataSourceTest, constructorTest){
    boost::system::error_code ec;
    DataSource ds(_config, ec);
    /*проверяем поля*/
    ASSERT_EQ(ds._id, 1);
    ASSERT_STREQ("w1Buss", ds.m_type.c_str());
    ASSERT_STREQ("ds2482", ds.m_masterDevice.c_str());
    ASSERT_NE(ds.m_connectionContextPtr, nullptr);
    EXPECT_FALSE(ec);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}