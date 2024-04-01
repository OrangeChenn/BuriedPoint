#include "http_report.h"

#include "../third_party/boost/boost/asio/connect.hpp"
#include "../third_party/boost/boost/asio/io_context.hpp"
#include "../third_party/boost/boost/asio/ip/tcp.hpp"
#include "../third_party/boost/boost/beast/core.hpp"
#include "../third_party/boost/boost/beast/http.hpp"
#include "../third_party/boost/boost/version.hpp"
#include "../third_party/spdlog/include/spdlog/spdlog.h"

namespace beast = boost::beast;     // boost/beast.hpp
namespace http = beast::http;       // boost/beast/http.hpp
namespace net = boost::asio;        // boost/asio.hpp
namespace tcp net::ip::tcp          // boost/asio/ip/tcp.hpp

namespace buried {

static boost::asio::io_context ioc;

HttpReport::HttpReport(std::shared_ptr<spdlog::logger> logger) : logger_(logger) {}

bool HttpReport::Report() {
    try
    {
        int version = 11;

        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc);

        boost::asio::ip::tcp::resolver::query query(host_, port_);
        auto const results = resolver.query(query);
        stream.connect(results);

        http::request<http::string_body> req{http::verb::post, topic_, version};
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::content_type, "application/json");
        req.body() = body_;
        req.prepare_payload();
        http::write(stream, req);

        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(stream, buffer, res);

        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
        if(ec && ec != beast::errc::not_connected) throw beast::system_error{ec};

        auto res_status = res.result();
        if(res_status != http::status::ok) {
            SPDLOG_LOGGER_ERROR(logger_, 
                                "report error " + std::to_string(res.result_int()));
            return false;
        }
    }
    catch(const std::exception& e)
    {
        SPDLOG_LOGGER_ERROR(logger_, "report error " + std::string(e.what()));
        return false;
    }
    return true;
}

} // namespace buried
