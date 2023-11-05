#include <iostream>
#include <boost/thread/thread.hpp>
#include <utility>

#include "asyncWebSocketServer.h"

#define ENABLE_SEND_MESSAGES  true
#define DISABLE_SEND_MESSAGES false

static bool MessagesState = ENABLE_SEND_MESSAGES;
static std::string ReplyMessages;

WebSocketSession::WebSocketSession(Tcp::socket socket, std::function<void(std::string)> onMessage):
    _socket(std::move(socket)),
    _onMessage(std::move(onMessage))
{
}


void WebSocketSession::start()
{
    // Perform WebSocket handshake
    websocket::stream<Tcp::socket&> ws{ _socket };
    ws.set_option(websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
    ws.set_option(websocket::stream_base::decorator([](websocket::response_type& res)
        {
            res.set(http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-sync");
        }));
    ws.accept();

    const boost::asio::ip::tcp::endpoint remoteEp = _socket.remote_endpoint();
    const std::string clienIp = remoteEp.address().to_string();
    std::cout << "Client " << clienIp << " connected" << std::endl;

    // Receive and process WebSocket messages
    while (true)
    {
        // Receive WebSocket message
        beast::multi_buffer buffer;

        //ws.read(buffer);
        if (ws.next_layer().available() > 0) {
            ws.read(buffer);
            const std::string message = boost::beast::buffers_to_string(buffer.data());
            _onMessage(message);
            //std::cout << "Receive WebSocket message: " << message << std::endl;
        }
        // Echo WebSocket message back to client
        ws.text(true);
        if (MessagesState == ENABLE_SEND_MESSAGES)
        {
            ws.write(boost::asio::buffer(ReplyMessages));
            MessagesState = DISABLE_SEND_MESSAGES;
        }
    }

    // Close WebSocket connection
    //ws.close(websocket::close_code::normal);
}


void WebSocketSession::readMessage(Tcp::socket socket)
{
    std::lock_guard<std::mutex> lock(_mutex);
    // Receive WebSocket message
    boost::beast::multi_buffer buffer;

    //ws.read(buffer);
}



AsyncWebSocketServer::AsyncWebSocketServer(std::function<void(std::string)> onMessage, boost::asio::io_context& io_context, const std::string& address, const short port) :
    _acceptor(io_context, Tcp::endpoint(boost::asio::ip::make_address(address), port)),
    _onMessage(std::move(onMessage))
{
    std::cout << "Server started on port " << port << std::endl;
    startAccept();
}

AsyncWebSocketServer::~AsyncWebSocketServer()
{
    _acceptor.close();
    _acceptor.release();

    _ioContext.stop();
    _ioContext.reset();


}

void AsyncWebSocketServer::startAccept()
{
    _acceptor.async_accept([this](boost::system::error_code ec, Tcp::socket socket)
        {
            if (!ec)
            {
                // Start a new thread to handle the WebSocket session
                boost::thread{ [s = std::move(socket), this] ()
                {
                    handleClients(std::move(const_cast<Tcp::socket&>(s)));
                } }.detach();

                // Continue accepting new connections
                startAccept();
            }
            else
            {
                //[OT]: TODO  Handle the error (e.g., log it)
            }
        });
}

void AsyncWebSocketServer::handleClients(Tcp::socket socket) const
{
    WebSocketSession session(std::move(socket), [this](const std::string& msg) {
        subscribeForNewMessage(msg);
        });

    try
    {
        session.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception in WebSocket session: " << e.what() << std::endl;
    }
}


void AsyncWebSocketServer::subscribeForNewMessage(const std::string& msg) const
{
    _onMessage(msg);
}


void AsyncWebSocketServer::sendMessages(const std::string& msg)
{
    ReplyMessages = msg;
    MessagesState = ENABLE_SEND_MESSAGES;
}