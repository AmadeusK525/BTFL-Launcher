#include "OverviewPanel.h"

BEGIN_EVENT_TABLE(OverviewPanel, wxCustomBackgroundWindow<wxPanel>)
EVT_SIZE(OverviewPanel::OnSize)
END_EVENT_TABLE()

OverviewPanel::OverviewPanel(wxWindow* parent) {
	Create(parent);
	
	wxBitmap bmp(wxBITMAP_PNG(pabloRA-Phoenix));
	m_background = bmp.ConvertToImage();
	m_bgRatio = (double)m_background.GetWidth() / m_background.GetHeight();
	bmp = m_background;

	SetBackgroundBitmap(bmp);
}

void OverviewPanel::OnSize(wxSizeEvent& event) {
	wxSize size = GetVirtualSize();
	SetBackgroundBitmap(amGetScaledImage(size.x, size.y, m_background, m_bgRatio));
	Refresh();
}
