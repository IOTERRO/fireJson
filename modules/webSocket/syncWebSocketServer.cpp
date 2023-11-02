#include <iostream>

#include "syncWebSocketServer.h"

#define ENABLE_SEND_MESSAGES  true
#define DISABLE_SEND_MESSAGES false

static bool MessagesState = ENABLE_SEND_MESSAGES;
static std::string ReplyMessages;


SyncWebSocketServer::SyncWebSocketServer(boost::asio::ip::address address, unsigned short port, std::string replyMessage) :
    _address(address),
    _port(port),
    _replyMessage(replyMessage)
{
    _replyMessage = "omar";
}

SyncWebSocketServer::~SyncWebSocketServer()
{
}

void SyncWebSocketServer::handleWebSocket(Tcp::socket s)
{
    websocket::stream<Tcp::socket> ws{ std::move(const_cast<Tcp::socket&>(s)) };

    // Set a decorator to change the Server of the handshake
    // no need to set. It ıs not necessary
    ws.set_option(websocket::stream_base::decorator(
        [](websocket::response_type& res)
        {
            res.set(http::field::server,
                std::string(BOOST_BEAST_VERSION_STRING) +
                " websocket-server-sync");
        }));

    // Accept the websocket handshake
    ws.accept();

    while (true)
    {
        try
        {
            boost::asio::streambuf buff;
            boost::asio::read_until(ws, buff, "\n");
            std::string message(boost::asio::buffers_begin(buff.data()), boost::asio::buffers_end(buff.data()));
            std::cout << "Received message: " << message << std::endl;
          
            beast::flat_buffer buffer;

            if (MessagesState == ENABLE_SEND_MESSAGES)
            {
                ws.write(boost::asio::buffer(ReplyMessages));
                MessagesState = DISABLE_SEND_MESSAGES;
            }

        }
        catch (beast::system_error const& se)
        {
            if (se.code() != websocket::error::closed)
            {
                std::cerr << "Error: " << se.code().message() << std::endl;
                break;
            }
        }
    }
}

void SyncWebSocketServer::createWebSocketServer()
{
        net::io_context ioc{1};

        Tcp::acceptor acceptor{ioc, {_address, _port}};
        for(;;)
        {

            Tcp::socket socket{ioc};

            acceptor.accept(socket);

            std::thread{ [s = std::move(socket), this] () {
            handleWebSocket(std::move(const_cast<Tcp::socket&>(s)));
        } }.detach();

        }
}

void SyncWebSocketServer::sendMessages(const std::string& msg)
{
    ReplyMessages = msg;
    MessagesState = ENABLE_SEND_MESSAGES;
}
