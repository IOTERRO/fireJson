#include "webSocketClient.h"


MyWebSocketClient::MyWebSocketClient(std::string host, unsigned short port) :
    _host(host),
    _port(port)
{

}


void MyWebSocketClient::wsClient(Tcp::socket& socket, net::io_context& ioc, websocket::stream<Tcp::socket>& ws, WsState stateWs, std::string* message)
{
    try {
        switch (stateWs)
        {
        case WsState::Connect:
        {
            Tcp::resolver resolver(ioc);
            auto const results = resolver.resolve(_host, std::to_string(_port));
            net::connect(socket, results);
            std::cout << "Connect to the server" << std::endl;
            break;
        }
        case WsState::HandShake:
        {
            ws.handshake(_host, "/");
            std::cout << "WebSocket handshake complete" << std::endl;
            break;
        }
        case WsState::SendMessage:
        {   
            sendMessage(ws, *message);
            std::cout << "Send message to the server: " << std::endl;
            break;
        }
        case WsState::ReceiveMessage:
        {
            *message = receiveMessage(ws);
            std::cout << "Received message from server: " << *message << std::endl;
            break;
        }
        case WsState::Close:
        {
            close(ws);
            std::cout << "Close connection " << std::endl;
            break;
        }

        default:
            break;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void MyWebSocketClient::sendMessage(websocket::stream<Tcp::socket>& ws, const std::string message)
{
    try
    {
        //net::write(ws, net::buffer(message));
        ws.write(net::buffer(message));
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error sending message: " << ex.what() << std::endl;
        throw;
    }
}

std::string MyWebSocketClient::receiveMessage(websocket::stream<Tcp::socket>& ws)
{
    try
    {
        beast::flat_buffer buffer;
        ws.read(buffer);
        return beast::buffers_to_string(buffer.data());
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error receiving message: " << ex.what() << std::endl;
        throw;
    }
}

void MyWebSocketClient::close(websocket::stream<Tcp::socket>& ws)
{
    try
    {
        ws.close(websocket::close_code::normal);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error closing WebSocket connection: " << ex.what() << std::endl;
        throw;
    }
}