#ifndef MAINFRAME_H_
#define MAINFRAME_H_
#pragma once

#include <wx\wx.h>
#include "OverviewPanel.h"

class MainFrame : public wxFrame {
private:
	OverviewPanel* m_overview = nullptr;

public:
	MainFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxFrameNameStr);

	inline OverviewPanel* GetOverviewPanel() { return m_overview; }
};

#endif