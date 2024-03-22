cmake_minimum_required(VERSION 3.20)
project(BuriedTest)

# include_directories(. ../src ../src/third_party ../src/third_party/sqlite third_party/mbedtls/include)
include_directories(../src/third_party)

set(TEST_SRC
    # ../src/third_party/sqlite/sqlite3.c
    # ../src/database/database.cc
    # ../src/crypt/Crypt.cc
    test_crypt.cc
    test_db.cc
    test.cc)

add_executable(buried_test ${TEST_SRC})
target_link_libraries(buried_test Buried_static gtest)
