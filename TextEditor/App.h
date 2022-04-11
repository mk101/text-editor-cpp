#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <core.h>
#include "MainFrame.h"

class App : public wxApp {
public:
private:
	virtual bool OnInit() override;
	MainFrame *m_Frame;
};

