#pragma once

#include <memory>
#include <string>

#include "../common/common_service.h"

namespace spdlog {
    class logger;
}

namespace buried
{
    
struct BuriedData {
    std::string title;  // 标题
    std::string data;   // 详细数据
    uint32_t priority;  // 优先级
};

class BuriedReportImpl;

class BuriedReport {
public:
    BuriedReport(std::shared_ptr<spdlog::logger> logger,
                CommonService common_service, std::string work_path);
    ~BuriedReport();

    void Start();
    void InsertData(const BuriedData& data);

private:
    std::unique_ptr<BuriedReportImpl> ipml_;
};

} // namespace buried

