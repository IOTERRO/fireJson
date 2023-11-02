#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using Tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


class SyncWebSocketServer
{
public:
    SyncWebSocketServer(boost::asio::ip::address address, unsigned short port, std::string replyMessage);
    ~SyncWebSocketServer();
    void createWebSocketServer();

    void sendMessages(const std::string& msg);

private:
    void handleWebSocket(Tcp::socket s);
    boost::asio::ip::address _address;
    const unsigned short _port;
    std::string _replyMessage;
};