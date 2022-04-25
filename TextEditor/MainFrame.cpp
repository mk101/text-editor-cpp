#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Text editor"), m_Text() {
	InitMenu();

	CreateStatusBar();
	SetStatusText("TBC");

	m_TextCtrl = new wxStyledTextCtrl(this, wxID_ANY);
	SetTextStyle();

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_TextCtrl, 1, wxEXPAND);
	SetSizer(sizer);

	Bind(wxEVT_MENU, &MainFrame::OnOpenFile, this, (int)MainFrameMenuId::ID_OpenFile);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::InitMenu() {
	wxMenu* menuFile = new wxMenu;
	menuFile->Append((int)MainFrameMenuId::ID_OpenFile, "&Open File\tCtrl+O");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	SetMenuBar(menuBar);
}

void MainFrame::OnOpenFile(wxCommandEvent& event) {
	wxFileDialog ofd(
		this,
		("Open js file"),
		wxEmptyString,
		wxEmptyString,
		"JavaScripts files (*.js)|*.js",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);

	if (ofd.ShowModal() == wxID_CANCEL) {
		return;
	}

	/*wxFileInputStream inputStream(ofd.GetPath());
	if (!inputStream.IsOk()) {
		wxLogError("Cannot open file '%s'", ofd.GetPath());
		return;
	}
	
	size_t size = inputStream.GetSize();
	char *buf = new char[size + 1];
	inputStream.Read((void*)buf, size);
	buf[size] = '\0';

	wxString text = wxString::FromAscii(buf);
	delete[] buf;*/
	m_Text.Read(ofd.GetPath().mb_str(wxConvUTF8));
	std::stringstream ss;
	m_Text.Print(ss);
	m_TextCtrl->AddText(ss.str());

	//m_TextCtrl->AddText(inputStream.Read)
}

void MainFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}

void MainFrame::SetTextStyle() {
	m_TextCtrl->StyleClearAll();

	m_TextCtrl->SetLexer(wxSTC_LEX_ESCRIPT);
	m_TextCtrl->SetKeyWords(
		0,
		wxT("async await break case catch class const continue debugger default delete do else enum export extends false finally for function if implements import in instanceof interface let new null package private protected public return super switch static this throw try true typeof var void while with yield")
	);

	//EnableCodeFolding();

	for (size_t i = wxSTC_ESCRIPT_DEFAULT; i <= wxSTC_ESCRIPT_WORD3; i++) {
		m_TextCtrl->StyleSetFaceName(i, wxT("menlo"));
		m_TextCtrl->StyleSetSize(i, 12);
	}
	

	m_TextCtrl->SetMarginWidth(MARGIN_LINE_NUMBERS, 20);
	m_TextCtrl->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	m_TextCtrl->SetWrapMode(wxSTC_WRAP_NONE);

	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_DEFAULT, wxColor(56, 58, 66));

	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_COMMENT, wxColor(160, 161, 167));
	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_COMMENTLINE, wxColor(160, 161, 167));
	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_COMMENTDOC, wxColor(160, 161, 167));

	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_NUMBER, wxColor(64, 120, 242));
	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_WORD, wxColor(166, 38, 164));
	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_STRING, wxColor(80, 161, 79));

	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_OPERATOR, wxColor(1, 132, 188));
	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_IDENTIFIER, wxColor(56, 58, 66));
	m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_BRACE, wxColor(56, 58, 66));

	m_TextCtrl->StyleSetItalic(wxSTC_ESCRIPT_COMMENT, true);
	m_TextCtrl->StyleSetItalic(wxSTC_ESCRIPT_COMMENTLINE, true);
	m_TextCtrl->StyleSetItalic(wxSTC_ESCRIPT_COMMENTDOC, true);

	//m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_WORD2, wxColor(166, 38, 164));
	//m_TextCtrl->StyleSetForeground(wxSTC_ESCRIPT_WORD3, wxColor(80, 161, 79));
}

void MainFrame::EnableCodeFolding() {
	m_TextCtrl->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
	m_TextCtrl->SetMarginWidth(MARGIN_FOLD, 15);
	m_TextCtrl->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
	m_TextCtrl->StyleSetBackground(MARGIN_FOLD, wxColor(200, 200, 200));
	m_TextCtrl->SetMarginSensitive(MARGIN_FOLD, true);

	m_TextCtrl->SetProperty(wxT("fold"), wxT("1"));
	m_TextCtrl->SetProperty(wxT("fold.comment"), wxT("1"));
	m_TextCtrl->SetProperty(wxT("fold.compact"), wxT("1"));

	wxColor grey(100, 100, 100);
	m_TextCtrl->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW);
	m_TextCtrl->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, grey);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, grey);

	m_TextCtrl->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN);
	m_TextCtrl->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, grey);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, grey);

	m_TextCtrl->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
	m_TextCtrl->MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, grey);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, grey);

	m_TextCtrl->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW);
	m_TextCtrl->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, grey);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, _T("WHITE"));

	m_TextCtrl->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
	m_TextCtrl->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, grey);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, _T("WHITE"));

	m_TextCtrl->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
	m_TextCtrl->MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, grey);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, grey);

	m_TextCtrl->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
	m_TextCtrl->MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, grey);
	m_TextCtrl->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, grey);
}
