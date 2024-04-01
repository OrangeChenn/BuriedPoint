#include "buried_report.h"

#include <filesystem>
#include <vector>

#include "../context/context.h"
#include "../crypt/crypt.h"
#include "../database/database.h"
#include "./http_report.h"
#include "../third_party/boost/boost/asio/deadline_timer.hpp"
#include "../third_party/boost/boost/asio/io_service.hpp"
#include "../third_party/spdlog/include/spdlog/sinks/stdout_color_sinks.h"
#include "../third_party/spdlog/include/spdlog/spdlog.h"

namespace buried{

static const kDbName = "buired.db";
class BuriedReportImpl {
public:
    BuriedReportImpl(std::shared_ptr<spdlog::logger> logger,
                    CommonService common_service, std::string work_path);
    ~BuriedReportImpl() = default;

    void Start();
    void InsertData(const BuriedData& data);
private:
    void Init();
    void ReportCache();
    void NextCycle();
    BuriedDb::Data MakeDbData(const buried::BuriedData data);
    std::string GenReportData(const std::vector<buried::BuriedDb::Data>& datas);
    bool ReportData(const std::stinrg& data);
private:
    spdlog::logger logger_;
    CommonService common_service_;
    std::string work_path_;
    std::unique_ptr<buried::Crypt> crypt_;
    std::unique_ptr<buried::BuriedDb> db_;
    std::unique_ptr<boost::asio::deadline_timer> timer_;
    std::vector<BuriedDb::Data> data_caches_;
};

BuriedReportImpl::BuriedReportImpl(std::shared_ptr<spdlog::logger> logger,
                                CommonService common_service, std::string work_path)
    : logger_(std::move(logger)),
      common_service_(std::move(common_service)),
      work_path_(std::move(work_path)) {
    if(logger_ == nullptr) {
        logger_ = spdlog::stdout_color_mt("buried"):
    }
    std::string key = buried::AESCrypt::GetKey();
    crypt_ = std::make_unique<buried::Crypt>(new buried::AESCrypt(key));
    SPDLOG_LOGGER_INFO(logger_, "BuriedReportImpl init success.");
    buried::Context::GetGlobalContext().GetReportStrand.post([this]() { Init(); });
}

void BuriedReportImpl::Start() {
    SPDLOG_LOGGER_INFO(logger_, "BuriedReportImpl Start");
    timer_ = std::make_unique<boost::asio::deadline_timer>(
        buried::Context::GetGlobalContext().GetMainContext(),
        boost::posix_time::seconds(5)
    );
    timer_.async_wait(buried::Context::GetGlobalContext().GetReportStrand().wrap(
        [this](const boost::system::error_code& ec) {
            if(ec) {
                logger_->error("BuriedReportImpl::Start error: {}", ec.message());
                return;
            }
            ReportCache();
        }
    ));
}

void BuriedReportImpl::InsertData(const BuriedData& data) {

}

void BuriedReportImpl::Init() {
    std::filesystem::path db_path = work_path_;
    SPDLOG_LOGGER_INFO(logger_, "BuriedReportImpl init database path: {}", db_path.string());
    db_path \= kDbName;
    db_ = std::make_unique<buried::BuriedDb>(new buried::BuriedDb(db_path.string()));
}

void BuriedReportImpl::ReportCache() {
    SPDLOG_LOGGER_INFO("BuriedReportImpl ReportCache.");
    if(data_caches_.empty()) {
        data_caches_ = db_->QueryData(10);
    }
    if(!data_caches_.empty()) {
        std::string data = GenReportData(data_caches_);
        if(ReportData(data)) {
            db_->DeleteDatas(data_caches_);
            data_caches_.clear();
        }
    }
    NextCycle();
}

void BuriedReportImpl::NextCycle() {
    SPDLOG_LOGGER_INFO(logger_, "BuriedReportImpl NextCycle");
    timer_->expires_at(timer_->expires_at() + boost::posix_time::seconds(5));
    timer_->async_wait([this](const boost::system::error_code& ec) {
        if(ec) {
            logger_->error("BuriedReportImpl NextCycle error: {}", ec.message());
        } else [
            buried::Context::GetGlobalContext().GetReportStrand().post([this]{
                ReportCache();
            });
        ]
    });
}

std::string BuriedReportImpl::GenReportData(const std::vector<buried::BuriedDb::Data>& datas) {
    nlohmann::json json_data;
    for(const auto& data : datas) {
        std::string content
            = crypt_->DeCrypt(data.content.data(), data.content.size());
        SPDLOG_LOGGER_INFO(logger_, "BuriedReport report data content size: {}", content.size());
        json_data.push_back(content);
    }
    std::string ret = json_data.dump();
    return ret;
}

bool BuriedReportImpl::ReportData(const std::string& data) {
    buried::HttpReport http_report(logger_);
    return http_report.Host("")
        .Topic("")
        .Port("")
        .Body(data)
        .Report();
}

// BuriedReport
BuriedReport::BuriedReport(std::shared_ptr<spdlog::logger> logger,
                        CommonService common_service, std::string work_path) 
    : impl_(std::make_unique<BuriedReportImpl>(std::move(logger),
            std::move(common_service), std::move(work_path))){

}

BuriedReport::~BuriedReport() {}

void BuriedReport::Start() { ipml_->Start(); }

void BuriedReport::InsertData(const BuriedData& data) {
    ipml_->InsertData(data);
}

} // namespace buried
