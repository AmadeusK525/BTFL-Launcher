#include "MainPanel.h"

XS_IMPLEMENT_CLONABLE_CLASS(TransparentButton, wxSFRoundRectShape);

TransparentButton::TransparentButton(const wxString& label, const wxRealPoint& pos, const wxRealPoint& size,
	double radius, wxSFDiagramManager* manager) :
	wxSFRoundRectShape(pos, size, radius, manager) {
	SetFill(wxBrush(wxColour(255, 255, 255, 50)));
	SetBorder(*wxTRANSPARENT_PEN);
	
	RemoveStyle(sfsHIGHLIGHTING);
	RemoveStyle(sfsPOSITION_CHANGE);
	RemoveStyle(sfsSIZE_CHANGE);
	RemoveStyle(sfsSHOW_HANDLES);

	m_label = label;
}

TransparentButton::TransparentButton(const TransparentButton& other) {

}

void TransparentButton::SetLabel(const wxString& label) {
	m_label = label;
	RecalculateSelf();
}

void TransparentButton::SetFont(const wxFont& font) {
	m_font = font;
	RecalculateSelf();
}

void TransparentButton::RecalculateSelf() {
	MainPanel* pCanvas = dynamic_cast<MainPanel*>(GetParentCanvas());
	if (!pCanvas)
		return;

	wxClientDC dc(pCanvas);
	dc.SetFont(m_font);
	wxSize size = dc.GetMultiLineTextExtent(m_label);
	m_xLabel = m_xPadding;

	if (m_bitmap) {
		if (!m_label.IsEmpty()) {

		} else {

		}
	}

	SetRectSize(size.x + (m_xPadding * 2), size.y + (m_yPadding * 2));
}

void TransparentButton::DrawLabel(wxDC& dc) {
	dc.SetFont(m_font);
	dc.SetTextForeground(m_textColour);

	dc.DrawText(m_label, GetAbsolutePosition() + wxRealPoint(m_xPadding, m_yPadding));
}

void TransparentButton::DrawNormal(wxDC& dc) {
	wxSFRoundRectShape::DrawNormal(dc);
	DrawLabel(dc);
}

void TransparentButton::DrawHover(wxDC& dc) {
	wxSFRoundRectShape::DrawHover(dc);
	DrawLabel(dc);
}

void TransparentButton::DrawHighlighted(wxDC& dc) {
	wxSFRoundRectShape::DrawHighlighted(dc);
	DrawLabel(dc);
}


/////////////////////////////////////////////////////////////////////
//////////////////////////// MainPanel //////////////////////////////
/////////////////////////////////////////////////////////////////////


wxBEGIN_EVENT_TABLE(MainPanel, wxSFShapeCanvas)

EVT_SIZE(MainPanel::OnSize)
EVT_TIMER(1234, MainPanel::OnBgAnimTimer)

EVT_ENTER_WINDOW(MainPanel::OnEnterWindow)
EVT_LEAVE_WINDOW(MainPanel::OnLeaveWindow)

wxEND_EVENT_TABLE()

MainPanel::MainPanel(wxSFDiagramManager* manager,
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style) : wxSFShapeCanvas(manager, parent, id, pos, size, style), m_bgAnimTimer(this, 1234) {
	m_background = wxBITMAP_PNG(pabloRA_Roc);
	m_logo = wxBITMAP_PNG(logo);
	m_bgRatio = (double)m_background.GetWidth() / m_background.GetHeight();

	manager->AcceptShape("TransparentButton");

	EnableGC(true);
	SetHoverColour(wxColour(255, 255, 255));

	m_mainButton = new TransparentButton("INSTALL", wxDefaultPosition, wxDefaultPosition, 1.0, manager);
	m_mainButton->SetFont(wxFontInfo(20).FaceName("Times New Roman"));
	manager->AddShape(m_mainButton, nullptr, wxDefaultPosition, true);

	SetScale(1.0);
	RemoveStyle(sfsMULTI_SELECTION);
	RemoveStyle(sfsPROCESS_MOUSEWHEEL);
}

void MainPanel::RepositionAll() {
	wxSize size = GetClientSize();
	wxRealPoint shapeSize = m_mainButton->GetRectSize();

	m_mainButton->RecalculateSelf();
	m_mainButton->MoveTo(40, size.y - shapeSize.y - 40);
}

void MainPanel::OnUpdateVirtualSize(wxRect& rect) {
	rect.SetTopLeft(wxPoint(0, 0));
	rect.SetSize(GetClientSize());
}

void MainPanel::DrawBackground(wxDC& dc, bool fromPaint) {
	dc.SetUserScale(m_bgScale, m_bgScale);
	dc.DrawBitmap(m_background, m_bgx + m_bgxoffset, m_bgy + m_bgyoffset);
	dc.SetUserScale(1.0, 1.0);

	dc.DrawBitmap(m_logo, m_logox, m_logoy);
}

void MainPanel::OnSize(wxSizeEvent& event) {
	wxSize size = GetSize();
	double curRatio = (double)size.x / size.y;
	int overallOffset = MAX_BG_OFFSET * 2;

	if (curRatio > m_bgRatio) {
		m_bgScale = (double)(size.x + overallOffset) / m_background.GetWidth();
		m_bgy = (((size.y - overallOffset) - ((double)m_background.GetHeight() * m_bgScale)) / 2) / m_bgScale;
		m_bgx = -MAX_BG_OFFSET;
	} else {
		m_bgScale = (double)(size.y + overallOffset) / m_background.GetHeight();
		m_bgx = (((size.x - overallOffset) - ((double)m_background.GetWidth() * m_bgScale)) / 2) / m_bgScale;
		m_bgy = -MAX_BG_OFFSET;
	}

	m_logox = (size.x / 2) - (m_logo.GetWidth() / 2);
	m_logoy = 70;

	RepositionAll();
	Refresh();
	event.Skip();
}

void MainPanel::OnMouseMove(wxMouseEvent& event) {
	wxSFShapeCanvas::OnMouseMove(event);

	wxPoint pos = wxGetMousePosition();
	wxSize size = GetClientSize();
	
	m_bgxoffset = (double)(( m_mousePosOnEnter.x - pos.x) * MAX_BG_OFFSET) / size.x;
	m_bgyoffset = (double)((m_mousePosOnEnter.y - pos.y) * MAX_BG_OFFSET) / size.y;

	if (m_bgxoffset > MAX_BG_OFFSET)
		m_bgxoffset = MAX_BG_OFFSET;
	else if (m_bgxoffset < -MAX_BG_OFFSET)
		m_bgxoffset = -MAX_BG_OFFSET;

	if (m_bgyoffset > MAX_BG_OFFSET)
		m_bgyoffset = MAX_BG_OFFSET;
	else if (m_bgyoffset < -MAX_BG_OFFSET)
		m_bgyoffset = -MAX_BG_OFFSET;

	Refresh();
}

void MainPanel::OnEnterWindow(wxMouseEvent& event) {
	m_mousePosOnEnter = wxGetMousePosition();
	event.Skip();
}

void MainPanel::OnLeaveWindow(wxMouseEvent& event) {
	m_bgAnimTimer.Start(1);
	event.Skip();
}

void MainPanel::OnBgAnimTimer(wxTimerEvent& event) {
	if (m_bgxoffset > 0)
		m_bgxoffset--;
	else if (m_bgxoffset < 0)
		m_bgxoffset++;

	if (m_bgyoffset > 0)
		m_bgyoffset--;
	else if (m_bgyoffset < 0)
		m_bgyoffset++;

	if (m_bgxoffset == 0 && m_bgyoffset == 0)
		m_bgAnimTimer.Stop();

	Refresh();
	Update();

}
