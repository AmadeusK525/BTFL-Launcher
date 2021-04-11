#ifndef PATCHNOTESWINODW_H_
#define PATCHNOTESWINODW_H_
#pragma once

#include <wx\wx.h>
#include <wx\scrolwin.h>

class OverviewPanel;

class PatchNotesWindow : public wxScrolledWindow {
	OverviewPanel* m_overviewPanel = nullptr;

	wxString m_title, m_titleToDraw;
	wxString m_content, m_contentToDraw;
	wxSize m_titleSize, m_contentSize;

	double m_bgScale = 1.0;
	int m_bgx = 0, m_bgy = 0;
	wxPoint m_separatorPos;
	int m_contentY = 0;

public:
	PatchNotesWindow(wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxHSCROLL | wxVSCROLL);

	inline void SetBackgroundX(int x) { m_bgx = x; }
	inline void SetBackgroundY(int y) { m_bgy = y; }
	inline void SetBackgroundScale(double scale) { m_bgScale = scale; }

	inline void SetTitle(const wxString& title) { 
		m_title = title;
		DoReposition(false, true);
	}

	inline void SetContent(const wxString& content) {
		m_content = content;
		DoReposition(true, false);
	}

	void DoReposition(bool calcContent, bool calcTitle);

	void OnPaint(wxPaintEvent& dc);
	void OnScroll(wxScrollWinEvent& event);

	DECLARE_EVENT_TABLE()

private:
	void CalcWrappedText(wxString* str, wxString* strToDraw,const wxFont& font, wxSize* size = nullptr);
};

#endif