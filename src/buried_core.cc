#include "buried_core.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> Buried::Logger() { return logger_; }

void Buried::InitWorkPath(const std::string& work_path) {
    std::filesystem::path _work_path(work_path);
    if(!std::filesystem::exists(_work_path)) {
        std::filesystem::create_directories(_work_path);
    }
    work_path_ = _work_path / "buried";
    if(!std::filesystem::exists(work_path_)) {
        std::filesystem::create_directories(work_path_);
    }
}

void Buried::InitLogger() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    std::filesystem::path _log_dir = work_path_ / "buried.log";
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(_log_dir.string(), true);

    logger_ = std::shared_ptr<spdlog::logger>(new spdlog::logger("buried_sink", {console_sink, file_sink}));

    logger_->set_pattern("[%c] [%s:%#] [%l] %v");
    logger_->set_level(spdlog::level::trace);
    SPDLOG_LOGGER_TRACE(logger_, "Some trace message with param: {}", 42);
    SPDLOG_LOGGER_DEBUG(logger_, "Some debug message");
}

Buried::Buried(const std::string& work_dir) {
    InitWorkPath(work_dir);
    InitLogger();
}

Buried::~Buried() {}

BuriedResult Buried::Start(const Config& config) {
    return BuriedResult::kBuriedOk;
}

BuriedResult Buried::Report(std::string title, std::string data, uint32_t priority) {
    return BuriedResult::kBuriedOk;
}
