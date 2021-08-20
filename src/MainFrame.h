#ifndef MAINFRAME_H_
#define MAINFRAME_H_
#pragma once

#include <wx\wx.h>

#include "MainPanel.h"
#include "SecondaryPanel.h"
#include "PatchNotes.h"

enum
{
	TIMER_LoadPatchNotes
};

class MainFrame : public wxFrame
{
private:
	MainPanel* m_mainPanel = nullptr;
	SecondaryPanel* m_seconPanel = nullptr;
	wxSFDiagramManager m_mainPanelManager;
	wxSFDiagramManager m_seconPanelManager;

	LeftSidebar* m_patchNotesWindow = nullptr;
	wxPanel* m_copyrightPanel = nullptr;

	wxTimer m_loadPatchNotesTimer;
	unsigned int m_nLoadPatchNotesAttempts = 0;
	unsigned int m_nLoadPatchNotesCountdown = 0;

	bool m_hasUserAgreedToDisclaimer = false;

public:
	MainFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxFrameNameStr);

	void LoadPatchNotes();
	void OnLoadPatchNotesTimer(wxTimerEvent& event);

	void ShowMainPanel();
	void ShowSecondaryPanel();
	void ShowDisclaimer();
	void ShowSettings();

	void VerifyIso() { m_mainPanel->VerifyIso(); }

	void AcceptDisclaimer() { m_hasUserAgreedToDisclaimer = true; }
	void OnReadDisclaimer(wxMouseEvent& event);

	bool IsIsoSelected() { return m_mainPanel->GetISOFile().FileExists(); }
	bool HasUserAgreedToDisclaimer() { return m_hasUserAgreedToDisclaimer; }

	inline const wxBitmap& GetBackgroundBitmap() { return m_mainPanel->GetBackgroundBitmap(); }

	wxDECLARE_EVENT_TABLE();
};

#endif