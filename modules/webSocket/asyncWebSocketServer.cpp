#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <utility>
#include <random>

#include "asyncWebSocketServer.h"

#include <iomanip>

#define ENABLE_SEND_MESSAGES  true
#define DISABLE_SEND_MESSAGES false

static bool MessagesState = ENABLE_SEND_MESSAGES;
static std::string ReplyMessages;


unsigned AsyncWebSocketServer::getRandomUnsignedInt()
{
    // Using a pseudo-random number generator
    std::random_device rd; // Used to obtain a random seed from the system
    std::mt19937 gen(rd()); // Mersenne Twister 32-bit generator
    std::uniform_int_distribution<unsigned int> dis;

    return dis(gen); // Returns a random value of type unsigned int
}
WebSocketSession::WebSocketSession(Tcp::socket socket, AsyncWebSocketServer::ClientInfo clientInfo, std::function<void(std::string)> onMessage,
                                   std::function<void(AsyncWebSocketServer::WsStatus, AsyncWebSocketServer::ClientInfo)> onWsStatus):
    _socket(std::move(socket)),
    _clientInfo(std::move(clientInfo)),
    _onMessage(std::move(onMessage)),
    _onWsStatus(std::move(onWsStatus))

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
    try
    {
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
    }
    catch (const boost::system::system_error& e)
    {
        if (e.code() == websocket::error::closed)
        {
            // WebSocket connection closed by the client
            _onWsStatus(AsyncWebSocketServer::WsStatus::ClientDisconnected, _clientInfo);
            //const boost::asio::ip::tcp::endpoint remoteEp = _socket.remote_endpoint();
            //const std::string clienIp = remoteEp.address().to_string();
            std::cout << "Client " << _clientInfo.ip << " disconnected" << std::endl;
        }
        else
        {
            // Handle other WebSocket-related errors
            std::cerr << "WebSocket error: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        // Handle other exceptions
        std::cerr << "Exception in WebSocket session: " << e.what() << std::endl;
    }

    // Close WebSocket connection (if not closed already)
    ws.close(websocket::close_code::normal);
}


void WebSocketSession::readMessage(Tcp::socket socket)
{
    std::lock_guard<std::mutex> lock(_mutex);
    // Receive WebSocket message
    boost::beast::multi_buffer buffer;

    //ws.read(buffer);
}



AsyncWebSocketServer::AsyncWebSocketServer(std::function<void(std::string)> onMessage, boost::asio::io_context& io_context, const std::string& address, const short port, std::function<void(WsStatus, ClientInfo)> onWsStatus) :
    _acceptor(io_context, Tcp::endpoint(boost::asio::ip::make_address(address), port)),
    _onMessage(std::move(onMessage)),
    _onWsStatus(std::move(onWsStatus))
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

std::string AsyncWebSocketServer::getMacAddress()
{
    // Use boost::asio to retrieve the MAC address of the first network interface
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end; // End marker.

    while (iter != end)
    {
        boost::asio::ip::tcp::endpoint endpoint = *iter++;
        std::string addressString = endpoint.address().to_string();

        // Remove any characters that are not hexadecimal digits
        boost::erase_all(addressString, ":");
        boost::erase_all(addressString, ".");

        // If the resulting string is longer than 12 characters, trim it
        if (addressString.length() > 12)
        {
            addressString = addressString.substr(0, 12);
        }

        // Convert the string to uppercase
        boost::to_upper(addressString);

        // Format the MAC address with colons
        std::ostringstream macAddress;
        for (std::size_t i = 0; i < addressString.length(); i += 2)
        {
            macAddress << addressString.substr(i, 2);
            if (i < addressString.length() - 2)
            {
                macAddress << ":";
            }
        }

        return macAddress.str();
    }

    return ""; // Return an empty string if MAC address is not found
}

unsigned AsyncWebSocketServer::getClientId()
{
    // Use the MAC address as a fixed identifier for each client
    std::string macAddress = getMacAddress();
    std::hash<std::string> hasher;
    return hasher(macAddress);
}


void AsyncWebSocketServer::startAccept()
{
    _acceptor.async_accept([this](boost::system::error_code ec, Tcp::socket socket)
        {
            if (!ec)
            {

                Sleep(500);

                // Generate a random unsigned int
                unsigned int id = getRandomUnsignedInt();
                std::string clientKey = "client_" + std::to_string(id);
                // Keep generating a new id until it is unique
                while (_clientsConnectionMap.find(clientKey) != _clientsConnectionMap.end()) {
                    id = getRandomUnsignedInt();
                    clientKey = "client_" + std::to_string(id);
                }

                // Add the new id to the map
                _clientsConnectionMap[clientKey] = id;

                const boost::asio::ip::tcp::endpoint remoteEp = socket.remote_endpoint();
                const std::string clientIp = "";//remoteEp.address().to_string();
                ClientInfo clientInfo;
                clientInfo.ip = clientIp;
                clientInfo.id = _clientsConnectionMap.at(clientKey);
                clientInfo.clientsCount = _clientsConnectionMap.size();

                _onWsStatus(WsStatus::ClientConnected, clientInfo);

                // Start a new thread to handle the WebSocket session
                boost::thread{ [&, s = std::move(socket), this] ()
                {
                    handleClients(std::move(const_cast<Tcp::socket&>(s)), clientInfo);
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

void AsyncWebSocketServer::handleClients(Tcp::socket socket, const ClientInfo& clientInfo)
{
    WebSocketSession session(std::move(socket), clientInfo, [this](const std::string& msg) {
        subscribeForNewMessage(msg);
        }, [&](const WsStatus status, ClientInfo& info)
        {
            if(status == WsStatus::ClientDisconnected)
            {
                removeClient(info.id);
                info.clientsCount = _clientsConnectionMap.size();
                _onWsStatus(WsStatus::ClientDisconnected, info);
            }
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

void AsyncWebSocketServer::removeClient(const unsigned id)
{
    // Check if the key exists before attempting to remove
    const auto keyToRemove = "client_" + std::to_string(id);
    const auto it = _clientsConnectionMap.find(keyToRemove);
    if (it != _clientsConnectionMap.end()) {
        // Key found, erase it
        _clientsConnectionMap.erase(it);
    }
    else {
        // TODO Key not found, handle accordingly
    }
}


void AsyncWebSocketServer::sendMessages(const std::string& msg)
{
    ReplyMessages = msg;
    MessagesState = ENABLE_SEND_MESSAGES;
}
