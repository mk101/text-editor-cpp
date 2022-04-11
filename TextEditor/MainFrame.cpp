#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Text editor") {
	InitMenu();

	CreateStatusBar();
	SetStatusText("TBC");

	wxPanel *panel = new wxPanel(this, wxID_ANY);
	m_TextCtrl = new wxStyledTextCtrl(panel, wxID_ANY);

	m_TextCtrl->SetLexer(wxSTC_LEX_PYTHON);
	//m_TextCtrl->SetLexerLanguage(wxT("python"));
	m_TextCtrl->SetProperty("fold.comment.python", "true");
	m_TextCtrl->SetProperty("fold.quotes.python", "true");
	m_TextCtrl->SetProperty("tab.timmy.whinge.level", "4");

	m_TextCtrl->StyleSetForeground(wxSTC_P_COMMENTLINE, wxColor(60, 162, 2));
	m_TextCtrl->StyleSetForeground(wxSTC_P_CHARACTER, wxColor(255, 60, 10));
	m_TextCtrl->StyleSetForeground(wxSTC_P_STRING, wxColor(255, 60, 10));
	m_TextCtrl->StyleSetForeground(wxSTC_P_WORD, wxColor(0, 0, 255));
	m_TextCtrl->SetKeyWords(
		0,
		wxT("False None True and as assert async await break class continue def del elif else except finally for from global if import in is lambda nonlocal not or pass raise return try while with yield")
	);

	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->Add(m_TextCtrl, 1, wxEXPAND);
	panel->SetSizer(panelSizer);

	// Set up the sizer for the frame and resize the frame
	// according to its contents
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(topSizer);

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
		("Open python file"),
		wxEmptyString,
		wxEmptyString,
		"Python files (*.py)|*.py",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);

	if (ofd.ShowModal() == wxID_CANCEL) {
		return;
	}

	wxFileInputStream inputStream(ofd.GetPath());
	if (!inputStream.IsOk()) {
		wxLogError("Cannot open file '%s'", ofd.GetPath());
		return;
	}
	
	size_t size = inputStream.GetSize();
	char *buf = new char[size + 1];
	inputStream.Read((void*)buf, size);
	buf[size] = '\0';

	wxString text = wxString::FromAscii(buf);
	delete[] buf;

	m_TextCtrl->AddText(text);

	//m_TextCtrl->AddText(inputStream.Read)
}

void MainFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}