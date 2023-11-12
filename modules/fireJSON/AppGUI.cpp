#include "AppGUI.h"

#include "Windows.h"
#include <codecvt>
#include <utility>

#include <iostream>
#include <fstream>

#include "wx/filedlg.h"

wxIMPLEMENT_APP(AppGUI);

AppGUI::AppGUI():
    _fileName("*Untitled..."),
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

    // Attendez que le thread se termine
    if (_thread.joinable()) {
        _thread.join();
    }
}

bool AppGUI::OnInit()
{
    if (!wxApp::OnInit())
    {
        OnExit();
        return false;
    }

    // Create the main frame window
    _frame       = new FrameMain(nullptr);
    _frame->_textEditor->Hide();
    _frame->_fileName->Hide();

    _frame->_textEditorPanel->Hide();
    _frame->_fireJsonPanel->Show();

    _frame->_fireGauge->SetRange(100);
    _frame->_fireGauge->Hide();

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

    //text editor
    _frame->_textEditor->Bind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onTextModified, this);

    //bind ws Url and port
    _frame->_wsAddressUrl->Bind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onWsUrlTextChanged, this);
    _frame->_wsPortNumber->Bind(wxEVT_COMMAND_TEXT_UPDATED, &AppGUI::onWsPortTextChanged, this);


    //fire button
     _frame->_fireButton->Bind(wxEVT_BUTTON, [&](const wxCommandEvent&)
                              {

                                  _frame->_consol->SetValue("");
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
                                      processFireButton("singleTest");
                                  }

                                  _wsMessageEnabled = true;
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
   _frame->_consol->SetValue(msg);
}

void AppGUI::doWork()
{
    //ws://127.0.0.1:5000/ws
    _wsManager = std::make_shared<WebSocketManager>(_wsUrl, _wsPort, [this](std::string msg) {
        notifyMe(std::move(msg));
        });
    _wsManager->createAsyncWebSocketServer();
}

void AppGUI::onMessageDoWork()
{
    while(_threadSafe && _frame != nullptr)
    {
        if(_wsMessageEnabled)
        {
            _frame->_fireButton->Disable();
            _frame->_fireGauge->Show();
            std::string contentFileUtf = "";
            std::string reply;

            const int totalIterations = static_cast<int>(_jsonFiles.size());
            int iterations = 0;
            for (const auto& pair : _jsonFiles) {
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
                    _frame->_consol->SetValue(pair.first + " is not a valid UTF-8 file");
                    break;
                }
            }
            _wsMessageEnabled = false;
        }
        _frame->_fireButton->Enable();
        _frame->_fireGauge->SetValue(0);
        _frame->_fireGauge->Hide();
        Sleep(1000);
    }
}

void AppGUI::adaptSize(const PanelType type) const
{
    int w, h;
    if(type == PanelType::Fire)
    {
        _frame->_textEditorPanel->GetSize(&w, &h);
        _frame->_fireJsonPanel->SetSize(w, h);
    }
    else if(type == PanelType::Editor)
    {
        _frame->_fireJsonPanel->GetSize(&w, &h); 
        _frame->_textEditorPanel->SetSize(w, h);
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

    adaptSize(PanelType::Editor);

}

void AppGUI::onFileOpenClicked()
{
    adaptSize(PanelType::Editor);
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
                const wxFileName file(filePath);
                _fileName = file.GetFullName();

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

void AppGUI::onMenuItemAbout(const wxCommandEvent& event)
{
    const int menuItemId = event.GetId();

    if (menuItemId == _frame->_menuItemAbout->GetId()) {

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

void AppGUI::onMenuItemEdit(const wxCommandEvent& event)
{
    if (event.GetId() == _frame->_menuItemEditUndo->GetId()) {
        _frame->_textEditor->Undo();
    }
    else if (event.GetId() == _frame->_menuItemEditCut->GetId()) {
        _frame->_textEditor->Cut();
    }
    else if (event.GetId() == _frame->_menuItemEditCopy->GetId()) {
        _frame->_textEditor->Copy();
    }
    else if (event.GetId() == _frame->_menuItemEditPaste->GetId()) {
        _frame->_textEditor->Paste();
    }
    else if (event.GetId() == _frame->_menuItemEditDelete->GetId()) {
        // Delete selected text in the wxTextCtrl
        long from, to;
        _frame->_textEditor->GetSelection(&from, &to);
        _frame->_textEditor->Remove(from, to);
    }
    else if (event.GetId() == _frame->_menuItemEditSelectAll->GetId()) {
        // Select all the text in the wxTextCtrl
        _frame->_textEditor->SetSelection(0, _frame->_textEditor->GetLastPosition());
    }
}

void AppGUI::onMenuItemFire(const wxCommandEvent& event)
{
    if (event.GetId() == _frame->_menuItemFire->GetId()) {
         adaptSize(PanelType::Fire);
        _frame->_textEditorPanel->Hide();
        _frame->_fireJsonPanel->Show();
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
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            // Check if the file has a .json extension
            if (entry.path().extension() == ".json") {
                // Extract the filename without extension
                std::string filenameWithoutExtension = entry.path().stem().string();

                // Store the filename and full path in the map
                _jsonFiles[filenameWithoutExtension] = entry.path().string();
            }
        }
    }
    else {
        std::cout << "Default folder does not exist or is not a directory." << std::endl;
    }
}