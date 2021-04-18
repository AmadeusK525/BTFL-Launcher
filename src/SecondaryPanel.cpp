#include "SecondaryPanel.h"
#include "Scrollbar.h"

DisclaimerPanel::DisclaimerPanel(SecondaryPanel* parent,
	wxWindowID id,
	const wxString& value,
	const wxPoint& pos,
	const wxSize& size,
	long style) : ReadOnlyRTC(parent, id, value, pos, size, style) {
	m_seconPanel = parent;
}

void DisclaimerPanel::PaintBackground(wxDC& dc) {
	double scale = m_seconPanel->GetBackgroundScale();
	int yo;
	CalcUnscrolledPosition(0, 0, nullptr, &yo);

	dc.SetUserScale(scale, scale);
	dc.DrawBitmap(m_seconPanel->GetBackgroundBitmap(), m_bgx, m_bgy + ((double)yo / scale));
	dc.SetUserScale(1.0, 1.0);
}


/////////////////////////////////////////////////////////////////////////
//////////////////////////// SecondaryPanel /////////////////////////////
/////////////////////////////////////////////////////////////////////////


SecondaryPanel::SecondaryPanel(wxSFDiagramManager* manager, wxWindow* parent) : BackgroundImageCanvas(manager, parent, -1) {
	MAX_BG_OFFSET = 0;
	m_background.LoadFile("Assets\\Background\\Subpage@2x.png", wxBITMAP_TYPE_PNG);
	m_bgRatio = (double)m_background.GetWidth() / m_background.GetHeight();

	m_topSeparator.LoadFile("Assets\\Spacer\\Full Window Width@2x.png", wxBITMAP_TYPE_PNG);
	m_frameButtons = (FrameButtons*)manager->AddShape(CLASSINFO(FrameButtons), false);
	m_frameButtons->Init();

	m_disclaimer = new DisclaimerPanel(this, -1, "", wxDefaultPosition, wxSize(700, -1));
	CustomRTCScrollbar* scrollbar = new CustomRTCScrollbar(this, m_disclaimer, -1);

	wxRichTextBuffer buf;
	buf.LoadFile("Assets\\Temp.xml", wxRICHTEXT_TYPE_XML);
	buf.SetBasicStyle(m_disclaimer->GetBasicStyle());
	m_disclaimer->GetBuffer() = buf;
	m_disclaimer->Invalidate();

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddStretchSpacer(1);
	sizer->Add(m_disclaimer, wxSizerFlags(0).Expand().Border(wxTOP, 50));
	sizer->AddStretchSpacer(1);
	sizer->Add(scrollbar, wxSizerFlags(0).Expand().Border(wxTOP, 50));
	sizer->AddSpacer(20);

	SetSizer(sizer);
}

void SecondaryPanel::ShowDisclaimer() {}

void SecondaryPanel::ShowSettings() {}

void SecondaryPanel::RepositionAll() {
	wxSize size = GetClientSize();
	m_frameButtons->MoveTo(size.x - m_frameButtons->GetRectSize().x - 10, 10);
}

void SecondaryPanel::DrawForeground(wxDC& dc, bool fromPaint) {
	dc.SetUserScale(m_bgScale, m_bgScale);
	dc.DrawBitmap(m_topSeparator, 0, 49.0 / m_bgScale, true);
	dc.SetUserScale(1.0, 1.0);
}

void SecondaryPanel::OnSize(wxSizeEvent& event) {
	BackgroundImageCanvas::OnSize(event);
	wxPoint disclaimerPos = m_disclaimer->GetPosition();
	
	// For some reason I need to offset the x component by 15 so it statys aligned.
	// Might be a quirk with how wxRTC draws itself
	m_disclaimer->SetBackgroundX(m_bgx - 15 - ((double)disclaimerPos.x / m_bgScale));
	m_disclaimer->SetBackgroundY(m_bgy - ((double)disclaimerPos.y / m_bgScale));

	RepositionAll();
}