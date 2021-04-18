#ifndef MAINFRAME_H_
#define MAINFRAME_H_
#pragma once

#include <wx\wx.h>

#include "MainPanel.h"
#include "SecondaryPanel.h"
#include "PatchNotes.h"

class MainFrame : public wxFrame {
private:
	MainPanel* m_mainPanel = nullptr;
	SecondaryPanel* m_seconPanel = nullptr;
	wxSFDiagramManager m_mainPanelManager;
	wxSFDiagramManager m_seconPanelManager;

	LeftSidebar* m_patchNotesWindow = nullptr;
	wxPanel* m_copyrightPanel = nullptr;

public:
	MainFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxFrameNameStr);

	void LoadPatchNotes();

	void OnReadDisclaimer(wxMouseEvent& event);

	inline const wxBitmap& GetBackgroundBitmap() { return m_mainPanel->GetBackgroundBitmap(); }
};

#endif