if(CMAKE_YOCTO_BUILD)
    set(IS_YOCTO_BUILD TRUE)
    message(STATUS "Yocto build detected")
else()
    set(IS_YOCTO_BUILD FALSE)
    message(STATUS "Local build detected")
endif()

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

if(IS_YOCTO_BUILD)
    find_library(FMT_LIB fmt)
    if(FMT_LIB)
        message(STATUS "Found fmt library: ${FMT_LIB}")
        link_libraries(${FMT_LIB})
    else()
        message(FATAL_ERROR "fmt library not found")
    endif()

    find_path(NLOHMANN_JSON_INCLUDE_DIR
        NAMES json.hpp nlohmann/json.hpp
        PATHS ${CMAKE_INSTALL_PREFIX}/include
        /usr/include
        /usr/local/include
    )
else()
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json 
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.12.0)
    FetchContent_MakeAvailable(nlohmann_json)
endif()

include_directories(${nlohmann_json_SOURCE_DIR}/include)

add_library(config config.cpp)
if(IS_YOCTO_BUILD)
    target_include_directories(config PRIVATE ${NLOHMANN_JSON_INCLUDE_DIR})
else()
    target_link_libraries(config PUBLIC nlohmann_json::nlohmann_json)
endif()

add_library(i2c-io i2c-io.cpp)
target_link_libraries(i2c-io spdlog::spdlog config)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.78 COMPONENTS program_options locale system REQUIRED)

add_library(connectionContext connectionContext.cpp)
if(IS_YOCTO_BUILD)
    target_link_libraries(connectionContext PUBLIC
                        Boost::locale
                        Boost::system
                        Boost::program_options
                        )
    target_include_directories(connectionContext PRIVATE ${NLOHMANN_JSON_INCLUDE_DIR})
else()
    target_link_libraries(connectionContext PUBLIC
                        nlohmann_json::nlohmann_json
                        nlohmann_json
                        Boost::locale
                        Boost::system
                        Boost::program_options
                        )

endif()

add_library(dataSource dataSource.cpp)
target_link_libraries(dataSource PUBLIC connectionContext)
target_link_libraries(config PUBLIC dataSource)

add_library(programOptions programOptions.cpp)
target_link_libraries(programOptions PUBLIC Boost::program_options)

add_library(connectionHandler connectionHandler.cpp)
target_link_libraries(connectionHandler PUBLIC dataSource ds2482 w1)

add_library(app app.cpp)
target_link_libraries(app PUBLIC programOptions config connectionHandler)


if(IS_YOCTO_BUILD)
    find_package(ZeroMQ REQUIRED)
    set(SOBJECTIZER_INCLUDE_DIR "/usr/include")
    message(STATUS "Using system SObjectizer from Yocto")
else()
    FetchContent_Declare(
      libzmq
      GIT_REPOSITORY https://github.com/zeromq/libzmq.git
      GIT_TAG v4.3.4
    )
    FetchContent_Declare(
    cppzmq
    GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
    GIT_TAG v4.9.0
    )

    FetchContent_MakeAvailable(libzmq cppzmq)

    FetchContent_Declare(
    sobjectizer
    GIT_REPOSITORY https://github.com/Stiffstream/sobjectizer.git
    GIT_TAG v5.8.4
    )

    FetchContent_MakeAvailable(sobjectizer)
endif()

add_library(zeroMqAgent zeroMqAgent.cpp)
if(IS_YOCTO_BUILD)
    target_link_libraries(zeroMqAgent libzmq)
else()
    target_link_libraries(zeroMqAgent libzmq cppzmq)
    target_include_directories(zeroMqAgent PRIVATE 
        ${cppzmq_SOURCE_DIR}
        ${libzmq_SOURCE_DIR}/include
    )
endif()

target_link_libraries(app PUBLIC zeroMqAgent)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

set(PROTO_FILES ../proto/messages.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(messageHandler messageHandler.cpp ${PROTO_SRCS})
target_link_libraries(messageHandler ${Protobuf_LIBRARIES} Boost::system)
target_include_directories(messageHandler PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(app PUBLIC messageHandler)