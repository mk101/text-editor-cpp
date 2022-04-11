#include "App.h"

bool App::OnInit() {
	m_Frame = new MainFrame();
	m_Frame->Show(true);
	return true;
}

wxIMPLEMENT_APP(App);