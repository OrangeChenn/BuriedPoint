include_directories(. .. ../src ../src/context ../src/third_party ../src/third_party/boost)

add_executable(buried_example_static buried_example.cc)
target_link_libraries(buried_example_static buried_static)

add_executable(buried_example_shared buried_example.cc)
target_link_libraries(buried_example_shared buried_shared)

add_executable(context_example_static context_example.cc)
target_link_libraries(context_example_static buried_static)
