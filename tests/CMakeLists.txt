cmake_minimum_required(VERSION 3.20)
project(BuriedTest)

include_directories(. ../src ../src/third_party ../src/third_party/sqlite)

set(TEST_SRC
    ../src/third_party/sqlite/sqlite3.c
    ../src/database/database.cc
    test_db.cc
    test.cc)

add_executable(buried_test ${TEST_SRC})
target_link_libraries(buried_test Buried_static gtest)
