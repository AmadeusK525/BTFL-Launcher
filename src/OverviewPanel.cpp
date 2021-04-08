#include "OverviewPanel.h"

#include <wx\dcbuffer.h>
#include <wx\dcgraph.h>

BEGIN_EVENT_TABLE(OverviewPanel, wxPanel)
EVT_SIZE(OverviewPanel::OnSize)
EVT_PAINT(OverviewPanel::OnPaint)
END_EVENT_TABLE()

OverviewPanel::OverviewPanel(wxWindow* parent) : wxPanel(parent) {
	m_background = wxBITMAP_PNG(pabloRA-Phoenix);
	SetBackgroundColour(wxColour(10, 10, 10));
}

void OverviewPanel::OnSize(wxSizeEvent& event) {
	wxSize size = GetClientSize();
	m_scale = (double)size.x / m_background.GetWidth();
	m_bgy = (size.y - ((double)m_background.GetHeight() * m_scale)) / 2;
	Refresh();
}

void OverviewPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	dc.SetUserScale(m_scale, m_scale);
	dc.DrawBitmap(m_background, 0, m_bgy / m_scale);
}
