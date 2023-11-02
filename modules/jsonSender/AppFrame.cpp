///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "AppFrame.h"

///////////////////////////////////////////////////////////////////////////

AppFrame::AppFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );

	_wsPort = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( _wsPort, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	_fireJson = new wxButton( this, wxID_ANY, _("fire JSON"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer29->Add( _fireJson, 1, wxALL|wxEXPAND, 1 );


	bSizer43->Add( bSizer29, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	_logger = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxPoint( -1,-1 ), wxSize( -1,-1 ), 0 );
	bSizer11->Add( _logger, 1, wxALL|wxEXPAND, 1 );


	bSizer43->Add( bSizer11, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer43 );
	this->Layout();

	this->Centre( wxBOTH );
}

AppFrame::~AppFrame()
{
}
