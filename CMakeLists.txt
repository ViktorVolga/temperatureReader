cmake_minimum_required(VERSION 3.5)
project(sh-temperature-reader)

# Correct C++ standard setup
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for language servers (clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(source)
add_subdirectory(tests)

add_executable(
    sh-temperature-reader
    ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
)

target_link_libraries(sh-temperature-reader ds2482 RomFinder w1 logger config app)
install(TARGETS sh-temperature-reader RUNTIME DESTINATION bin)