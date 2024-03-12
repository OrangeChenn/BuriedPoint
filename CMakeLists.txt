# 要求的最小CMAKE版本
cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "BuriedPoint")

# 项目信息
project(${PROJECT_NAME}
        VERSION 0.1.1
        DESCRIPTION "OrangeCheng"  
)

# 定义开关，表示是否编译动态库
option(BUILD_BURIED_SHARED_LIBS "build shared library" ON)

# 定义开关，表示是否编译静态库
option(BUILD_BURIED_STATIC_LIBS "build static library" ON)

# 定义开关，表示是否编译示例代码
option(BUILD_BURIED_EXAMPLES "build examples" OFF)

# 定义开关，表示是否编单元测试程序
option(BUILD_BURIED_TEST "build unittest" OFF)

set(INCDIRS . lib ${CMAKE_BINARY_DIR})
include_directories(${INCDIRS})

if(MVSC)
    # MSVC编译专用配置，表示启动多线程编译，如果是Linux平台下，需注释
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}/MP")

    set(LIBS ${LIBS} winmm iphlpapi ws2_32 dbghelp)

    # generate pdb file
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
    set(CMAKE_SHARED_LINKED_FLAGS_RELEASE "${CMAKE_EXE_LINKED_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

    # 表示启用c++20标准和c11标准
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
    set(CMAKE_C_FLAGS "${CMAKE_CFLAGS} /std:c11")
else()
    set(LIBS ${LIBS} dl pthread)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_CFLAGS} -std=c11 -fPIC")
endif()

#打印信息
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")

# Debug模式下定义DEBUG宏，Release模式下定义NDEBUG宏
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

# 添加src子目录到项目中，子目录中也会对应有一个CMakeLists.txt
add_subdirectory(src)

if(BUILD_BURIED_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_BURIED_TEST)
    include_directories(
        googletest/googletest
        googletest/googletest/include
    )
    add_subdirectory(googletest)
    add_subdirectory(tests)
endif()
