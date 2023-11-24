///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "AppFrame.h"

///////////////////////////////////////////////////////////////////////////

FrameMain::FrameMain( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerMainFrame;
	bSizerMainFrame = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerFrameMain;
	bSizerFrameMain = new wxBoxSizer( wxVERTICAL );

	_textEditorPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	_textEditorPanel->Hide();

	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );

	_fileName = new wxStaticText( _textEditorPanel, wxID_ANY, _("new"), wxDefaultPosition, wxDefaultSize, 0 );
	_fileName->Wrap( -1 );
	bSizer27->Add( _fileName, 0, wxALL, 5 );

	_textEditor = new wxTextCtrl( _textEditorPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer27->Add( _textEditor, 1, wxALL|wxEXPAND, 0 );


	_textEditorPanel->SetSizer( bSizer27 );
	_textEditorPanel->Layout();
	bSizer27->Fit( _textEditorPanel );
	bSizerFrameMain->Add( _textEditorPanel, 1, wxEXPAND | wxALL, 0 );

	_fireJsonPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );

	m_staticText6 = new wxStaticText( _fireJsonPanel, wxID_ANY, _("FireJSON"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	m_staticText6->SetFont( wxFont( 18, wxFONTFAMILY_MODERN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, wxT("Courier New") ) );
	m_staticText6->SetForegroundColour( wxColour( 0, 0, 160 ) );

	bSizer24->Add( m_staticText6, 0, wxALIGN_CENTER|wxALL, 8 );


	bSizer22->Add( bSizer24, 0, wxEXPAND, 0 );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	_wsAddressLabel = new wxStaticText( _fireJsonPanel, wxID_ANY, _("Url:"), wxDefaultPosition, wxDefaultSize, 0 );
	_wsAddressLabel->Wrap( -1 );
	_wsAddressLabel->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );
	_wsAddressLabel->SetForegroundColour( wxColour( 7, 28, 120 ) );

	bSizer8->Add( _wsAddressLabel, 0, wxALL, 10 );

	_wsAddressUrl = new wxTextCtrl( _fireJsonPanel, wxID_ANY, _("127.0.0."), wxDefaultPosition, wxDefaultSize, 0 );
	_wsAddressUrl->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer8->Add( _wsAddressUrl, 0, wxALL, 7 );


	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );

	_wsPortLabel = new wxStaticText( _fireJsonPanel, wxID_ANY, _("Port:"), wxDefaultPosition, wxDefaultSize, 0 );
	_wsPortLabel->Wrap( -1 );
	_wsPortLabel->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );
	_wsPortLabel->SetForegroundColour( wxColour( 7, 28, 120 ) );

	bSizer8->Add( _wsPortLabel, 0, wxALL, 10 );

	_wsPortNumber = new wxTextCtrl( _fireJsonPanel, wxID_ANY, _("500"), wxDefaultPosition, wxDefaultSize, 0 );
	_wsPortNumber->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer8->Add( _wsPortNumber, 0, wxALL, 7 );


	bSizer7->Add( bSizer8, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 0, 0, 0 );

	_fireButton = new wxButton( _fireJsonPanel, wxID_ANY, _("Fire"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	_fireButton->SetFont( wxFont( 12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );
	_fireButton->SetForegroundColour( wxColour( 7, 28, 120 ) );
	_fireButton->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNSHADOW ) );
	_fireButton->Enable( false );

	gSizer1->Add( _fireButton, 0, wxALIGN_CENTER, 0 );

	_fireGauge = new wxGauge( _fireJsonPanel, wxID_ANY, 100, wxDefaultPosition, wxSize( 80,-1 ), wxGA_HORIZONTAL );
	_fireGauge->SetValue( 0 );
	gSizer1->Add( _fireGauge, 1, wxALIGN_CENTER|wxALL|wxBOTTOM, 20 );


	bSizer10->Add( gSizer1, 0, wxEXPAND, 0 );

	wxBoxSizer* bSizer151;
	bSizer151 = new wxBoxSizer( wxHORIZONTAL );

	_wsStateIndicator = new wxStaticBitmap( _fireJsonPanel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 48,48 ), 0 );
	bSizer151->Add( _wsStateIndicator, 0, wxALIGN_CENTER|wxALL, 5 );

	_wsStatusLabel = new wxStaticText( _fireJsonPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	_wsStatusLabel->Wrap( -1 );
	bSizer151->Add( _wsStatusLabel, 1, wxALIGN_CENTER|wxALL, 0 );


	bSizer10->Add( bSizer151, 1, wxALL, 5 );


	bSizer7->Add( bSizer10, 1, wxEXPAND, 0 );


	bSizer13->Add( bSizer7, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );


	bSizer15->Add( 0, 0, 0, wxALL, 4 );

	_consol = new wxTextCtrl( _fireJsonPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer15->Add( _consol, 1, wxALL|wxEXPAND, 0 );


	bSizer13->Add( bSizer15, 1, wxEXPAND, 5 );


	bSizer22->Add( bSizer13, 1, wxEXPAND, 5 );


	_fireJsonPanel->SetSizer( bSizer22 );
	_fireJsonPanel->Layout();
	bSizer22->Fit( _fireJsonPanel );
	bSizerFrameMain->Add( _fireJsonPanel, 1, wxEXPAND | wxALL, 0 );


	bSizerMainFrame->Add( bSizerFrameMain, 1, wxEXPAND, 0 );


	this->SetSizer( bSizerMainFrame );
	this->Layout();
	_menubarMain = new wxMenuBar( 0 );
	_menuFile = new wxMenu();
	_menuItemFileNew = new wxMenuItem( _menuFile, wxID_ANY, wxString( _("New") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	_menuFile->Append( _menuItemFileNew );

	_menuItemFileOpen = new wxMenuItem( _menuFile, wxID_ANY, wxString( _("Open") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	_menuFile->Append( _menuItemFileOpen );

	_menuItemFileSave = new wxMenuItem( _menuFile, wxID_ANY, wxString( _("Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	_menuFile->Append( _menuItemFileSave );

	_menuFile->AppendSeparator();

	_menuItemFileExit = new wxMenuItem( _menuFile, wxID_ANY, wxString( _("Exit") ) + wxT('\t') + wxT("Alt-F4"), wxEmptyString, wxITEM_NORMAL );
	_menuFile->Append( _menuItemFileExit );

	_menubarMain->Append( _menuFile, _("File") );

	_menuEdit = new wxMenu();
	_menuItemEditUndo = new wxMenuItem( _menuEdit, wxID_ANY, wxString( _("Undo") ) + wxT('\t') + wxT("Ctrl+Z"), wxEmptyString, wxITEM_NORMAL );
	_menuEdit->Append( _menuItemEditUndo );

	_menuEdit->AppendSeparator();

	_menuItemEditCut = new wxMenuItem( _menuEdit, wxID_ANY, wxString( _("Cut") ) + wxT('\t') + wxT("Ctrl+X"), wxEmptyString, wxITEM_NORMAL );
	_menuEdit->Append( _menuItemEditCut );

	_menuItemEditCopy = new wxMenuItem( _menuEdit, wxID_ANY, wxString( _("Copy") ) + wxT('\t') + wxT("Ctrl+C"), wxEmptyString, wxITEM_NORMAL );
	_menuEdit->Append( _menuItemEditCopy );

	_menuItemEditPaste = new wxMenuItem( _menuEdit, wxID_ANY, wxString( _("Paste") ) + wxT('\t') + wxT("Ctrl+V"), wxEmptyString, wxITEM_NORMAL );
	_menuEdit->Append( _menuItemEditPaste );

	_menuItemEditDelete = new wxMenuItem( _menuEdit, wxID_ANY, wxString( _("Delete") ) + wxT('\t') + wxT("Del"), wxEmptyString, wxITEM_NORMAL );
	_menuEdit->Append( _menuItemEditDelete );

	_menuEdit->AppendSeparator();

	_menuItemEditSelectAll = new wxMenuItem( _menuEdit, wxID_ANY, wxString( _("Select All") ) + wxT('\t') + wxT("Ctrl+A"), wxEmptyString, wxITEM_NORMAL );
	_menuEdit->Append( _menuItemEditSelectAll );

	_menubarMain->Append( _menuEdit, _("Edit") );

	_menuFire = new wxMenu();
	_menuItemFire = new wxMenuItem( _menuFire, wxID_ANY, wxString( _("FireJSON") ) + wxT('\t') + wxT("Ctrl+F"), wxEmptyString, wxITEM_NORMAL );
	_menuFire->Append( _menuItemFire );

	_menuItemTestMode = new wxMenu();
	wxMenuItem* _menuItemTestModeItem = new wxMenuItem( _menuFire, wxID_ANY, _("Test Mode"), wxEmptyString, wxITEM_NORMAL, _menuItemTestMode );
	#if (defined( __WXMSW__ ) || defined( __WXGTK__ ) || defined( __WXOSX__ ))
	_menuItemTestModeItem->SetBitmap( wxNullBitmap );
	#endif

	_menuItemAutoMode = new wxMenuItem( _menuItemTestMode, wxID_ANY, wxString( _("Auto") ) , wxEmptyString, wxITEM_RADIO );
	_menuItemTestMode->Append( _menuItemAutoMode );

	_menuItemInteractiveMode = new wxMenuItem( _menuItemTestMode, wxID_ANY, wxString( _("Interactive") ) , wxEmptyString, wxITEM_RADIO );
	_menuItemTestMode->Append( _menuItemInteractiveMode );

	_menuFire->Append( _menuItemTestModeItem );

	_menubarMain->Append( _menuFire, _("Fire") );

	_menuHelp = new wxMenu();
	_menuItemAbout = new wxMenuItem( _menuHelp, wxID_ANY, wxString( _("About...") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	_menuHelp->Append( _menuItemAbout );

	_menubarMain->Append( _menuHelp, _("Help") );

	this->SetMenuBar( _menubarMain );


	this->Centre( wxBOTH );

	// Connect Events
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemFileNewOnMenuSelection ), this, _menuItemFileNew->GetId());
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemFileOpenOnMenuSelection ), this, _menuItemFileOpen->GetId());
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemFileSaveOnMenuSelection ), this, _menuItemFileSave->GetId());
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemFileExitOnMenuSelection ), this, _menuItemFileExit->GetId());
	_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemEditUndoOnMenuSelection ), this, _menuItemEditUndo->GetId());
	_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemEditCutOnMenuSelection ), this, _menuItemEditCut->GetId());
	_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemEditCopyOnMenuSelection ), this, _menuItemEditCopy->GetId());
	_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemEditPasteOnMenuSelection ), this, _menuItemEditPaste->GetId());
	_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemEditDeleteOnMenuSelection ), this, _menuItemEditDelete->GetId());
	_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemEditSelectAllOnMenuSelection ), this, _menuItemEditSelectAll->GetId());
	_menuFire->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemFireOnMenuSelection ), this, _menuItemFire->GetId());
	_menuItemTestMode->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemAutoModeOnMenuSelection ), this, _menuItemAutoMode->GetId());
	_menuItemTestMode->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemInteractiveModeOnMenuSelection ), this, _menuItemInteractiveMode->GetId());
	_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FrameMain::_menuItemAboutOnMenuSelection ), this, _menuItemAbout->GetId());
}

FrameMain::~FrameMain()
{
	// Disconnect Events

}

DialogAbout::DialogAbout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerAboutFrame;
	bSizerAboutFrame = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerFrameAbout;
	bSizerFrameAbout = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	_aboutText = new wxStaticText( this, wxID_ANY, _("fireJSon App"), wxDefaultPosition, wxDefaultSize, 0 );
	_aboutText->Wrap( -1 );
	_aboutText->SetFont( wxFont( 10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	bSizer5->Add( _aboutText, 1, wxALL|wxEXPAND, 5 );

	_aboutOkButton = new wxButton( this, wxID_ANY, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( _aboutOkButton, 0, wxALIGN_CENTER|wxALL, 1 );


	bSizerFrameAbout->Add( bSizer5, 1, wxEXPAND, 5 );


	bSizerAboutFrame->Add( bSizerFrameAbout, 1, wxEXPAND, 5 );


	this->SetSizer( bSizerAboutFrame );
	this->Layout();

	this->Centre( wxBOTH );
}

DialogAbout::~DialogAbout()
{
}

AboutMePage::AboutMePage( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );

	_htmlPageAbout = new wxHtmlWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
	bSizer20->Add( _htmlPageAbout, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer20 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	_htmlPageAbout->Connect( wxEVT_CHAR, wxKeyEventHandler( AboutMePage::_htmlPageAboutOnChar ), NULL, this );
	_htmlPageAbout->Connect( wxEVT_COMMAND_HTML_CELL_CLICKED, wxHtmlCellEventHandler( AboutMePage::_htmlPageAboutOnHtmlCellClicked ), NULL, this );
	_htmlPageAbout->Connect( wxEVT_COMMAND_HTML_CELL_HOVER, wxHtmlCellEventHandler( AboutMePage::_htmlPageAboutOnHtmlCellHover ), NULL, this );
	_htmlPageAbout->Connect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutMePage::_htmlPageAboutOnHtmlLinkClicked ), NULL, this );
}

AboutMePage::~AboutMePage()
{
	// Disconnect Events
	_htmlPageAbout->Disconnect( wxEVT_CHAR, wxKeyEventHandler( AboutMePage::_htmlPageAboutOnChar ), NULL, this );
	_htmlPageAbout->Disconnect( wxEVT_COMMAND_HTML_CELL_CLICKED, wxHtmlCellEventHandler( AboutMePage::_htmlPageAboutOnHtmlCellClicked ), NULL, this );
	_htmlPageAbout->Disconnect( wxEVT_COMMAND_HTML_CELL_HOVER, wxHtmlCellEventHandler( AboutMePage::_htmlPageAboutOnHtmlCellHover ), NULL, this );
	_htmlPageAbout->Disconnect( wxEVT_COMMAND_HTML_LINK_CLICKED, wxHtmlLinkEventHandler( AboutMePage::_htmlPageAboutOnHtmlLinkClicked ), NULL, this );

}
