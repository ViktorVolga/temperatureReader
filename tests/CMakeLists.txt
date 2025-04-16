find_package(GTest REQUIRED)

add_executable(test-romFinder test-romFinder.cpp)
target_link_libraries(test-romFinder PRIVATE GTest::GTest RomFinder)
add_test(romFinderTest test-romFinder)