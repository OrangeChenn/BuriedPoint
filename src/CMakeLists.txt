# 表示项目中的头文件只会被包含一次
include_guard(GLOBAL)

include_directories(. third_party third_party/spdlog/include third_party/boost third_party/mbedtls/include)

add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

# 一定要定义这个宏，否则会导致Custom Formatting功能异常
# 详见 https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/mbedtls/library)

set(BURIED_SRCS
    Buried.cc
    BuriedCore.cc)

set(MODULE_NAME "Buried")

message(STATUS "libs ${LIBS}")

# 编译动态库
if(BUILD_BURIED_SHARED_LIBS)
    add_library(${MODULE_NAME}_shared SHARED ${BURIED_SRCS})
    target_link_libraries(${MODULE_NAME}_shared PRIVATE ${LIBS} spdlog mbedtls_static)
endif()

# 编译静态库
if(BUILD_BURIED_STATIC_LIBS)
    add_library(${MODULE_NAME}_static STATIC ${BURIED_SRCS})
    target_link_libraries(${MODULE_NAME}_static PRIVATE ${LIBS} spdlog mbedtls_static)
endif()

# 编译可执行文件
# if(BUILD_BURIED_EXE)
#     add_executable(${MODULE_NAME} ${BURIED_SRCS})
#     target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS})
# endif()
