#include "SecondaryPanel.h"
#include "Scrollbar.h"
#include "MainFrame.h"

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

	wxSize virtualSize = GetVirtualSize();
	int halfx = virtualSize.x / 2;

	wxColour grey(150, 150, 150);
	dc.GradientFillLinear(wxRect(0, virtualSize.y - 1, halfx, 1), *wxBLACK, grey);
	dc.GradientFillLinear(wxRect(halfx, virtualSize.y - 1, halfx, 1), grey, *wxBLACK);
}


/////////////////////////////////////////////////////////////////////////
//////////////////////////// SecondaryPanel /////////////////////////////
/////////////////////////////////////////////////////////////////////////


wxBEGIN_EVENT_TABLE(SecondaryPanel, BackgroundImageCanvas)

EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Help, SecondaryPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Minimize, SecondaryPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Close, SecondaryPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Back, SecondaryPanel::OnFrameButtons)

wxEND_EVENT_TABLE()

SecondaryPanel::SecondaryPanel(wxSFDiagramManager* manager, MainFrame* parent) :
	BackgroundImageCanvas(manager, parent, -1)
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
	m_title->SetTextColour(wxColour(255, 255, 255));
	m_title->SetFont(wxFontInfo(15).Bold().FaceName("Times New Roman"));
	m_title->SetStyle(0);

	m_frameButtons = (FrameButtons*)manager->AddShape(CLASSINFO(FrameButtons), false);
	m_frameButtons->Init();

	m_verSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(m_verSizer);
}

void SecondaryPanel::ShowDisclaimer() 
{
	m_title->SetText("DISCLAIMER AGREEMENT");
	
	if ( !m_disclaimer )
	{
		m_disclaimer = new DisclaimerPanel(this, -1, "", wxDefaultPosition, wxSize(700, -1));
		CustomRTCScrollbar* scrollbar = new CustomRTCScrollbar(this, m_disclaimer, -1);

		wxRichTextBuffer buf;
		buf.LoadFile("Assets\\Temp.xml", wxRICHTEXT_TYPE_XML);
		buf.SetBasicStyle(m_disclaimer->GetBasicStyle());
		m_disclaimer->GetBuffer() = buf;
		m_disclaimer->Invalidate();

		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->AddStretchSpacer(1);
		sizer->Add(m_disclaimer, wxSizerFlags(0).Expand());
		sizer->AddStretchSpacer(1);
		sizer->Add(scrollbar, wxSizerFlags(0).Expand());
		sizer->AddSpacer(20);

		m_verSizer->Clear(true);
		m_verSizer->AddSpacer(TOP_SPACE);
		m_verSizer->Add(sizer, wxSizerFlags(1).Expand());
	}

	if ( !m_disDecline && !m_mainFrame->HasUserAgreedToDisclaimer() )
	{
		m_verSizer->AddSpacer(BOTTOM_SPACE);
		wxSFDiagramManager* manager = GetDiagramManager();

		m_disDecline = new TransparentButton("DECLINE", wxDefaultPosition, wxDefaultPosition, 3.0, manager);
		m_disDecline->SetId(BUTTON_DisclaimerDecline);
		m_disDecline->SetFont(wxFontInfo(20).FaceName("Times New Roman"));
		manager->AddShape(m_disDecline, nullptr, wxDefaultPosition, true, false);

		m_disAgree = new TransparentButton("ACCEPT & VERIFY", wxDefaultPosition, wxDefaultPosition, 3.0, manager);
		m_disAgree->SetId(BUTTON_DisclaimerAgree);
		m_disAgree->SetFill(wxBrush(wxColour(255, 255, 255)));
		m_disAgree->SetTextColour(wxColour(0, 0, 0));
		m_disAgree->SetFont(wxFontInfo(20).FaceName("Times New Roman"));
		manager->AddShape(m_disAgree, nullptr, wxDefaultPosition, true, false);
	}

	Layout();
	SendSizeEvent();
	RepositionAll();
}

void SecondaryPanel::ShowSettings()
{
	m_title->SetText("SETTINGS");

	if ( m_disclaimer )
	{
		m_verSizer->Clear(true);
		m_disclaimer = nullptr;

		if ( m_disDecline )
		{
			wxSFDiagramManager* manager = GetDiagramManager();
			manager->RemoveShape(m_disDecline, false);
			manager->RemoveShape(m_disAgree, true);
			m_disDecline = nullptr;
			m_disAgree = nullptr;
		}
	}

	Layout();
	SendSizeEvent();
	RepositionAll();
}

void SecondaryPanel::OnFrameButtons(wxSFShapeMouseEvent& event)
{
	switch ( event.GetId() )
	{
	case BUTTON_Help:
		break;

	case BUTTON_Minimize:
		m_mainFrame->Iconize();
		break;

	case BUTTON_Close:
		m_mainFrame->Close();
		break;

	case BUTTON_Back:
		m_mainFrame->ShowMainPanel();
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

	if ( m_disclaimer )
	{
		wxRect disclaimerRect = m_disclaimer->GetRect();

		if ( m_disDecline )
		{
			int yButtonHalf = disclaimerRect.GetBottom() + ((size.y - disclaimerRect.GetBottom()) / 2);

			m_disDecline->RecalculateSelf();
			shapeSize = m_disDecline->GetRectSize();
			m_disDecline->MoveTo(disclaimerRect.x, yButtonHalf - (shapeSize.y / 2));

			m_disAgree->RecalculateSelf();
			shapeSize = m_disAgree->GetRectSize();
			m_disAgree->MoveTo(disclaimerRect.GetRight() - shapeSize.x, yButtonHalf - (shapeSize.y / 2));
		}
	}
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

	if ( m_disclaimer )
	{
		wxPoint disclaimerPos = m_disclaimer->GetPosition();

		// For some reason I need to offset the x component by 15 so it statys aligned.
		// Might be a quirk with how wxRTC draws itself
		m_disclaimer->SetBackgroundX(m_bgx - ((double)disclaimerPos.x / m_bgScale));
		m_disclaimer->SetBackgroundY(m_bgy - ((double)disclaimerPos.y / m_bgScale));
	}

	RepositionAll();
}