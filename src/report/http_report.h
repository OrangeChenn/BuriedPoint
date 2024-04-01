#pragma once

#include <memory>

namespace spdlog {
    class logger;
}

namespace buried {

class HttpReport {
public:
    explicit HttpReport(std::shared_ptr<spdlog::logger> logger);

    HttpReport& Host(const string& host) {
        host_ = host;
        return *this;
    }

    HttpReport& Topic(const std::string& topic) {
        topic_ = topic;
        return *this;
    }

    HttpReport& Port(const std::string& port) {
        port_ = port;
        return *this;
    }

    HttpReport& Body(const std::string& body) {
        body_ = body;
        return *this;
    }

    bool Report();
private:
    std::string host_;
    std::string topic_;
    std::string port_;
    std::string body_;

    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace buried
