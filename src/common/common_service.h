#pragma once

#include <string>

#include "../third_party/nlohmann/json.hpp"

namespace buried {

struct CommonService {
public:
    std::string host;
    std::string port;
    std::string topic;
    std::string user_id;
    std::string app_version;
    std::string app_name;

    nlohmann::json custom_data;

    std::string system_version;
    std::string device_id;
    std::string device_name;
    std::string buried_version;
    std::string lifecycle_id;       // 标识一次进程的生命周期
public:
    CommonService();

    static std::string GetProcessTime();
    static std::string GetNowDate();
    static std::string GetRandomId();

private:
    void Init();
};

} // namespace buried
