#ifndef WXFLATSCROLLBAR_H_
#define WXFLATSCROLLBAR_H_
#pragma once

#include <wx\wx.h>
#include <wx\generic\scrolwin.h>
#include <wx\richtext\richtextctrl.h>

enum
{
	TIMER_SetOffOnHold,
	TIMER_ScrollOnHold
};

class CustomRTCScrollbar : public wxControl
{
private:
	wxRichTextCtrl* m_rtc = nullptr;
	int m_orientation = wxVERTICAL;

	double m_dPrivateScale = 1.0;
	double m_dRatio = 1.0;
	double m_dBorderRadius = 2.0;

	wxRect m_currentThumbRect;
	int m_nCurrentPos = 0;
	int m_nMinWidth = 5;

	int m_nBarWidth = 1;
	bool m_bIsDragging = false;
	int m_nDragOffset = 0;

	wxTimer m_setOffOnHold;
	wxTimer m_scrollOnHold;

public:
	CustomRTCScrollbar(wxWindow* parent, wxRichTextCtrl* target, wxWindowID id, int orientation = wxVERTICAL,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

	void RecalculateSelf();
	void DoScroll(int pos);

	void OnTargetScroll(wxScrollWinEvent& event);
	void OnTargetSize(wxSizeEvent& event);

	void OnSetOffOnHold(wxTimerEvent& event);
	void OnScrollOnHold(wxTimerEvent& event);

	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

	void OnLeftDown(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);

	void OnMouseMove(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);
	void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif