#include "AppGUI.h"

#include "Windows.h"
#include <locale>
#include <codecvt>
#include <utility>

wxIMPLEMENT_APP(AppGUI);

AppGUI::AppGUI() :
    _paused(false),
    _jsonActivated(false)
{
    _thread = boost::thread(&AppGUI::doWork, this);
    _jsonParser = std::make_shared<MyJSON>();
}

AppGUI::~AppGUI()
{
    if (_thread.timed_join(boost::posix_time::seconds(3)))
        return;

    _thread.interrupt();
    _thread.timed_join(boost::posix_time::seconds(3));
}

bool AppGUI::OnInit()
{
    if (!wxApp::OnInit())
    {
        OnExit();
        return false;
    }

    // Create the main frame window
    _frame       = new AppFrame(nullptr);
    //_logTextCtrl = new wxLogTextCtrl(_frame->_logTextCtrl);
    wxLog::SetActiveTarget(_logTextCtrl);
    wxLog::SetLogLevel(wxLOG_Trace);
#ifndef _DEBUG
    //_frame->_logTextCtrl->Hide();
#endif

     _frame->_fireJson->Bind(wxEVT_BUTTON, [&](const wxCommandEvent&)
                              {
                                  _paused = false;
                                  std::string reply;
                                  _jsonParser->jsonToString("testJSON.json", reply);
                                  std::cout << reply << std::endl;
                                  _wsManager->sendWsMessage(reply);
                                  //_jsonMessage = reply;
                                  //_jsonActivated = true;
                              });

    _frame->Show();
    return true;
}

int AppGUI::OnExit()
{
    return wxApp::OnExit();
}

void AppGUI::notifyMe(std::string msg)
{
    std::cout << "msg:" << msg <<std::endl;
    _frame->_logger->SetValue(msg);
}

void AppGUI::doWork()
{
    //ws://127.0.0.1:5000/ws
    std::string host = "127.0.0.1";
    std::string port = "5000";

    _wsManager = std::make_shared<WebSocketManager>(host, port, [this](std::string msg) {
        notifyMe(std::move(msg));
        });
    _wsManager->createAsyncWebSocketServer();

   /* while (true)
    {
        if(_jsonActivated && !_jsonMessage.empty())
        {
            _wsManager->sendWsMessage(_jsonMessage);
        }
    }*/
}
