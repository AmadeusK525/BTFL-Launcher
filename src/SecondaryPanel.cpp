#include "SecondaryPanel.h"
#include "Scrollbar.h"
#include "MyApp.h"

DisclaimerPanel::DisclaimerPanel(SecondaryPanel* parent,
	wxWindowID id,
	const wxString& value,
	const wxPoint& pos,
	const wxSize& size,
	long style) : ReadOnlyRTC(parent, id, value, pos, size, style)
{
	m_seconPanel = parent;
}

void DisclaimerPanel::PaintBackground(wxDC& dc)
{
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


wxBEGIN_EVENT_TABLE(SecondaryPanel, BackgroundImageCanvas)

EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Help, SecondaryPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Minimize, SecondaryPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Close, SecondaryPanel::OnFrameButtons)

wxEND_EVENT_TABLE()

SecondaryPanel::SecondaryPanel(wxSFDiagramManager* manager, MainFrame* parent) :
	BackgroundImageCanvas(manager, (wxWindow*)parent, -1)
{
	m_mainFrame = parent;

	MAX_BG_OFFSET = 0;
	m_background.LoadFile("Assets\\Background\\Subpage@2x.png", wxBITMAP_TYPE_PNG);
	m_bgRatio = (double)m_background.GetWidth() / m_background.GetHeight();

	manager->AcceptShape("All");

	m_topSeparator.LoadFile("Assets\\Spacer\\Full Window Width@2x.png", wxBITMAP_TYPE_PNG);

	m_backArrow = (wxSFBitmapShape*)manager->AddShape(CLASSINFO(wxSFBitmapShape), false);
	m_backArrow->CreateFromFile("Assets\\Icon\\Arrow Left.png", wxBITMAP_TYPE_PNG);
	m_backArrow->SetId(BUTTON_Back);
	m_backArrow->SetStyle(
		wxSFShapeBase::STYLE::sfsHOVERING |
		wxSFShapeBase::STYLE::sfsEMIT_EVENTS
	);

	m_title = (wxSFTextShape*)manager->AddShape(CLASSINFO(wxSFTextShape), false);
	m_title->SetText("DISCLAIMER");
	m_title->SetTextColour(wxColour(255, 255, 255));
	m_title->SetFont(wxFontInfo(15).Bold().FaceName("Times New Roman"));
	m_title->SetStyle(0);

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
	sizer->Add(m_disclaimer, wxSizerFlags(0).Expand().Border(wxTOP, TOP_SPACE));
	sizer->AddStretchSpacer(1);
	sizer->Add(scrollbar, wxSizerFlags(0).Expand().Border(wxTOP, TOP_SPACE));
	sizer->AddSpacer(20);

	SetSizer(sizer);
}

void SecondaryPanel::ShowDisclaimer() {}

void SecondaryPanel::ShowSettings() {}

void SecondaryPanel::OnFrameButtons(wxSFShapeMouseEvent& event)
{
	switch ( event.GetId() )
	{
	case BUTTON_Help:
		break;

	case BUTTON_Minimize:
		((wxFrame*)wxGetApp().GetTopWindow())->Iconize();
		break;

	case BUTTON_Close:
		wxGetApp().GetTopWindow()->Close();
		break;
	}
}

void SecondaryPanel::RepositionAll()
{
	wxSize size = GetClientSize();
	wxRealPoint shapeSize;

	shapeSize = m_frameButtons->GetRectSize();
	m_frameButtons->MoveTo(size.x - shapeSize.x - 10, 10);

	shapeSize = m_title->GetRectSize();
	m_title->MoveTo((size.x / 2) - (shapeSize.x / 2), (TOP_SPACE / 2) - (shapeSize.y / 2));

	shapeSize = m_backArrow->GetRectSize();
	m_backArrow->MoveTo(10, 10);
}

void SecondaryPanel::DrawForeground(wxDC& dc, bool fromPaint)
{
	dc.SetUserScale(m_bgScale, m_bgScale);
	dc.DrawBitmap(m_topSeparator, 0, 49.0 / m_bgScale, true);
	dc.SetUserScale(1.0, 1.0);
}

void SecondaryPanel::OnSize(wxSizeEvent& event)
{
	BackgroundImageCanvas::OnSize(event);
	wxPoint disclaimerPos = m_disclaimer->GetPosition();

	// For some reason I need to offset the x component by 15 so it statys aligned.
	// Might be a quirk with how wxRTC draws itself
	m_disclaimer->SetBackgroundX(m_bgx - 15 - ((double)disclaimerPos.x / m_bgScale));
	m_disclaimer->SetBackgroundY(m_bgy - ((double)disclaimerPos.y / m_bgScale));

	RepositionAll();
}