#include "OverviewPanel.h"

#include <wx\dcbuffer.h>
#include <wx\dcgraph.h>

BEGIN_EVENT_TABLE(OverviewPanel, wxPanel)

EVT_SIZE(OverviewPanel::OnSize)
EVT_PAINT(OverviewPanel::OnPaint)

EVT_LEFT_DCLICK(OverviewPanel::OnDoubleClick)

END_EVENT_TABLE()

OverviewPanel::OverviewPanel(wxWindow* parent) : wxPanel(parent) {
	m_background = wxBITMAP_PNG(pabloRA-Phoenix);
	m_bgRatio = (double)m_background.GetWidth() / m_background.GetHeight();
	
	SetMinSize(FromDIP(wxSize(350, 250)));

	m_patchNotesWindow = new PatchNotesWindow(this, -1, wxDefaultPosition, wxDefaultSize, wxRETAINED | wxCLIP_CHILDREN);
	m_patchNotesWindow->SetScrollRate(15, 15);
	m_patchNotesWindow->SetVirtualSize(200, 2000);
	m_patchNotesWindow->EnableScrolling(false, false);
	m_patchNotesWindow->SetBackgroundStyle(wxBG_STYLE_PAINT);

	m_sizer = new wxBoxSizer(wxHORIZONTAL);
	m_sizer->Add(m_patchNotesWindow, wxSizerFlags(2).Expand().Border(wxALL, 20));
	m_sizer->AddStretchSpacer(3);

	SetSizer(m_sizer);
}

void OverviewPanel::OnSize(wxSizeEvent& event) {
	wxSize size = GetClientSize();
	double curRatio = (double)size.x / size.y;

	if (curRatio > m_bgRatio) {
		m_bgScale = (double)size.x / m_background.GetWidth();
		m_bgy = ((size.y - ((double)m_background.GetHeight() * m_bgScale)) / 2) / m_bgScale;
		m_bgx = 0;
	} else {
		m_bgScale = (double)size.y / m_background.GetHeight();
		m_bgx = ((size.x - ((double)m_background.GetWidth() * m_bgScale)) / 2) / m_bgScale;
		m_bgy = 0;
	}

	m_patchNotesWindow->SetBackgroundX(m_bgx - ((double)20 / m_bgScale));
	m_patchNotesWindow->SetBackgroundY(m_bgy - ((double)20 / m_bgScale));
	m_patchNotesWindow->SetBackgroundScale(m_bgScale);
	Refresh();
	event.Skip();
}

void OverviewPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);

	dc.SetUserScale(m_bgScale, m_bgScale);
	dc.DrawBitmap(m_background, m_bgx, m_bgy);
}

void OverviewPanel::OnDoubleClick(wxMouseEvent& event) {
	m_patchNotesWindow->DoReposition(true, false);
}
