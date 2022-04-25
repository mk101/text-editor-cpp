#include "App.h"

bool App::OnInit() {
	TTextLink::InitMemSystem(1024);
	m_Frame = new MainFrame();
	m_Frame->Show(true);
	return true;
}

wxIMPLEMENT_APP(App);