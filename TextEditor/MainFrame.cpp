#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Text editor"), 
						 m_Text(new TTextLink("", nullptr, nullptr)) {
	m_PathToFile = "";
	m_Text.Reset();

	InitMenu();

	CreateStatusBar();
	//SetStatusText("TBC");

	m_TextCtrl = new wxStyledTextCtrl(this, wxID_ANY);
	SetTextStyle();
	//m_TextCtrl->Bind(wxEVT_STC_CHARADDED, &MainFrame::OnCharAdded);
	m_TextCtrl->Connect(wxEVT_STC_CHARADDED, wxStyledTextEventHandler(MainFrame::OnCharAdded), NULL, this);
	m_TextCtrl->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MainFrame::OnKeyDown), NULL, this);
	m_TextCtrl->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MainFrame::OnMouseDown), NULL, this);
	m_TextCtrl->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(MainFrame::OnMouseDown), NULL, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_TextCtrl, 1, wxEXPAND);
	SetSizer(sizer);

	Bind(wxEVT_MENU, &MainFrame::OnOpenFile, this, (int)MainFrameMenuId::ID_OpenFile);
	Bind(wxEVT_MENU, &MainFrame::OnSaveFile, this, (int)MainFrameMenuId::ID_SaveFile);
	Bind(wxEVT_MENU, &MainFrame::OnExecute, this, (int)MainFrameMenuId::ID_Exec);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

char MainFrame::GetPressedChar(wxKeyEvent& e) const {
	char base = e.m_keyCode;

	bool isLower = e.m_shiftDown;

	if (isLower) {
		base = tolower(base);
	}

	return base;
}

void MainFrame::InitMenu() {
	wxMenu* menuFile = new wxMenu;
	menuFile->Append((int)MainFrameMenuId::ID_OpenFile, "&Open File");
	menuFile->Append((int)MainFrameMenuId::ID_SaveFile, "&Save File");
	menuFile->Append((int)MainFrameMenuId::ID_Exec, "&Execude File");
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

	m_PathToFile = ofd.GetPath().mb_str(wxConvUTF8);
	SetStatusText(m_PathToFile);

	m_Text.Read(ofd.GetPath().mb_str(wxConvUTF8));
	std::stringstream ss;
	m_Text.Print(ss);
	m_TextCtrl->SetText(ss.str());
	m_TextCtrl->SetSelectionEnd(0);
	m_CurLinePosition = 0;
	m_Text.Reset();

	//m_TextCtrl->AddText(inputStream.Read)
}

void MainFrame::OnSaveFile(wxCommandEvent& event) {
	wxFileDialog sfd(
		this,
		("Save js file"),
		wxEmptyString,
		wxEmptyString,
		"JavaScripts files (*.js)|*.js",
		wxFD_SAVE
	);

	if (sfd.ShowModal() == wxID_CANCEL) {
		return;
	}

	std::stringstream ss;
	m_Text.Print(ss);

	m_PathToFile = sfd.GetPath().mb_str(wxConvUTF8);
	SetStatusText(m_PathToFile);

	std::ofstream file(m_PathToFile);
	file << ss.str();
	file.close();
}

void MainFrame::OnExecute(wxCommandEvent& event) {
	if (m_PathToFile == "") {
		return;
	}

	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	//std::string command = "node -e \"";

	//std::stringstream ss;
	//m_Text.PrintInline(ss);

	//command += ss.str();

	//command += '\"';
	//const char* c = command.c_str();

	std::string command = "node " + m_PathToFile;
	int r = system(command.c_str());
	FreeConsole();
}

void MainFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}

void MainFrame::OnCharAdded(wxStyledTextEvent& evt) {
	std::string buf = m_TextCtrl->GetCurLine().ToStdString();

	size_t count = 0;
	for (size_t i = 0; i < buf.size(); i++) {
		if (buf.at(i) != '\t') {
			break;
		}

		count++;
	}
	buf.erase(0, count);

	if (buf[buf.size() - 1] == '\n') {
		buf.pop_back();
	}

	m_Text.SetLine(buf);
	m_CurLinePosition++;

	return;
}

/*
 * 8 - backspace
 * 13 - enter
 * 316 - right arrow
 * 314 - left arrow
 * 315 - up arrow
 * 317 - down arrow
 */
void MainFrame::OnKeyDown(wxKeyEvent &evt) {
	//int curPosition = m_TextCtrl->GetCurP
	
	switch (evt.GetKeyCode()) {
	case 315:
	case 317:
		return;
	case 316:
		if (m_CurLinePosition == m_Text.GetLine().size()) {
			if (m_Text.IsTextEnded()) {
				return;
			}
			m_Text.GoNext();
			m_CurLinePosition = 0;

			m_TextCtrl->CharRight();
			if (m_TextCtrl->GetCharAt(m_TextCtrl->GetCurrentPos()) == '{' || m_TextCtrl->GetCharAt(m_TextCtrl->GetCurrentPos()) == '}') {
				m_TextCtrl->CharRight();
				m_TextCtrl->CharRight();
			}

			while (m_TextCtrl->GetCharAt(m_TextCtrl->GetCurrentPos()) == '\t') {
				m_TextCtrl->CharRight();
			}

			return;
		}
		
		m_TextCtrl->CharRight();
		m_CurLinePosition++;
		return;
	case 314:
		if (m_CurLinePosition == 0) {
			if (m_TextCtrl->GetCurrentPos() == 0) {
				return;
			}

			m_TextCtrl->CharLeft();

			while (m_TextCtrl->GetCharAt(m_TextCtrl->GetCurrentPos()) == '\t') {
				m_TextCtrl->CharLeft();
			}


			if (m_TextCtrl->GetCharAt(m_TextCtrl->GetCurrentPos() - 1) == '{' || m_TextCtrl->GetCharAt(m_TextCtrl->GetCurrentPos() - 1) == '}') {
				m_TextCtrl->CharLeft();
				m_TextCtrl->CharLeft();
			}


			m_Text.GoPrev();
			m_CurLinePosition = m_Text.GetLine().size();
			return;
		}

		m_TextCtrl->CharLeft();
		m_CurLinePosition--;
		return;
	case 8:
		if (m_CurLinePosition != 0) {
			m_TextCtrl->DeleteBack();
			std::string buf = m_TextCtrl->GetCurLine().ToStdString();

			size_t count = 0;
			for (size_t i = 0; i < buf.size(); i++) {
				if (buf.at(i) != '\t') {
					break;
				}

				count++;
			}
			buf.erase(0, count);

			if (buf[buf.size() - 1] == '\n') {
				buf.pop_back();
			}

			m_Text.SetLine(buf);
			m_CurLinePosition--;
			return;
		}

		if (m_Text.GetLine().size() == 0) {
			m_Text.DelCurrent();
			TTextLink::MemCleaner(m_Text);

			std::stringstream ss;
			m_Text.Print(ss);
			m_TextCtrl->SetText(ss.str());
			m_TextCtrl->SetSelectionEnd(0);
			m_CurLinePosition = 0;
			m_Text.Reset();
		}

		return;
	case 13:
		auto pos = m_TextCtrl->GetCurrentPos();
		m_Text.InsNextSection("");
		std::stringstream ss;
		m_Text.Print(ss);
		m_TextCtrl->SetText(ss.str());
		m_TextCtrl->SetSelectionEnd(0);
		m_CurLinePosition = 0;
		m_Text.Reset();

		return;
	}

	evt.Skip();

	//if (evt.IsKeyInCategory(
	//	wxKeyCategoryFlags::WXK_CATEGORY_ARROW		|
	//	wxKeyCategoryFlags::WXK_CATEGORY_CUT		|
	//	wxKeyCategoryFlags::WXK_CATEGORY_JUMP       |
	//	wxKeyCategoryFlags::WXK_CATEGORY_NAVIGATION |
	//	wxKeyCategoryFlags::WXK_CATEGORY_PAGING		|
	//	wxKeyCategoryFlags::WXK_CATEGORY_TAB
	//)) {
	//	return;
	//}
	//
	//m_TextCtrl->AddText(GetPressedChar(evt));

	//std::string buf = m_TextCtrl->GetCurLine().ToStdString();

	//size_t count = 0;
	//for (size_t i = 0; i < buf.size(); i++) {
	//	if (buf.at(i) != '\t') {
	//		break;
	//	}

	//	count++;
	//}
	//buf.erase(0, count);

	//if (buf[buf.size() - 1] == '\n') {
	//	buf.pop_back();
	//}

	//m_Text.SetLine(buf);
	//m_CurLinePosition++;

	//evt.Skip();
}

void MainFrame::OnMouseDown(wxMouseEvent& evt) {
	int a = 0;
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

//wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
//
//EVT_STC(wxEVT_STC_CHARADDED, MainFrame::OnCharAdded)
//
//wxEND_EVENT_TABLE()
