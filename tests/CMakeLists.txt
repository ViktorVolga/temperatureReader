find_package(GTest REQUIRED)

add_executable(test-romFinder test-romFinder.cpp)
target_link_libraries(test-romFinder PRIVATE GTest::GTest RomFinder)
add_test(romFinderTest test-romFinder)

add_executable(test-connectionContext test-connectionContext.cpp)
target_link_libraries(test-connectionContext PRIVATE GTest::GTest connectionContext)
add_test(connectionContext-test test-connectionContext)

add_executable(test-dataSource test-dataSource.cpp)
target_link_libraries(test-dataSource PRIVATE GTest::GTest dataSource)
add_test(dataSource-test test-dataSource)