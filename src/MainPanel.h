#ifndef MAINPANEL_H_
#define MAINPANEL_H_
#pragma once

#include <wx\wx.h>
#include <wx\wxsf\wxShapeFramework.h>

#include "PatchNotes.h"

class MainPanel;

class TransparentButton : public wxSFRoundRectShape {
protected:
	MainPanel* m_parent = nullptr;
	
	wxString m_label;
	wxFont m_font;
	wxColour m_textColour{ 255,255,255 };

	wxBitmap* m_bitmap = nullptr;

	int m_xPadding = 30, m_yPadding = 15;
	int m_xLabel = 30, m_xBitmap = 30;

public:
	XS_DECLARE_CLONABLE_CLASS(TransparentButton);
	TransparentButton() = default;
	TransparentButton(const wxString& label, const wxRealPoint& pos, const wxRealPoint& size,
		double radius, wxSFDiagramManager* manager);
	TransparentButton(const TransparentButton& other);

	void SetLabel(const wxString& label);
	void SetFont(const wxFont& font);
	void SetBitmap(const wxBitmap& bmp);

	void RecalculateSelf();

	void DrawLabel(wxDC& dc);

	virtual void DrawNormal(wxDC& dc);
	virtual void DrawHover(wxDC& dc);
	virtual void DrawHighlighted(wxDC& dc);
};


/////////////////////////////////////////////////////////////////////
//////////////////////////// MainPanel //////////////////////////////
/////////////////////////////////////////////////////////////////////


class MainPanel : public wxSFShapeCanvas {
private:
	MainFrame* m_mainFrame = nullptr;

	wxBitmap m_background;
	wxBitmap m_logo;
	int m_bgx = 0, m_bgy = 0,
		m_bgxoffset = 0, m_bgyoffset = 0;
	int m_logox = 0, m_logoy = 0;
	double m_bgRatio, m_bgScale;
	const int MAX_BG_OFFSET = 20;

	TransparentButton* m_mainButton = nullptr;

	wxPoint m_mousePosOnEnter{ 0,0 };
	wxTimer m_bgAnimTimer;

public:
	MainPanel(wxSFDiagramManager* manager, 
		wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxBORDER_NONE);

	inline const wxBitmap& GetBackgroundBitmap() { return m_background; }
	void RepositionAll();

	void OnSize(wxSizeEvent& event);
	virtual void OnMouseMove(wxMouseEvent& event) override;

	virtual void OnUpdateVirtualSize(wxRect& rect) override;
	virtual void DrawBackground(wxDC& dc, bool fromPaint) override;

	void OnEnterWindow(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);

	void OnBgAnimTimer(wxTimerEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif