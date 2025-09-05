find_package(spdlog REQUIRED)

add_library(ds2482 ds2482.cpp)
target_link_libraries(ds2482 i2c-io)

add_library(RomFinder RomFinder.cpp)
target_link_libraries(RomFinder ds2482)

add_library(w1 w1.cpp)
target_link_libraries(w1 PUBLIC ds2482 RomFinder)

add_library(tSensor tSensor.cpp)

add_library(logger logger.cpp)
target_link_libraries(logger spdlog::spdlog)

include(FetchContent)
FetchContent_Declare(
    json 
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.12.0)

FetchContent_MakeAvailable(json)

include_directories(${json_SOURCE_DIR}/include)

add_library(config config.cpp)
target_link_libraries(config PUBLIC nlohmann_json::nlohmann_json)

add_library(i2c-io i2c-io.cpp)
target_link_libraries(i2c-io spdlog::spdlog config)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.78 COMPONENTS program_options locale system
REQUIRED) 

add_library(connectionContext connectionContext.cpp)
target_link_libraries(connectionContext PUBLIC
                    nlohmann_json::nlohmann_json
                    Boost::locale
                    Boost::system
                    Boost::program_options
                    )

add_library(dataSource dataSource.cpp)
target_link_libraries(dataSource PUBLIC connectionContext)
target_link_libraries(config PUBLIC dataSource)

add_library(programOptions programOptions.cpp)
target_link_libraries(programOptions PUBLIC Boost::program_options)

add_library(connectionHandler connectionHandler.cpp)
target_link_libraries(connectionHandler PUBLIC dataSource ds2482 w1)

add_library(app app.cpp)
target_link_libraries(app PUBLIC programOptions config connectionHandler)