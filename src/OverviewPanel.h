#ifndef OVERVIEWPANEL_H_
#define OVERVIEWPANEL_H_
#pragma once

#include <wx\wx.h>

class OverviewPanel : public wxPanel{
private:
	wxBitmap m_background{};
	int m_bgx = 0, m_bgy = 0;
	double m_bgRatio = 0.0;
	double m_bgScale = 1.0;

public:
	OverviewPanel(wxWindow* parent);

	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif