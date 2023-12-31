#include "AppGUI.h"

#include "Windows.h"
#include <codecvt>
#include <utility>

#include <iostream>
#include <fstream>

#include "wx/filedlg.h"
#include <wx/image.h>

wxIMPLEMENT_APP(AppGUI);

#ifdef _DEBUG
#define GREEN_INDICATOR "../../modules/fireJSON/images/green-ind.png"
#define RED_INDICATOR   "../../modules/fireJSON/images/red-ind.png"
#define ClEAR_INDICATOR   "../../modules/fireJSON/images/clear.xpm"
#else
#define GREEN_INDICATOR "images/green-ind.png"
#define RED_INDICATOR   "images/red-ind.png"
#define ClEAR_INDICATOR   "images/clear.xpm"
#endif

AppGUI::AppGUI():
    _fileName("*Untitled..."),
    _wsStatus(AsyncWebSocketServer::WsStatus::Unknown),
    _testMode(TestMode::Auto),
    _fileNameEntered(false),
    _fileSaveAllowed(false),
    _isWsUrlValid(false),
    _isWsPortValid(false),
    _wsMessageEnabled(false),
    _threadSafe(true)
{
    _jsonParser = std::make_shared<MyJSON>();
}

AppGUI::~AppGUI()
{

    _threadSafe = false;
    Sleep(100);

    _wsManager.reset();

    _thread.interrupt();

    if (_thread.joinable()) {
        _thread.join();
    }

    (void)killApp();
}

bool AppGUI::OnInit()
{
    if (!wxApp::OnInit())
    {
        OnExit();
        return false;
    }

    if (isAppAlreadyRunning()) {
        // The application is already running.
        MessageBoxA(nullptr, "Another instance of the application is already running.", "Erreur", MB_OK | MB_ICONERROR);
        OnExit();
        return false;
    }

    EVT_SIZE(AppGUI::OnSize)
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxXPMHandler);

    // Create the main frame window
    _frame       = new FrameMain(nullptr);
    _frame->_textEditor->Hide();
    _frame->_fileName->Hide();

    _frame->_textEditorPanel->Hide();
    _frame->_fireJsonPanel->Show();

    //_frame->_fireGauge->Hide();
    _frame->_fireGauge->Show(false);
    _frame->_fireGauge->SetRange(100);

    //_dialogAboutMePage = new AboutMePage(this, wxID_ANY, "About Me", wxDefaultPosition, wxSize(400, 300), wxDEFAULT_DIALOG_STYLE);

    //_logTextCtrl = new wxLogTextCtrl(_frame->_logTextCtrl);
    wxLog::SetActiveTarget(_logTextCtrl);
    wxLog::SetLogLevel(wxLOG_Trace);
#ifndef _DEBUG
    //_frame->_logTextCtrl->Hide();
#endif

    //menus
    _frame->_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        onMenuItemFile(event);
        });
    _frame->_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        onMenuItemAbout(event);
        });
    _frame->_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        onMenuItemEdit(event);
        });
    _frame->_menuFire->Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        onMenuItemFire(event);
        });

    _frame->_menuItemTestMode->Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        onMenuFireItemTestMode(event);
        });

    //text editor
    _frame->_textEditor->Bind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onTextModified, this);

    //bind ws Url and port
    _frame->_wsAddressUrl->Bind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onWsUrlTextChanged, this);
    _frame->_wsPortNumber->Bind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onWsPortTextChanged, this);


    //fire button
     _frame->_fireButton->Bind(wxEVT_BUTTON, [&](const wxCommandEvent&)
                              {

                                  adaptSize(AdaptType::FireGauge);
                                  if(!_filePath.empty())
                                  {
                                      std::filesystem::path path(_filePath);
                                      // Extract the directory path
                                      std::filesystem::path directoryPath = path.parent_path();
                                      // Convert it back to a string
                                      const std::string directoryPathStr = directoryPath.string();
                                      processFireButton(directoryPathStr);
                                  }
                                  else
                                  {
                                      //default folder
                                      processFireButton("JsonTest");
                                  }

                                  _wsMessageEnabled = true;
                              });

    //consol clear button
     _frame->_consolClearButton->SetBitmap(wxBitmap(wxImage(ClEAR_INDICATOR, wxBITMAP_TYPE_XPM)));
     _frame->_consolClearButton->Bind(wxEVT_BUTTON, [&](const wxCommandEvent&)
         {
             _frame->_consol->Clear();
         });


     //_frame->Bind(wxEVT_SIZE, &AppGUI::OnSize, this);
     //_frame->Bind(wxEVT_MAXIMIZE, &AppGUI::OnMaximize, this);
     //_frame->Bind(wxEVT_ICONIZE, &AppGUI::OnIconize, this);

    _frame->Show();
    return true;
}

int AppGUI::OnExit()
{
    return wxApp::OnExit();
}

void AppGUI::OnSize(wxSizeEvent& event)
{
   // _frame->_fireGauge->Show();
    
}

void AppGUI::OnMaximize(wxMaximizeEvent& event)
{
    //_frame->_fireGauge->Show();
}

void AppGUI::OnIconize(wxIconizeEvent& event)
{
    
}

void AppGUI::notifyMe(const std::string& msg)
{
    if (!msg.empty())
    {
        writeConsole("Message received: \n"
                     "-------------------- \n"
                     + msg);
    }

    if(_testMode == TestMode::Interactive && !_jsonFiles.empty())
    {
        _jsonFilesMapIndex++;
        if(_jsonFilesMapIndex < _jsonFiles.size())
        {
            interactiveMode(_jsonFilesMapIndex);
            return;
        }

        _jsonFilesMapIndex = 0;
        interactiveMode(_jsonFilesMapIndex);
    }
}

void AppGUI::doWork()
{
    //ws://127.0.0.1:5000/ws
    _wsManager = std::make_shared<WebSocketManager>(_wsUrl, _wsPort, [this](const std::string& msg) {
        notifyMe(msg);
        }, [this](const AsyncWebSocketServer::WsStatus status, const AsyncWebSocketServer::ClientInfo& info)
        {
            _clientsNumber = info.clientsCount;
            if(status == AsyncWebSocketServer::WsStatus::ClientConnected)
            {
                writeConsole("Client " + std::to_string(info.id) + " connected (clients number : " + std::to_string(info.clientsCount) + ")");
                _clientStatusKey = "client_" + std::to_string(info.id) + "_connected";
                _wsStatus = AsyncWebSocketServer::WsStatus::ClientConnected;
            }
            else if(status == AsyncWebSocketServer::WsStatus::ClientDisconnected)
            {
                writeConsole("Client " + std::to_string(info.id) + " disconnected (clients number : " + std::to_string(info.clientsCount) + ")");
                _clientStatusKey = "client_" + std::to_string(info.id) + "_disconnected";
                _wsStatus = AsyncWebSocketServer::WsStatus::ClientDisconnected;
            }
        });

    Sleep(1000);
    _wsManager->createAsyncWebSocketServer();
}

void AppGUI::onMessageDoWork()
{
    while(_threadSafe && _frame != nullptr)
    {
        if(_wsMessageEnabled)
        {
            _frame->_fireButton->Disable();
            //_frame->_fireGauge->Show();
            _frame->_fireGauge->Show(true);

            if(_testMode == TestMode::Auto)
            {
                autoMode();
                _frame->_fireButton->Enable();
                _frame->_fireGauge->SetValue(0);
                //_frame->_fireGauge->Hide();
                _frame->_fireGauge->Show(false);
            }
            else if(_testMode == TestMode::Interactive)
            {
                /*_frame->_fireGauge->Hide();
                _frame->_fireButton->Disable();
                _jsonFilesMapIndex = 0;
                interactiveMode(_jsonFilesMapIndex);*/
            }
            _wsMessageEnabled = false;
        }

        if(_previousClientStatusKey != _clientStatusKey)
        {
            _previousClientStatusKey = _clientStatusKey;
            if (_wsStatus == AsyncWebSocketServer::WsStatus::ClientConnected)
            {
                setWsIndicator(WsStatusIndicator::Connected);
                _frame->_wsStatusLabel->SetLabel("Client Connected...");
                Sleep(1000);
                _frame->_wsStatusLabel->SetLabel("");
            }
            else if (_wsStatus == AsyncWebSocketServer::WsStatus::ClientDisconnected && !_clientsNumber)
            {
                setWsIndicator(WsStatusIndicator::Disconnected);
                _frame->_wsStatusLabel->SetLabel("Not connected...");
            }
        }
        Sleep(1000);
    }
}

void AppGUI::writeConsole(const std::string& msg) const
{
    _frame->_consol->AppendText("\n" + msg + "\n" +
                                "**************************************************************");
    // Scroll to the end of the consol
    _frame->_consol->ShowPosition(_frame->_consol->GetLastPosition());
}

void AppGUI::autoMode()
{
    std::string contentFileUtf = "";
    std::string reply;

    const int totalIterations = static_cast<int>(_jsonFiles.size());
    int iterations = 0;

    for(unsigned int jsonFileMapIndex = 0; jsonFileMapIndex < _jsonFiles.size(); jsonFileMapIndex++)
    {
        for (const auto& pair : _jsonFiles[jsonFileMapIndex]) {
            if (_jsonParser->ifUtf8(pair.second))
            {
                _jsonParser->convertFileToUtf8(pair.second, contentFileUtf);
                _jsonParser->jsonToString(pair.second, reply, contentFileUtf);
                _wsManager->sendWsMessage(reply);
                // Calculate the progress and update the gauge
                const int progress = static_cast<int>((iterations + 1) * 100.0 / totalIterations);
                _frame->_fireGauge->SetValue(progress);
                iterations++;

                if (progress != 100)
                {
                    Sleep(2000);
                }
                else
                {
                    Sleep(500);
                }

            }
            else
            {
                writeConsole(pair.first + " is not a valid UTF-8 file");
                break;
            }
        }
    }
}

void AppGUI::interactiveMode(const unsigned int indexFile)
{

    const auto file = _jsonFiles[indexFile].cbegin()->second;
    if (_jsonParser->ifUtf8(file))
    {
        std::string contentFileUtf;
        std::string reply;
        _jsonParser->convertFileToUtf8(file, contentFileUtf);
        _jsonParser->jsonToString(file, reply, contentFileUtf);
        _wsManager->sendWsMessage(reply);
    }
}

void AppGUI::setWsIndicator(const WsStatusIndicator& status) const
{
    if(status == WsStatusIndicator::Connected)
    {
        _frame->_wsStateIndicator->SetBitmap(wxBitmap(wxImage(GREEN_INDICATOR, wxBITMAP_TYPE_PNG)));
    }
    else
    {
        _frame->_wsStateIndicator->SetBitmap(wxBitmap(wxImage(RED_INDICATOR, wxBITMAP_TYPE_PNG)));
    }
}

bool AppGUI::isAppAlreadyRunning()
{
    const HANDLE mutexHandle = CreateMutex(nullptr, TRUE, reinterpret_cast<LPCWSTR>("FireJsonMutex"));

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        std::cerr << "Another instance of the application is already running." << std::endl;
        CloseHandle(mutexHandle);
        return true;
    }

    return false;
}

void AppGUI::adaptSize(const AdaptType type) const
{
    int w, h;
    if(type == AdaptType::FirePanel)
    {
        _frame->_textEditorPanel->GetSize(&w, &h);
        _frame->_fireJsonPanel->SetSize(w, h);
    }
    else if(type == AdaptType::EditorPanel)
    {
        _frame->_fireJsonPanel->GetSize(&w, &h); 
        _frame->_textEditorPanel->SetSize(w, h);
    }
    else if(type == AdaptType::FireGauge)
    {

        //_frame->_fireButton->GetSize(&w, &h);
        
        /*_frame->_fireGauge->GetSize(&w, &h);
        //_frame->_fireGauge->SetSize(w, h);
        //_frame->_fireGauge->Refresh();*/
    }
}

void AppGUI::onMenuItemFile(const wxCommandEvent& event)
{
    if (event.GetId() == _frame->_menuItemFileNew->GetId())
    {
        onFileNewClicked();
    }
    else if (event.GetId() == _frame->_menuItemFileOpen->GetId())
    {
        onFileOpenClicked();
    }
    else if (event.GetId() == _frame->_menuItemFileSave->GetId())
    {
        onFileSaveClicked();
    }
    else if (event.GetId() == _frame->_menuItemFileExit->GetId())
    {
        onFileExitClicked();
    }
}

void AppGUI::onFileNewClicked()
{
    _fileNameEntered = false;
    _fileSaveAllowed = true;
    _frame->_textEditor->Clear();
    _frame->_fileName->SetLabel("*Untitled...");
    _frame->_textEditor->Show();
    _frame->_fileName->Show();

    _frame->_textEditorPanel->Show();
    _frame->_fireJsonPanel->Hide();

    adaptSize(AdaptType::EditorPanel);
}

void AppGUI::onFileOpenClicked()
{
    if (const wxString filePath = wxFileSelector("Open JSON File", "", "", "", "JSON Files (*.json)|*.json", wxFD_OPEN); !filePath.empty())
    {
        // Read the contents of the selected file
        wxFile file(filePath, wxFile::read);
        if (file.IsOpened())
        {
            _fileSaveAllowed = true;

            wxString content;
            file.ReadAll(&content);
            file.Close();

            _filePath = filePath;
            _fileName = wxFileName(filePath).GetFullName();
            _fileNameEntered = true;

            _frame->_textEditor->Clear();
            _frame->_textEditor->SetValue(content);
            _frame->_fileName->SetLabel(_fileName);
            _frame->_textEditor->Show();
            _frame->_fileName->Show();

            _previousText = content;

            _frame->_textEditorPanel->Show();
            _frame->_fireJsonPanel->Hide();
        }
    }

    adaptSize(AdaptType::EditorPanel);
}

void AppGUI::onFileSaveClicked()
{
    if(_fileSaveAllowed)
    {
        if (!_fileNameEntered)
        {
            const wxString filePath = wxFileSelector("Save JSON File", "", "", "", "JSON Files (*.json)|*.json", wxFD_SAVE);

            if (!filePath.empty())
            {

                // Get file name from full path
                _fileName = wxFileName(filePath).GetFullName();

                const wxString text = _frame->_textEditor->GetValue();
                if (wxFile file(filePath, wxFile::write); file.IsOpened())
                {
                    _fileNameEntered = true;
                    _filePath = filePath;
                    file.Write(text.c_str(), text.size());
                    file.Close();
                }
                _previousText = text;
            }

            _frame->_fileName->SetLabel(_fileName);
        }
        else
        {
            const wxString text = _frame->_textEditor->GetValue();
            if (wxFile file(_filePath, wxFile::write); file.IsOpened())
            {
                file.Write(text.c_str(), text.size());
                file.Close();
            }

            _frame->_fileName->SetLabel(_fileName);
            _previousText = text;
        }
    }
}

void AppGUI::onFileExitClicked()
{
    wxGetApp().ExitMainLoop();
}

void AppGUI::onTextModified(wxCommandEvent& event)
{
    const auto currentText = _frame->_textEditor->GetValue().ToStdString();

    if (currentText != _previousText)
    {
        // Content has been changed, add an asterisk to the file name
        if (!_filePath.empty())
        {
            _frame->_fileName->SetLabel("*" + _fileName);
        }

    }
    else
    {
        _frame->_fileName->SetLabel(_fileName);
    }
}

unsigned int AppGUI::killApp()
{

    // Get a handle from the process
    const HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, GetCurrentProcessId());

    if (hProcess == nullptr) {
        std::cerr << "Error opening process:" << GetLastError() << std::endl;
        return 1;
    }

    // Termine le processus
    if (!TerminateProcess(hProcess, 0)) {
        std::cerr << "Error terminating process: " << GetLastError() << std::endl;
        return 1;
    }
    // Close the process handle
    CloseHandle(hProcess);

    return 0;
}

void AppGUI::onMenuItemAbout(const wxCommandEvent& event)
{
    if (event.GetId() == _frame->_menuItemAbout->GetId()) {

        //const auto htmlFilePath = "about.html";
        //std::ifstream file(htmlFilePath);
        //if (file.is_open()) {
        //    std::stringstream buffer;
        //    buffer << file.rdbuf();
        //    const wxString htmlContent((buffer.str().c_str()), wxConvUTF8); // Convert to wxString with UTF-8 encoding
        //    _dialogAboutMePage->_htmlPageAbout->SetPage(htmlContent);
        //    file.close();
        //}
        //else {
        //    wxLogError("Failed to open HTML file: %s", htmlFilePath);
        //}

        // Create the about frame window
        _dialogAbout =  new DialogAbout(nullptr);

        _dialogAbout->_aboutOkButton->Bind(wxEVT_BUTTON, [&](const wxCommandEvent&)
            {
                _dialogAbout->Hide();
            });

        _dialogAbout->_aboutText->SetLabel("\nApplication designed to simplify real-time testing and data exchange "
            "between backend and frontend systems.\n"
            "\nDeveloped by: Omar Terro\n");
        _dialogAbout->Show();
    }
}

void AppGUI::onMenuItemEdit(const wxCommandEvent& event) const
{
    if (event.GetId() == _frame->_menuItemEditUndo->GetId()) {
        _frame->_textEditor->Undo();
        //_frame->_wsAddressUrl->Undo();
        //_frame->_wsPortNumber->Undo();
    }
    else if (event.GetId() == _frame->_menuItemEditCut->GetId()) {
        _frame->_textEditor->Cut();
        //_frame->_wsAddressUrl->Cut();
        //_frame->_wsPortNumber->Cut();
    }
    else if (event.GetId() == _frame->_menuItemEditCopy->GetId()) {
        _frame->_textEditor->Copy();
        //_frame->_wsAddressUrl->Copy();
        //_frame->_wsPortNumber->Copy();
    }
    else if (event.GetId() == _frame->_menuItemEditPaste->GetId()) {
        _frame->_textEditor->Paste();
        //_frame->_wsAddressUrl->Paste();
        //_frame->_wsPortNumber->Paste();
    }
    else if (event.GetId() == _frame->_menuItemEditDelete->GetId()) {
        // Delete selected text in the wxTextCtrl
        long from, to;
        _frame->_textEditor->GetSelection(&from, &to);
        _frame->_textEditor->Remove(from, to);

        /*_frame->_wsAddressUrl->GetSelection(&from, &to);
        _frame->_wsAddressUrl->Remove(from, to);

        _frame->_wsPortNumber->GetSelection(&from, &to);
        _frame->_wsPortNumber->Remove(from, to);*/
    }
    else if (event.GetId() == _frame->_menuItemEditSelectAll->GetId()) {
        // Select all the text in the wxTextCtrl
        _frame->_textEditor->SetSelection(0, _frame->_textEditor->GetLastPosition());
        //_frame->_wsAddressUrl->SetSelection(0, _frame->_wsAddressUrl->GetLastPosition());
        //_frame->_wsPortNumber->SetSelection(0, _frame->_wsPortNumber->GetLastPosition());
    }
}

void AppGUI::onMenuItemFire(const wxCommandEvent& event) const
{
    if (event.GetId() == _frame->_menuItemFire->GetId()) {
         adaptSize(AdaptType::FirePanel);
        _frame->_textEditorPanel->Hide();
        _frame->_fireJsonPanel->Show();
    }
}

void AppGUI::onMenuFireItemTestMode(const wxCommandEvent& event)
{
    if (event.GetId() == _frame->_menuItemAutoMode->GetId()) 
    {
        _frame->_fireButton->Enable();
        _testMode = TestMode::Auto;
    }
    else if (event.GetId() == _frame->_menuItemInteractiveMode->GetId())
    {
        _testMode = TestMode::Interactive;
        //_frame->_fireGauge->Hide();
        _frame->_fireGauge->Show(false);
        _frame->_fireButton->Disable();
        _jsonFilesMapIndex = 0;
        interactiveMode(_jsonFilesMapIndex);
    }
}

void AppGUI::onWsUrlTextChanged(wxCommandEvent& event)
{
    _frame->_wsAddressUrl->Unbind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onWsUrlTextChanged, this);

    wxString text = _frame->_wsAddressUrl->GetValue();
    wxString validText;

    for (wxString::const_iterator it = text.begin(); it != text.end(); ++it)
    {
        if (wxIsdigit(*it) || *it == '.')
        {
            validText += *it;
        }
    }

    if(validText.length() == 9)
    {
        _isWsUrlValid = true;
        _wsUrl = validText;
    }
    else
    {
        _isWsUrlValid = false;
    }
    _frame->_wsAddressUrl->SetValue(validText);
    _frame->_wsAddressUrl->Bind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onWsUrlTextChanged, this);
    fireState();
}

void AppGUI::onWsPortTextChanged(wxCommandEvent& event)
{
    _frame->_wsPortNumber->Unbind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onWsPortTextChanged, this);

    wxString text = _frame->_wsPortNumber->GetValue();
    wxString validText;

    for (wxString::const_iterator it = text.begin(); it != text.end(); ++it)
    {
        if (wxIsdigit(*it))
        {
            validText += *it;
        }
    }

    if (validText.length() > 3)
    {
        _isWsPortValid = true;
        _wsPort = validText;
    }
    else
    {
        _isWsPortValid = false;
    }

    _frame->_wsPortNumber->SetValue(validText);
    _frame->_wsPortNumber->Bind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onWsPortTextChanged, this);

    fireState();
}

void AppGUI::fireState()
{
    if(_isWsUrlValid && _isWsPortValid)
    {
        _thread = boost::thread(&AppGUI::doWork, this);
        _onMessagethread = boost::thread(&AppGUI::onMessageDoWork, this);
        _frame->_fireButton->Enable();
    }
    else
    {
        _frame->_fireButton->Disable();
    }
}

void AppGUI::processFireButton(const std::string& folderPath)
{
    // Check if the folder exists
    if (std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath)) {
        _jsonFiles.clear();
        // Iterate over the files in the folder
        int jsonFilesIndex = 0;
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            // Check if the file has a .json extension
            if (entry.path().extension() == ".json") {
                // Extract the filename without extension
                std::string filenameWithoutExtension = entry.path().stem().string();

                // Store the filename and full path in the map
                _jsonFiles[jsonFilesIndex][filenameWithoutExtension] = entry.path().string();
            }
            jsonFilesIndex++;
        }
    }
    else {
        writeConsole("Default folder does not exist.\nPlease add \"JsonTest\" default folder.\nOr click ctrl+O to choose another one.");
    }
}