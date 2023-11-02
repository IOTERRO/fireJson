#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using Tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class MyWebSocketClient
{
public:
    MyWebSocketClient(std::string host, unsigned short port);
    ~MyWebSocketClient() = default;

    enum class WsState {
        Connect = 0,
        HandShake,
        SendMessage,
        ReceiveMessage,
        Close
    };

    void wsClient(Tcp::socket& socket, net::io_context& ioc, websocket::stream<Tcp::socket>& ws, WsState stateWs, std::string* message = new std::string(""));

private:
    void sendMessage(websocket::stream<Tcp::socket>& ws, const std::string message);
    std::string receiveMessage(websocket::stream<Tcp::socket>& ws);
    void close(websocket::stream<Tcp::socket>& ws);

    std::string _host;
    unsigned short _port;
};