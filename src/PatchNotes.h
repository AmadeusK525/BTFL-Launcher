#ifndef PATCHNOTESWINODW_H_
#define PATCHNOTESWINODW_H_
#pragma once

#include <wx\wx.h>
#include <wx\scrolwin.h>
#include <wx\richtext\richtextctrl.h>

#include "Scrollbar.h"

class MainFrame;

class PatchNotesWindow : public wxRichTextCtrl {
	wxBitmap m_shadowBitmap;
public:
	PatchNotesWindow(wxWindow* parent, 
		wxWindowID id = -1,
		const wxString& value = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxBORDER_NONE);

	virtual void PaintAboveContent(wxDC& dc) override;
	virtual void Paste() override {};
};


///////////////////////////////////////////////////////////////////////
/////////////////////////// HyperlinkPanel ////////////////////////////
///////////////////////////////////////////////////////////////////////


class HyperlinkPanel : public wxPanel {
	wxBitmap m_bitmap;
	wxString m_url;

	int m_padding = 1;
	int m_bgx = 0, m_bgy = 0;
	double m_bgScale = 0;
	double m_bgRatio = 0;

	bool m_isHovering = false;

public:
	HyperlinkPanel(wxWindow* parent, const wxString& url, const wxBitmap& bmp, const wxSize& size = wxDefaultSize,
		const wxPoint& pos = wxDefaultPosition, long style = wxBORDER_NONE);

	void RecalculateSelf();

	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

	void OnLeftDown(wxMouseEvent& event);
	void OnEnterWindow(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);

	wxDECLARE_EVENT_TABLE();
};


///////////////////////////////////////////////////////////////////////
///////////////////////////// LeftSidebar /////////////////////////////
///////////////////////////////////////////////////////////////////////


class LeftSidebar : public wxPanel {
	MainFrame* m_mainFrame = nullptr;
	PatchNotesWindow* m_rtc = nullptr;

	CustomRTCScrollbar* m_scrollbar = nullptr;

	bool m_isDragging = false;

public:
	LeftSidebar(wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxBORDER_NONE);

	bool Load();
	void OnPaint(wxPaintEvent& event);
	void OnMove(wxMouseEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif