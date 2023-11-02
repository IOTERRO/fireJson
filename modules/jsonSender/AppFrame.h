///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AppFrame
///////////////////////////////////////////////////////////////////////////////
class AppFrame : public wxFrame
{
	private:

	protected:

	public:
		wxTextCtrl* _wsPort;
		wxButton* _fireJson;
		wxTextCtrl* _logger;

		AppFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("TTS Test Application"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 638,263 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~AppFrame();

};

