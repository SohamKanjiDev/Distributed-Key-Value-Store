add_executable(
  kvstore_test
  kvstore_test.cpp
)
target_link_libraries(
  kvstore_test
  GTest::gtest_main
  KVStore
)

include(GoogleTest)
gtest_discover_tests(kvstore_test)