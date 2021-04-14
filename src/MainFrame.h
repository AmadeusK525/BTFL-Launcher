#ifndef MAINFRAME_H_
#define MAINFRAME_H_
#pragma once

#include <wx\wx.h>

#include "MainPanel.h"
#include "PatchNotes.h"

class MainFrame : public wxFrame {
private:
	MainPanel* m_mainPanel = nullptr;
	wxSFDiagramManager m_mainPanelManager;

	LeftSidebar* m_patchNotesWindow = nullptr;

public:
	MainFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxFrameNameStr);

	void LoadPatchNotes();
	inline const wxBitmap& GetBackgroundBitmap() { return m_mainPanel->GetBackgroundBitmap(); }
};

#endif