#include <cstdint>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "../src/third_party/boost/boost/asio/connect.hpp"
#include "../src/third_party/boost/boost/asio/ip/tcp.hpp"
#include "../src/third_party/boost/boost/beast/core.hpp"
#include "../src/third_party/boost/boost/beast/http.hpp"
#include "../src/third_party/boost/boost/beast/version.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace tcp = net::ip::tcp;

TEST(BuriedHttpTest, HttpPost) {
    try {
        auto const host = "localhost";
        auto const target = "/buried";
        int version = 11;

        net::io_context ioc;

        tcp::resolver resolver(ioc); // 解析host
        beast::tcp_stream stream(ioc); // 用于与服务器建立tcp连接

        boost::asio::ip::tcp::resolver::query query(host, "5678");
        auto const result = resolver(query);
        stream.connect(result); // 建立连接

        http::request<http::string_body> req{http::verb::post, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/json");
        req.body() = "{\"hello\":\"world\"}";
        req.prepare_payload(); // 构造整个请求体
        http::write(stream, req); // 发送请求

        beast::flat_buffer buffer; // 缓冲取，接受response
        http::response<http::dynamic_body> res;
        http::read(stream, buffer, res); // 接送应答

        std::string bdy = boost::beast::buffers_to_string(res.body().data());
        std::cout << "bdy " << bdy << std::endl;
        std::cout << "res " << res << std::endl;
        std::cout << "res code " << res.result_int() << std::endl;

        beast::error_code ec; // 处理错误
        stream.socket().shutdown(tcp::socket::shutdown_both, ec); // 关闭连接

        if(ec && ec != beast::errc::not_connected) throw beast::system_error{ec};
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }    
} 
