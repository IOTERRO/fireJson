#pragma once
#include "AppFrame.h"

#include <memory>
#include <boost/thread/thread.hpp>
#include <wx/app.h>
#include "webSocketManager.h"
#include "myJson.h"

class AppGUI final : public wxApp
{
public:
    AppGUI();
    ~AppGUI();

    bool OnInit() override;
    int OnExit() override;

private:

    void notifyMe(std::string msg);
    void doWork();
    AppFrame* _frame = nullptr;
    wxLogTextCtrl* _logTextCtrl = nullptr;
    std::shared_ptr<WebSocketManager> _wsManager;
    std::shared_ptr<MyJSON> _jsonParser;
    boost::thread _thread;
    int _paused;
    std::string _jsonMessage;
    bool _jsonActivated;
};
