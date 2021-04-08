#include "OverviewPanel.h"

#include <wx\dcbuffer.h>
#include <wx\dcgraph.h>

BEGIN_EVENT_TABLE(OverviewPanel, wxPanel)
EVT_SIZE(OverviewPanel::OnSize)
EVT_PAINT(OverviewPanel::OnPaint)
END_EVENT_TABLE()

OverviewPanel::OverviewPanel(wxWindow* parent) : wxPanel(parent) {
	m_background = wxBITMAP_PNG(pabloRA-Phoenix);
	m_bgRatio = (double)m_background.GetWidth() / m_background.GetHeight();
	SetBackgroundColour(wxColour(10, 10, 10));
	SetMinSize(FromDIP(wxSize(350, 250)));
}

void OverviewPanel::OnSize(wxSizeEvent& event) {
	wxSize size = GetClientSize();
	double curRatio = (double)size.x / size.y;

	if (curRatio > m_bgRatio) {
		m_bgScale = (double)size.x / m_background.GetWidth();
		m_bgy = (size.y - ((double)m_background.GetHeight() * m_bgScale)) / 2;
		m_bgx = 0;
	} else {
		m_bgScale = (double)size.y / m_background.GetHeight();
		m_bgx = (size.x - ((double)m_background.GetWidth() * m_bgScale)) / 2;
		m_bgy = 0;
	}

	Refresh();
}

void OverviewPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	dc.SetUserScale(m_bgScale, m_bgScale);
	dc.DrawBitmap(m_background, m_bgx / m_bgScale, m_bgy / m_bgScale);
}
