#include "BaseClasses.h"

ReadOnlyRTC::ReadOnlyRTC(wxWindow* parent,
	wxWindowID id,
	const wxString& value,
	const wxPoint& pos,
	const wxSize& size,
	long style) : wxRichTextCtrl(parent, id, value, pos, size, style) {
	SetCursor(wxCURSOR_DEFAULT);
	SetTextCursor(wxCURSOR_DEFAULT);
	Bind(wxEVT_SET_FOCUS, [](wxFocusEvent&) {});
	Bind(wxEVT_LEFT_DOWN, [](wxMouseEvent&) {});
	Bind(wxEVT_RIGHT_UP, [](wxMouseEvent&) {});
	Bind(wxEVT_CHAR, [](wxKeyEvent&) {});
	Bind(wxEVT_KEY_DOWN, [](wxKeyEvent&) {});

	wxRichTextAttr attr;
	attr.SetFont(wxFontInfo(12).FaceName("Times New Roman"));
	attr.SetAlignment(wxTEXT_ALIGNMENT_JUSTIFIED);
	attr.SetLeftIndent(64);
	attr.SetRightIndent(64);
	attr.SetLineSpacing(10);
	attr.SetTextColour(wxColour(210, 210, 210));

	SetBasicStyle(attr);
	SetBackgroundColour(wxColour(0, 0, 0));
	SetFontScale(1.13);

	m_shadowBitmap.LoadFile("Assets\\Scroll Shadow\\Large@2x.png", wxBITMAP_TYPE_PNG);
}

void ReadOnlyRTC::PaintAboveContent(wxDC& dc) {
	int yo;
	CalcUnscrolledPosition(0, 0, nullptr, &yo);
	wxSize size = GetClientSize();

	double scale = (double)size.x / m_shadowBitmap.GetWidth();
	dc.SetUserScale(scale, scale);
	dc.DrawBitmap(m_shadowBitmap, wxPoint(0, (size.y + yo - ((double)m_shadowBitmap.GetHeight() * scale)) / scale), true);
	dc.SetUserScale(1.0, 1.0);
}


//////////////////////////////////////////////////////////////////
////////////////////// BackgroundImageCanvas /////////////////////
//////////////////////////////////////////////////////////////////


BackgroundImageCanvas::BackgroundImageCanvas(wxSFDiagramManager* manager, 
	wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style) : wxSFShapeCanvas(manager, parent, id, pos, size, style), m_bgAnimTimer(this, 1234) {
	EnableGC(true);
	SetHoverColour(wxColour(255, 255, 255));

	RemoveStyle(sfsMULTI_SELECTION);
	RemoveStyle(sfsPROCESS_MOUSEWHEEL);

	Bind(wxEVT_SIZE, &BackgroundImageCanvas::_OnSize, this);
	Bind(wxEVT_ENTER_WINDOW, &BackgroundImageCanvas::OnEnterWindow, this);
	Bind(wxEVT_LEAVE_WINDOW, &BackgroundImageCanvas::OnLeaveWindow, this);
	Bind(wxEVT_TIMER, &BackgroundImageCanvas::OnBgAnimTimer, this);
}

void BackgroundImageCanvas::OnSize(wxSizeEvent& event) {
	wxSize size = GetSize();
	double curRatio = (double)size.x / size.y;
	int overallOffset = MAX_BG_OFFSET * 2;

	if (curRatio > m_bgRatio) {
		m_bgScale = (double)(size.x + overallOffset) / m_background.GetWidth();
		m_bgy = (double)(((size.y - overallOffset) - ((double)m_background.GetHeight() * m_bgScale)) / 2) / m_bgScale;
		m_bgx = -MAX_BG_OFFSET;
	} else {
		m_bgScale = (double)(size.y + overallOffset) / m_background.GetHeight();
		m_bgx = (double)(((size.x - overallOffset) - ((double)m_background.GetWidth() * m_bgScale)) / 2) / m_bgScale;
		m_bgy = -MAX_BG_OFFSET;
	}

	InvalidateVisibleRect();
}

void BackgroundImageCanvas::DrawBackground(wxDC& dc, bool fromPaint) {
	dc.SetUserScale(m_bgScale, m_bgScale);
	dc.DrawBitmap(m_background, m_bgx + m_bgxoffset, m_bgy + m_bgyoffset);
	dc.SetUserScale(1.0, 1.0);
}

void BackgroundImageCanvas::OnUpdateVirtualSize(wxRect& rect) {
	rect.SetTopLeft(wxPoint(0, 0));
	rect.SetSize(GetClientSize());
}

void BackgroundImageCanvas::OnMouseMove(wxMouseEvent& event) {
	wxSize size = GetClientSize();
	wxPoint pos = event.GetPosition();

	m_bgxoffset = (double)((m_mousePosOnEnter.x - pos.x) * MAX_BG_OFFSET) / size.x;
	m_bgyoffset = (double)((m_mousePosOnEnter.y - pos.y) * MAX_BG_OFFSET) / size.y;

	InvalidateVisibleRect();
	wxSFShapeCanvas::OnMouseMove(event);
}

void BackgroundImageCanvas::OnEnterWindow(wxMouseEvent& event) {
	m_bgAnimTimer.Stop();
	m_mousePosOnEnter = event.GetPosition();
}

void BackgroundImageCanvas::OnLeaveWindow(wxMouseEvent& event) {
	m_bgAnimTimer.Start(1);
}

void BackgroundImageCanvas::OnBgAnimTimer(wxTimerEvent& event) {
	if (m_bgxoffset > 1)
		m_bgxoffset -= 2;
	else if (m_bgxoffset < -1)
		m_bgxoffset += 2;

	if (m_bgyoffset > 1)
		m_bgyoffset -= 2;
	else if (m_bgyoffset < -1)
		m_bgyoffset += 2;

	if ((m_bgxoffset > -2 && m_bgxoffset < 2) && (m_bgyoffset > -2 && m_bgyoffset < 2)) {
		m_bgxoffset = 0;
		m_bgyoffset = 0;
		m_bgAnimTimer.Stop();
	}

	Refresh();
	Update();
}

void BackgroundImageCanvas::_OnSize(wxSizeEvent& event) {
	this->OnSize(event);
	event.Skip();
}
