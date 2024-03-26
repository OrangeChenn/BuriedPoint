#include "include/buried.h"

#include <iostream>

#include "buried_core.h"

extern "C" {
int BuriedTest() {
    std::cout << "Test..." << std::endl;
    return 1;
}

Buried* BuriedCreate(const char* work_dir) {
    if(!work_dir) {
        return nullptr;
    }
    return new Buried(work_dir);
}

void BuriedDestory(Buried* buried) {
    if(buried) {
        delete buried;
    }
}

uint32_t BuriedStart(Buried* buried, BuriedConfig* config) {
    Buried::Config buried_config;
    if(config->host) {
        buried_config.host = config->host;
    }
    if(config->port) {
        buried_config.port = config->port;
    }
    if(config->topic) {
        buried_config.topic = config->topic;
    }
    if(config->user_id) {
        buried_config.user_id = config->user_id;
    }
    if(config->app_version) {
        buried_config.add_version = config->app_version;
    }
    if(config->app_name) {
        buried_config.add_name = config->app_name;
    }
    if(config->custom_data) {
        buried_config.custom_data = config->custom_data;
    }

    return buried->Start(buried_config);
}

uint32_t BuriedReport(Buried* buried, const char* title, const char* data, uint32_t priority) {
    if(!title || !data) {
        return BuriedResult::KBuriedInvalidParam;
    }
    return buried->Report(title, data, priority);
}

}
