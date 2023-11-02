#pragma once
#include <functional>

#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using Tcp = boost::asio::ip::tcp;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
{
public:
    WebSocketSession(Tcp::socket socket, std::function<void(std::string)> onMessage);
    void start();

private:
    void readMessage(Tcp::socket socket);
    beast::multi_buffer _buffer;
    Tcp::socket _socket;
    std::mutex  _mutex;
    std::function<void(std::string)> _onMessage;
};



class AsyncWebSocketServer
{
public:
    AsyncWebSocketServer(std::function<void(std::string)> onMessage, boost::asio::io_context& io_context, const std::string& address, short port);
    ~AsyncWebSocketServer()= default;
    void sendMessages(const std::string& msg);

private:
    void startAccept();
    void handleClients(Tcp::socket socket) const;
    void subscribeForNewMessage(const std::string& msg) const;
    Tcp::acceptor _acceptor;
    boost::asio::io_context _ioContext;
    std::function<void(std::string)> _onMessage;
};
