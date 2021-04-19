#include "Scrollbar.h"

#include <wx\dcbuffer.h>
#include <wx\dcgraph.h>

wxBEGIN_EVENT_TABLE(CustomRTCScrollbar, wxPanel)

EVT_SIZE(CustomRTCScrollbar::OnSize)
EVT_PAINT(CustomRTCScrollbar::OnPaint)

EVT_LEFT_DOWN(CustomRTCScrollbar::OnLeftDown)
EVT_LEFT_UP(CustomRTCScrollbar::OnLeftUp)

EVT_MOTION(CustomRTCScrollbar::OnMouseMove)
EVT_LEAVE_WINDOW(CustomRTCScrollbar::OnLeaveWindow)
EVT_MOUSE_CAPTURE_LOST(CustomRTCScrollbar::OnMouseCaptureLost)

EVT_TIMER(TIMER_SetOffOnHold, CustomRTCScrollbar::OnSetOffOnHold)
EVT_TIMER(TIMER_ScrollOnHold, CustomRTCScrollbar::OnScrollOnHold)

wxEND_EVENT_TABLE()

CustomRTCScrollbar::CustomRTCScrollbar(wxWindow* parent, wxRichTextCtrl* target, wxWindowID id, int orientation,
	const wxPoint& pos, const wxSize& size) : wxControl(parent, id, pos, size, wxBORDER_NONE),
	m_setOffOnHold(this, TIMER_SetOffOnHold), m_scrollOnHold(this, TIMER_ScrollOnHold)
{
	m_rtc = target;
	m_orientation = orientation;

	if ( size != wxDefaultSize )
		m_minWidth = size.y;

	SetMinSize(wxSize(m_minWidth, -1));

	SetBackgroundStyle(wxBG_STYLE_PAINT);
	Bind(wxEVT_ERASE_BACKGROUND, [](wxEraseEvent&) {});

	SetBackgroundColour(wxColour(0, 0, 0));
	SetForegroundColour(wxColour(255, 255, 255));

	m_rtc->ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_NEVER);

	m_rtc->Bind(wxEVT_SIZE, &CustomRTCScrollbar::OnTargetSize, this);
	m_rtc->Bind(wxEVT_SCROLLWIN_BOTTOM, &CustomRTCScrollbar::OnTargetScroll, this);
	m_rtc->Bind(wxEVT_SCROLLWIN_TOP, &CustomRTCScrollbar::OnTargetScroll, this);
	m_rtc->Bind(wxEVT_SCROLLWIN_LINEDOWN, &CustomRTCScrollbar::OnTargetScroll, this);
	m_rtc->Bind(wxEVT_SCROLLWIN_LINEUP, &CustomRTCScrollbar::OnTargetScroll, this);
	m_rtc->Bind(wxEVT_SCROLLWIN_PAGEDOWN, &CustomRTCScrollbar::OnTargetScroll, this);
	m_rtc->Bind(wxEVT_SCROLLWIN_PAGEUP, &CustomRTCScrollbar::OnTargetScroll, this);
	m_rtc->Bind(wxEVT_SCROLLWIN_THUMBRELEASE, &CustomRTCScrollbar::OnTargetScroll, this);
	m_rtc->Bind(wxEVT_SCROLLWIN_THUMBTRACK, &CustomRTCScrollbar::OnTargetScroll, this);

	Hide();
}

void CustomRTCScrollbar::RecalculateSelf()
{
	wxSize size = GetSize();
	wxSize rtcSize = m_rtc->GetClientSize();
	wxSize rtcVirtualSize = m_rtc->GetVirtualSize();

	if ( rtcVirtualSize.y < rtcSize.y || rtcVirtualSize.x < rtcSize.x )
	{
		if ( IsShown() )
			Hide();

		return;
	}
	else
	{
		if ( !IsShown() )
		{
			Show();
			SendSizeEventToParent();
		}
	}

	if ( m_orientation == wxVERTICAL )
	{
		m_ratio = (double)size.y / rtcVirtualSize.y;

		int yo;
		m_rtc->CalcUnscrolledPosition(0, 0, nullptr, &yo);
		m_currentPos = yo * m_ratio;
		m_currentThumbRect = wxRect(0, m_currentPos, size.x, (int)(rtcSize.y * m_ratio));
	}

	Refresh();
	Update();
}

void CustomRTCScrollbar::DoScroll(int pos)
{
	int max = GetSize().y - m_currentThumbRect.GetHeight();
	if ( pos < 0 )
		m_currentPos = 0;
	else if ( pos > max )
		m_currentPos = max;
	else
		m_currentPos = pos;

	m_currentThumbRect.SetY(m_currentPos);
	m_rtc->Freeze();
	m_rtc->Scroll(0, ((double)m_currentPos / m_ratio) / m_rtc->GetLineHeight());
	m_rtc->Thaw();

	Refresh();
	Update();
}

void CustomRTCScrollbar::OnTargetScroll(wxScrollWinEvent& event)
{
	RecalculateSelf();
	event.Skip();
}

void CustomRTCScrollbar::OnTargetSize(wxSizeEvent& event)
{
	RecalculateSelf();
	event.Skip();
}

void CustomRTCScrollbar::OnSetOffOnHold(wxTimerEvent& event)
{
	m_scrollOnHold.Start(100);
}

void CustomRTCScrollbar::OnScrollOnHold(wxTimerEvent& event)
{
	wxMouseEvent mouseEvent;
	mouseEvent.SetPosition(ScreenToClient(wxGetMousePosition()));

	if ( !m_currentThumbRect.Contains(mouseEvent.GetPosition()) )
		OnLeftDown(mouseEvent);
}

void CustomRTCScrollbar::OnSize(wxSizeEvent& event)
{
	RecalculateSelf();
	event.Skip();
}

void CustomRTCScrollbar::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);
	wxGCDC gdc(dc);
	wxSize size = GetSize();

	gdc.SetPen(*wxTRANSPARENT_PEN);
	gdc.SetBrush(wxBrush(GetBackgroundColour()));
	gdc.DrawRectangle(wxPoint(0, 0), size);

	if ( m_barWidth > 0 )
	{
		gdc.SetPen(wxPen(wxColour(255, 255, 255, 50), m_barWidth));
		int x = size.x / 2;
		gdc.DrawLine(wxPoint(x, 0), wxPoint(x, size.y));
	}

	gdc.SetPen(*wxTRANSPARENT_PEN);
	gdc.SetBrush(wxBrush(GetForegroundColour()));
	gdc.DrawRoundedRectangle(m_currentThumbRect, m_borderRadius);
}

void CustomRTCScrollbar::OnLeftDown(wxMouseEvent& event)
{
	wxPoint pos = event.GetPosition();
	if ( m_currentThumbRect.Contains(pos) )
	{
		m_isDragging = true;
		CaptureMouse();
		m_dragOffset = pos.y - m_currentPos;
	} else
	{
		int toScroll = m_currentPos;

		if ( m_orientation == wxVERTICAL )
		{
			if ( pos.y > m_currentPos )
				toScroll += m_currentThumbRect.GetHeight() - 20;
			else
				toScroll -= m_currentThumbRect.GetHeight() + 20;
		} else
		{
			if ( pos.x > m_currentPos )
				toScroll += m_currentThumbRect.GetWidth() - 20;
			else
				toScroll -= m_currentThumbRect.GetWidth() + 20;
		}

		DoScroll(toScroll);
		m_setOffOnHold.Start(400, true);
	}
}

void CustomRTCScrollbar::OnLeftUp(wxMouseEvent& event)
{
	if ( HasCapture() )
		ReleaseMouse();

	m_isDragging = false;
	m_setOffOnHold.Stop();
	m_scrollOnHold.Stop();
	RecalculateSelf();
}

void CustomRTCScrollbar::OnMouseMove(wxMouseEvent& event)
{
	if ( m_isDragging )
	{
		if ( m_orientation == wxVERTICAL )
			DoScroll(event.GetY() - m_dragOffset);
		else
			DoScroll(event.GetX() - m_dragOffset);
	}
}

void CustomRTCScrollbar::OnLeaveWindow(wxMouseEvent& event)
{
	m_setOffOnHold.Stop();
	m_scrollOnHold.Stop();
}

void CustomRTCScrollbar::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
	m_isDragging = false;
	m_scrollOnHold.Stop();
	RecalculateSelf();
}