find_package(spdlog REQUIRED)

add_library(i2c-io i2c-io.cpp)
target_link_libraries(i2c-io spdlog::spdlog)

add_library(ds2482 ds2482.cpp)
target_link_libraries(ds2482 i2c-io)

add_library(RomFinder RomFinder.cpp)
target_link_libraries(RomFinder ds2482)

add_library(w1 w1.cpp)
target_link_libraries(w1 ds2482)

add_library(tSensor tSensor.cpp)
