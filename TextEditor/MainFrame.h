#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/stc/stc.h>
#include <wx/wfstream.h>
#include <wx/treectrl.h>

#include <Windows.h>

#include <core.h>

enum class MainFrameMenuId {
	ID_OpenFile = 1,
	ID_CharAdded = 2,
	ID_SaveFile = 3,
	ID_Exec = 4,
	ID_Update = 5
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
	wxTreeCtrl *m_Tree;
	TText m_Text;
	size_t m_CurLinePosition = 0;
	std::string m_PathToFile;

	char GetPressedChar(wxKeyEvent& e) const;

	void InitMenu();

	void OnOpenFile(wxCommandEvent& event);
	void OnSaveFile(wxCommandEvent& event);
	void OnExecute(wxCommandEvent& event);
	void OnUpdate(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);

	void OnCharAdded(wxStyledTextEvent &evt);
	void OnKeyDown(wxKeyEvent &evt);
	void OnMouseDown(wxMouseEvent &evt);

	// Use https://github.com/akamud/vscode-theme-onelight colors
	void SetTextStyle();
	void EnableCodeFolding();

	void UpdateTree();

	//wxDECLARE_EVENT_TABLE();
};

