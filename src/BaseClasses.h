#pragma once

#include <wx\wx.h>
#include <wx\richtext\richtextctrl.h>
#include <wx\wxsf\wxShapeFramework.h>


//////////////////////////////////////////////////////////////////
////////////////////////// ReadOnlyRTC ///////////////////////////
//////////////////////////////////////////////////////////////////


class ReadOnlyRTC : public wxRichTextCtrl
{
	wxBitmap m_shadowBitmap;
public:
	ReadOnlyRTC(wxWindow* parent,
		wxWindowID id = -1,
		const wxString& value = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxBORDER_NONE);

	virtual void PaintAboveContent(wxDC& dc) override;
	virtual void Paste() override {};
};


//////////////////////////////////////////////////////////////////
////////////////////// BackgroundImageCanvas /////////////////////
//////////////////////////////////////////////////////////////////


class BackgroundImageCanvas : public wxSFShapeCanvas
{
protected:
	wxBitmap m_background;
	int m_bgx = 0, m_bgy = 0,
		m_bgxoffset = 0, m_bgyoffset = 0;
	double m_bgRatio, m_bgScale;
	int MAX_BG_OFFSET = 30;

	wxPoint m_mousePosOnEnter{ 0,0 };
	wxTimer m_bgAnimTimer;

public:
	BackgroundImageCanvas(wxSFDiagramManager* manager,
		wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxBORDER_NONE);

	inline wxBitmap& GetBackgroundBitmap() { return m_background; }
	inline double GetBackgroundScale() { return m_bgScale; }
	inline int GetBackgroundXOffset() { return m_bgxoffset; }
	inline int GetBackgroundYOffset() { return m_bgyoffset; }

	virtual void OnSize(wxSizeEvent& event);
	virtual void DrawBackground(wxDC& dc, bool fromPaint) override;

	virtual void OnUpdateVirtualSize(wxRect& rect) override;
	virtual void OnMouseMove(wxMouseEvent& event) override;

	void OnEnterWindow(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);

	void OnBgAnimTimer(wxTimerEvent& event);

	void _OnSize(wxSizeEvent& event);
};