#pragma once
#include <functional>
#include <map>

#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using Tcp = boost::asio::ip::tcp;




class AsyncWebSocketServer
{
public:

    enum class WsStatus
    {
        ClientConnected,
        ClientDisconnected,
        Unknown
    };

    struct ClientInfo
    {
        std::string  ip;
        unsigned int id;
        unsigned int clientsCount;
    };


    AsyncWebSocketServer(std::function<void(std::string)> onMessage, boost::asio::io_context& io_context, const std::string& address, short port,
                         std::function<void(WsStatus, ClientInfo)> onWsStatus = [](WsStatus, ClientInfo) { /* default implementation */ });
    ~AsyncWebSocketServer();
    void sendMessages(const std::string& msg);

private:
    std::string getMacAddress();
    unsigned getClientId();
    void startAccept();
    void handleClients(Tcp::socket socket, const ClientInfo& clientInfo);
    void subscribeForNewMessage(const std::string& msg) const;
    void removeClient(unsigned int id);
    Tcp::acceptor _acceptor;
    boost::asio::io_context _ioContext;
    std::function<void(std::string)> _onMessage;
    std::function<void(WsStatus, ClientInfo)> _onWsStatus;
    std::map<std::string, unsigned int> _clientsConnectionMap;
    unsigned int getRandomUnsignedInt();
};

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
{
public:
    WebSocketSession(Tcp::socket socket, AsyncWebSocketServer::ClientInfo clientInfo, std::function<void(std::string)> onMessage,
                     std::function<void(AsyncWebSocketServer::WsStatus, AsyncWebSocketServer::ClientInfo)> onWsStatus);
    void start();

private:
    void readMessage(Tcp::socket socket);
    beast::multi_buffer _buffer;
    Tcp::socket _socket;
    std::mutex  _mutex;
    AsyncWebSocketServer::ClientInfo _clientInfo;
    std::function<void(std::string)> _onMessage;
    std::function<void(AsyncWebSocketServer::WsStatus, AsyncWebSocketServer::ClientInfo)> _onWsStatus;
};