#pragma once

#include <stdint.h>

#include <filesystem>
// #include <experimental/filesystem>
#include <memory>
#include <string>

#include "buried_common.h"
#include "include/buried.h"

namespace spdlog {
    class logger;
}

struct Buried {
  public:
    struct Config {
        std::string host;
        std::string port;
        std::string topic;
        std::string user_id;
        std::string add_version;
        std::string add_name;
        std::string custom_data;
    };
  public:
    Buried(const std::string& word_dir);
    ~Buried();
    BuriedResult Start(const Config& config);
    BuriedResult Report(std::string title, std::string data, uint32_t priority);

  public:
    std::shared_ptr<spdlog::logger> Logger();

  private:
    void InitWorkPath(const std::string& work_dir);
    void InitLogger();

  private:
    std::shared_ptr<spdlog::logger> logger_;
    std::filesystem::path work_path_;
};
