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
    ~AppGUI() override;

    bool OnInit() override;
    int OnExit() override;

private:

    void notifyMe(std::string msg);
    void doWork();
    void onMessageDoWork();

    //Adapt the size of the panels
    enum class PanelType
    {
        Fire,
        Editor
    };
    void adaptSize(PanelType type) const;

    //File menu
    void onMenuItemFile(const wxCommandEvent& event);
    void onFileNewClicked();
    void onFileOpenClicked();
    void onFileSaveClicked();
    void onFileExitClicked();

    //About menu
    void onMenuItemAbout(const wxCommandEvent& event);
    //Edit menu
    void onMenuItemEdit(const wxCommandEvent& event);
    //Fire menu
    void onMenuItemFire(const wxCommandEvent& event);

    //Ws url
    void onWsUrlTextChanged(wxCommandEvent& event);
    void onWsPortTextChanged(wxCommandEvent& event);

    //Fire button
    void fireState();
    void processFireButton(const std::string& folderPath);

    //Triggered text editing on editor
    void onTextModified(wxCommandEvent& event);

    FrameMain* _frame = nullptr;
    DialogAbout* _dialogAbout = nullptr;

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
    std::map<std::string, std::string> _jsonFiles;// A map to store the JSON file names

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
