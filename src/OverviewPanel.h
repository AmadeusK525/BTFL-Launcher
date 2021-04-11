#ifndef OVERVIEWPANEL_H_
#define OVERVIEWPANEL_H_
#pragma once

#include <wx\wx.h>
#include <wx\scrolwin.h>

#include "PatchNotes.h"

class OverviewPanel : public wxPanel {
private:
	wxBitmap m_background{};
	int m_bgx = 0, m_bgy = 0;
	int m_sbgx = 0, m_sbgy = 0;
	double m_bgRatio = 0.0;
	double m_bgScale = 1.0;

	PatchNotesWindow* m_patchNotesWindow = nullptr;
	wxBoxSizer* m_sizer = nullptr;

public:
	OverviewPanel(wxWindow* parent);

	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnDoubleClick(wxMouseEvent& event);

	inline double GetBackgroundRatio() { return m_bgRatio; }
	inline const wxBitmap& GetBackgroundBitmap() { return m_background; }

	inline PatchNotesWindow* GetPatchNotes() { return m_patchNotesWindow; }

	DECLARE_EVENT_TABLE()
};

#endif