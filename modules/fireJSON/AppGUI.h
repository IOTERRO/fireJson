#pragma once
#include "AppFrame.h"

#include <memory>
#include <boost/thread/thread.hpp>
#include <wx/app.h>
#include "webSocketManager.h"
#include "myJson.h"

class AppGUI final : public wxApp, public wxWindow, public wxEvtHandler
{
public:
    AppGUI();
    ~AppGUI() override;

    bool OnInit() override;
    int OnExit() override;
    void OnSize(wxSizeEvent& event);
    void OnMaximize(wxMaximizeEvent& event);
    void OnFullScreen(wxFullScreenEvent& event);
    void OnIconize(wxIconizeEvent& event);

private:

    void notifyMe(const std::string& msg);
    void doWork();
    void onMessageDoWork();

    void autoMode();
    void interactiveMode(const unsigned int indexFile);

    enum class WsStatusIndicator
    {
        Connected,
        Disconnected
    };
    void setWsIndicator(const WsStatusIndicator& status) const;

    bool isAppAlreadyRunning();

    //Adapt the size of the panels
    enum class AdaptType
    {
        FirePanel,
        EditorPanel,
        FireGauge
    };
    void adaptSize(AdaptType type) const;

    //File menu
    void onMenuItemFile(const wxCommandEvent& event);
    void onFileNewClicked();
    void onFileOpenClicked();
    void onFileSaveClicked();
    void onFileExitClicked();

    //About menu
    void onMenuItemAbout(const wxCommandEvent& event);
    //Edit menu
    void onMenuItemEdit(const wxCommandEvent& event) const;
    //Fire menu
    void onMenuItemFire(const wxCommandEvent& event) const;
    void onMenuFireItemTestMode(const wxCommandEvent& event);

    //Ws url
    void onWsUrlTextChanged(wxCommandEvent& event);
    void onWsPortTextChanged(wxCommandEvent& event);

    //Fire button
    void fireState();
    void processFireButton(const std::string& folderPath);

    //Triggered text editing on editor
    void onTextModified(wxCommandEvent& event);

    //terminate app
    unsigned int killApp();

    FrameMain* _frame = nullptr;
    DialogAbout* _dialogAbout = nullptr;
    //AboutMePage* _dialogAboutMePage = nullptr;

    wxLogTextCtrl* _logTextCtrl = nullptr;
    std::shared_ptr<WebSocketManager> _wsManager;
    std::shared_ptr<MyJSON> _jsonParser;
    boost::thread _thread;
    boost::thread _onMessagethread;
    std::string _jsonMessage;
    std::string _filePath;
    std::string _fileName;
    std::string _previousText;

    //Ws
    std::string _wsUrl;
    std::string _wsPort;
    std::string _wsMessage;
    AsyncWebSocketServer::WsStatus _wsStatus;
    AsyncWebSocketServer::WsStatus _previousWsStatus;

    //JSON files map
    std::map<unsigned int, std::map<std::string, std::string>>_jsonFiles;// A map to store the JSON file names
    unsigned int _jsonFilesMapIndex{};

    //Test mode
    enum class TestMode
    {
        Auto,
        Interactive
    };
    TestMode _testMode;

    //File booleans
    bool _fileNameEntered;
    bool _fileSaveAllowed;

    //Ws booleans
    bool _isWsUrlValid;
    bool _isWsPortValid;
    bool _wsMessageEnabled;

    //Thread safe
    bool _threadSafe;
};
