# 表示项目中的头文件只会被包含一次
include_guard(GLOBAL)

include_directories(. third_party third_party/sqlite third_party/spdlog/include third_party/boost third_party/mbedtls/include)

add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/buried_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/buried_config.h)

# 一定要定义这个宏，否则会导致Custom Formatting功能异常
# 详见 https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/mbedtls/library)

set(DB_SRCS third_party/sqlite/sqlite3.c database/database.cc)

set(BURIED_SRCS
    ${DB_SRCS}
    common/common_service.cc
    report/buried_report.cc
    report/http_report.cc
    crypt/crypt.cc
    context/context.cc
    buried.cc
    buried_core.cc)

set(MODULE_NAME "buried")

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
