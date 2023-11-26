#pragma once

#include "asyncWebSocketServer.h"
#include "export.h"
#include "syncWebSocketServer.h"
#include "webSocketClient.h"

class MYSOCKET_API WebSocketManager
{
public:
	WebSocketManager(std::string host, const std::string& port, std::function<void(std::string)> onMessage, std::function<void(AsyncWebSocketServer::WsStatus, AsyncWebSocketServer::ClientInfo)> onWsStatus);
	WebSocketManager(std::string host, const std::string& port);
    ~WebSocketManager();

	//create ws server
	void createSyncWebSocketServer();
	void createAsyncWebSocketServer();

	//create ws client
	int  webSocketClient();

	//send messages on ws
	void sendWsMessage(const std::string& msg) const;
private:
	void subscribeForNewMessage(const std::string& msg) const;
	void wsStatus(const AsyncWebSocketServer::WsStatus status, AsyncWebSocketServer::ClientInfo);
	std::function<void(std::string)> _onMessage;
	std::function<void(AsyncWebSocketServer::WsStatus, AsyncWebSocketServer::ClientInfo)> _onWsStatus;
	std::shared_ptr<SyncWebSocketServer> _mySyncServer;
	std::shared_ptr<AsyncWebSocketServer> _myAsyncServer;
	std::shared_ptr<MyWebSocketClient> _myClient;
	std::string _host;
	const unsigned short _port;
	std::string _serverReplyMessage;
	std::string _clientSendMessage;
	std::thread _ioContextThread;
	std::thread _ioThread; // Thread pour ioContext.run(
    boost::asio::io_context _ioContext;
};
