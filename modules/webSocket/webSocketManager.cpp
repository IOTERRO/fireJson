#include <fstream>
#include "webSocketManager.h"

#include <string>


using namespace boost;
static asio::io_context ioContext;


WebSocketManager::WebSocketManager(std::string host, const std::string& port, std::function<void(std::string)> onMessage):
    _onMessage(std::move(onMessage)),
    _host(std::move(host)),
    _port(static_cast<unsigned short>(std::stoi(port)))
{
}

WebSocketManager::WebSocketManager(std::string host, const std::string& port):
    _host(std::move(host)),
    _port(static_cast<unsigned short>(std::stoi(port)))
{

}

WebSocketManager::~WebSocketManager()
{
    ioContext.reset();
    _myAsyncServer.reset();
    _mySyncServer.reset();
}

void WebSocketManager::createSyncWebSocketServer()
{
    auto address = net::ip::make_address(_host);
     _mySyncServer = std::make_shared<SyncWebSocketServer>(address, _port, _serverReplyMessage);
     _mySyncServer->createWebSocketServer();
} 

void WebSocketManager::createAsyncWebSocketServer()
{
    try
    {
        _myAsyncServer = std::make_shared<AsyncWebSocketServer>([this](std::string msg) {
            subscribeForNewMessage(msg);
            }, ioContext, _host, _port);
        ioContext.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int WebSocketManager::webSocketClient()
{
    try 
    {
        MyWebSocketClient client(_host, _port);
        net::io_context ioc;
        Tcp::socket socket(ioc);

        client.wsClient(socket, ioc, websocket::stream<Tcp::socket>(std::move(socket)), MyWebSocketClient::WsState::Connect);
        websocket::stream<Tcp::socket> ws(std::move(socket));
        client.wsClient(socket, ioc, ws, MyWebSocketClient::WsState::HandShake);
        client.wsClient(socket, ioc, ws, MyWebSocketClient::WsState::SendMessage, &_clientSendMessage);
        client.wsClient(socket, ioc, ws, MyWebSocketClient::WsState::ReceiveMessage, &_clientSendMessage);
        client.wsClient(socket, ioc, ws, MyWebSocketClient::WsState::Close);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

void WebSocketManager::subscribeForNewMessage(const std::string& msg) const
{
    _onMessage(msg);
}

void WebSocketManager::sendWsMessage(const std::string& msg) const
{
    if (_mySyncServer != nullptr)
    {
        _mySyncServer->sendMessages(msg);
    }
    else if (_myAsyncServer != nullptr)
    {
        _myAsyncServer->sendMessages(msg);
    }
}
