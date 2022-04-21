#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/stc/stc.h>
#include <wx/wfstream.h>

enum class MainFrameMenuId {
	ID_OpenFile = 1
};

enum {
	MARGIN_LINE_NUMBERS,
	MARGIN_FOLD
};

class MainFrame : public wxFrame {
public:
	MainFrame();
private:
	wxStyledTextCtrl *m_TextCtrl;
	void InitMenu();

	void OnOpenFile(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);

	// Use https://github.com/akamud/vscode-theme-onelight colors
	void SetTextStyle();
	void EnableCodeFolding();
};

