cmake_minimum_required(VERSION 3.5)
project(sh-temperature-reader)
set(CMAKE_CXX_STANDART 23)

enable_testing()

add_subdirectory(source)
add_subdirectory(tests)

add_executable(
    sh-temperature-reader
    ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
)

target_link_libraries(sh-temperature-reader ds2482 RomFinder w1 logger)
install(TARGETS sh-temperature-reader RUNTIME DESTINATION bin)