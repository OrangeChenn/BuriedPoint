#pragma once

#include <stdint.h>

#ifdef __WIN32
    #define BURIED_EXPORT __declspec(dllexport)
#else
    #define BURIED_EXPORT
#endif

extern "C" {
    BURIED_EXPORT int BuriedTest();

    typedef struct Buried Buried;
    struct BuriedConfig {
        const char* host;           // 要上报的url
        const char* port;           // 对应的端口号
        const char* topic;          // 对应的topic，url:port/topic
        const char* user_id;        // 用户的id
        const char* app_version;    // 应用的版本号
        const char* app_name;       // 应用的名称
        const char* custom_data;    // 自定义数据，Json的字符串
    };

    // 创建一个Buried实例
    BURIED_EXPORT Buried* BuriedCreate(const char* work_dir);

    // 销毁一个Buried实例
    BURIED_EXPORT void BuriedDestory(Buried* buried);

    // 开启埋点上报能力
    BURIED_EXPORT uint32_t BuriedStart(Buried* buried, BuriedConfig* config);

    // 上报具体的数据
    BURIED_EXPORT uint32_t BuriedReport(Buried* buried, const char* title, const char* data, uint32_t priority);
}
