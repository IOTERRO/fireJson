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
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class FrameMain
///////////////////////////////////////////////////////////////////////////////
class FrameMain : public wxFrame
{
	private:

	protected:
		wxStaticText* m_staticText6;
		wxMenuBar* _menubarMain;

		// Virtual event handlers, override them in your derived class
		virtual void _menuItemFileNewOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemFileOpenOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemFileSaveOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemFileExitOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemEditUndoOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemEditCutOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemEditCopyOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemEditPasteOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemEditDeleteOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemEditSelectAllOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemFireOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void _menuItemAboutOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxPanel* _textEditorPanel;
		wxStaticText* _fileName;
		wxTextCtrl* _textEditor;
		wxPanel* _fireJsonPanel;
		wxStaticText* _wsAddressLabel;
		wxTextCtrl* _wsAddressUrl;
		wxStaticText* _wsPortLabel;
		wxTextCtrl* _wsPortNumber;
		wxButton* _fireButton;
		wxMenu* _menuFile;
		wxMenuItem* _menuItemFileNew;
		wxMenuItem* _menuItemFileOpen;
		wxMenuItem* _menuItemFileSave;
		wxMenuItem* _menuItemFileExit;
		wxMenu* _menuEdit;
		wxMenuItem* _menuItemEditUndo;
		wxMenuItem* _menuItemEditCut;
		wxMenuItem* _menuItemEditCopy;
		wxMenuItem* _menuItemEditPaste;
		wxMenuItem* _menuItemEditDelete;
		wxMenuItem* _menuItemEditSelectAll;
		wxMenu* _menuFire;
		wxMenuItem* _menuItemFire;
		wxMenu* _menuHelp;
		wxMenuItem* _menuItemAbout;

		FrameMain( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("FireJSON"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 612,399 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~FrameMain();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DialogAbout
///////////////////////////////////////////////////////////////////////////////
class DialogAbout : public wxDialog
{
	private:

	protected:

	public:
		wxStaticText* _aboutText;
		wxButton* _aboutOkButton;

		DialogAbout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 256,200 ), long style = wxDEFAULT_DIALOG_STYLE );

		~DialogAbout();

};
