#include "MainPanel.h"

#include "MainFrame.h"
#include "MyApp.h"

#include <wx\filename.h>

XS_IMPLEMENT_CLONABLE_CLASS(TransparentButton, wxSFRoundRectShape);

TransparentButton::TransparentButton(const wxString& label, const wxRealPoint& pos, const wxRealPoint& size,
	double radius, wxSFDiagramManager* manager) :
	wxSFRoundRectShape(pos, size, radius, manager) {
	SetFill(wxBrush(wxColour(255, 255, 255, 50)));
	SetBorder(*wxTRANSPARENT_PEN);
	
	AddStyle(sfsEMIT_EVENTS);

	RemoveStyle(sfsHIGHLIGHTING);
	RemoveStyle(sfsPOSITION_CHANGE);
	RemoveStyle(sfsSIZE_CHANGE);
	RemoveStyle(sfsSHOW_HANDLES);

	m_label = label;
}

TransparentButton::TransparentButton(const TransparentButton& other) {

}

TransparentButton::~TransparentButton() {
	if (m_bitmap)
		delete m_bitmap;
}

void TransparentButton::Enable(bool enable) {
	if (enable) {
		AddStyle(sfsEMIT_EVENTS);
		AddStyle(sfsHOVERING);
	} else {
		RemoveStyle(sfsEMIT_EVENTS);
		RemoveStyle(sfsHOVERING);
	}

	m_isEnabled = enable;
}

void TransparentButton::SetLabel(const wxString& label) {
	m_label = label;
	RecalculateSelf();
}

void TransparentButton::SetFont(const wxFont& font) {
	m_font = font;
	RecalculateSelf();
}

void TransparentButton::SetBitmap(const wxBitmap& bmp) {
	if (!bmp.IsOk()) {
		if (m_bitmap) {
			delete m_bitmap;
			m_bitmap = nullptr;
		}

		return;
	}

	if (!m_bitmap)
		m_bitmap = new wxBitmap(bmp);
	else
		*m_bitmap = bmp;

	m_bmpRatio = (double)bmp.GetWidth() / bmp.GetHeight();
}

void TransparentButton::SetPadding(int x, int y) {
	m_xPadding = x;
	m_yPadding = y;
}

void TransparentButton::GetPadding(int* x, int* y) {
	if (x) *x = m_xPadding;
	if (y) *y = m_yPadding;
}

void TransparentButton::RecalculateSelf(const wxSize& soloBmpSize) {
	MainPanel* pCanvas = dynamic_cast<MainPanel*>(GetParentCanvas());
	if (!pCanvas)
		return;

	wxClientDC dc(pCanvas);
	dc.SetFont(m_font);
	wxSize size = dc.GetMultiLineTextExtent(m_label);
	m_xLabel = m_xPadding;

	if (m_bitmap) {
		if (m_label.IsEmpty()) {
			if (soloBmpSize != wxDefaultSize) {
				double curRatio = (double)soloBmpSize.x / soloBmpSize.y;
				if (curRatio > m_bmpRatio) {
					m_bmpScale = (double)soloBmpSize.y / m_bitmap->GetHeight();
					m_xBitmap = (soloBmpSize.x / 2) - ((double)(m_bitmap->GetWidth() * m_bmpScale) / 2) + m_xPadding ;
					m_yBitmap = m_yPadding;
				} else {
					m_bmpScale = (double)soloBmpSize.x / m_bitmap->GetWidth();
					m_yBitmap = (soloBmpSize.y / 2) - (((double)m_bitmap->GetHeight() * m_bmpScale) / 2) + m_yPadding;
					m_xBitmap = m_xPadding;
				}

				size.x = (double)m_bitmap->GetWidth() * m_bmpScale;
				size.y = (double)m_bitmap->GetHeight() * m_bmpScale;
			}
		} else {
			m_bmpScale = (double)size.y / m_bitmap->GetHeight();
			m_yBitmap = m_yPadding;
			m_xBitmap = m_xPadding;

			int bmpx = (double)m_bitmap->GetWidth() * m_bmpScale;
			size.x += bmpx + 5;
			m_xLabel += bmpx + 5;
		}
	}

	SetRectSize(size.x + (m_xPadding * 2), size.y + (m_yPadding * 2));
}

void TransparentButton::DrawContent(wxDC& dc, bool isHovering) {
	dc.SetFont(m_font);
	dc.SetTextForeground(m_textColour);
	wxRealPoint absolutePos = GetAbsolutePosition();

	dc.DrawText(m_label, absolutePos + wxRealPoint(m_xLabel, m_yPadding));

	if (m_bitmap) {
		dc.SetUserScale(m_bmpScale, m_bmpScale);
		wxRealPoint bmpPos = (absolutePos + wxRealPoint(m_xBitmap, m_yBitmap));
		bmpPos.x /= m_bmpScale;
		bmpPos.y /= m_bmpScale;

		if (isHovering)
			dc.DrawBitmap(m_bitmap->ConvertToDisabled(0), bmpPos, true);
		else
			dc.DrawBitmap(*m_bitmap, bmpPos, true);

		dc.SetUserScale(1.0, 1.0);
	}
}

void TransparentButton::DrawNormal(wxDC& dc) {
	wxSFRoundRectShape::DrawNormal(dc);
	DrawContent(dc, false);
}

void TransparentButton::DrawHover(wxDC& dc) {
	wxBrush fillCache = GetFill();
	wxColour textColourCache = m_textColour;

	SetFill(wxBrush(wxColour(230, 230, 230)));
	m_textColour = { 0, 0, 0 };

	wxSFRoundRectShape::DrawHover(dc);
	DrawContent(dc, true);

	SetFill(fillCache);
	m_textColour = textColourCache;
}


/////////////////////////////////////////////////////////////////////
//////////////////////////// MainPanel //////////////////////////////
/////////////////////////////////////////////////////////////////////


wxBEGIN_EVENT_TABLE(MainPanel, wxSFShapeCanvas)

EVT_SF_SHAPE_LEFT_DOWN(BUTTON_SelectIso, MainPanel::OnSelectIso)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Help, MainPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Minimize, MainPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Close, MainPanel::OnFrameButtons)

wxEND_EVENT_TABLE()

MainPanel::MainPanel(wxSFDiagramManager* manager,
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style) : BackgroundImageCanvas(manager, parent, id, pos, size, style) {
	m_background.LoadFile("Assets\\Background\\Main Page Roc@2x.png", wxBITMAP_TYPE_PNG);
	m_logo.LoadFile("Assets\\LauncherLogo.png", wxBITMAP_TYPE_PNG);
	m_bgRatio = (double)m_background.GetWidth() / m_background.GetHeight();

	m_fileContainer.LoadFile("Assets\\Containers\\FilePath@2x.png", wxBITMAP_TYPE_PNG);
	m_fileBmp.LoadFile("Assets\\Icon\\Browse@2x.png", wxBITMAP_TYPE_PNG);
	m_fileBmpScale = (double)216 / m_fileContainer.GetWidth();

	manager->AcceptShape("TransparentButton");

	m_mainButton = new TransparentButton("SELECT ISO", wxDefaultPosition, wxDefaultPosition, 3.0, manager);
	m_mainButton->SetId(BUTTON_SelectIso);
	m_mainButton->SetFont(wxFontInfo(20).FaceName("Times New Roman"));
	m_mainButton->SetBitmap(wxBitmap("Assets\\Icon\\Verify@2x.png", wxBITMAP_TYPE_PNG));
	manager->AddShape(m_mainButton, nullptr, wxDefaultPosition, true, false);

	m_configButton = new TransparentButton("", wxDefaultPosition, wxDefaultPosition, 3.0, manager);
	m_configButton->SetBitmap(wxBitmap("Assets\\Icon\\Settings@2x.png", wxBITMAP_TYPE_PNG));
	m_configButton->SetPadding(15, 15);
	manager->AddShape(m_configButton, nullptr, wxDefaultPosition, true, false);
	
	m_frameButtons = (FrameButtons*)manager->AddShape(CLASSINFO(FrameButtons), false);
	m_frameButtons->Init();
}

void MainPanel::RepositionAll() {
	wxSize size = GetClientSize();

	m_mainButton->RecalculateSelf();
	wxRealPoint shapeSize = m_mainButton->GetRectSize();
	
	m_mainButton->MoveTo(40, size.y - shapeSize.y - 40);
	wxRect shapeBB = m_mainButton->GetBoundingBox();

	int yPadding;
	m_mainButton->GetPadding(nullptr, &yPadding);
	m_configButton->RecalculateSelf(wxSize(shapeSize.y - (yPadding * 2), shapeSize.y - (yPadding * 2)));
	m_configButton->MoveTo(shapeBB.GetTopRight() + wxPoint(15, 0));

	shapeSize = m_frameButtons->GetRectSize();
	m_frameButtons->MoveTo(size.x - shapeSize.x - 10, 10);

	m_xFCont = (double)shapeBB.GetLeft() / m_fileBmpScale;
	m_yFCont = (double)(shapeBB.GetTop() - ((double)m_fileContainer.GetHeight() * m_fileBmpScale) - 10) / m_fileBmpScale;

	m_xFBmp = m_xFCont + 20;
	m_yFBmp = m_yFCont + (((double)m_fileBmp.GetHeight() * m_fileBmpScale) / 2);

	m_xFLabel = (double)(m_xFBmp + m_fileBmp.GetWidth() + 10) * m_fileBmpScale;
	m_yFLabel = ((double)m_yFCont * m_fileBmpScale) + 5;

	wxClientDC dc(this);
	dc.SetFont(m_fileDescFont);
	wxSize textSize = dc.GetTextExtent(m_fileDesc);

	m_xFDesc = m_xFLabel;
	m_yFDesc = ((double)(m_yFCont + m_fileContainer.GetHeight()) * m_fileBmpScale) - textSize.y - 5;
	m_fileDescRect = { wxPoint(m_xFDesc, m_yFDesc), textSize };
}

void MainPanel::OnFrameButtons(wxSFShapeMouseEvent& event) {
	switch (event.GetId()) {
	case BUTTON_Help:
		break;

	case BUTTON_Minimize:
		((wxFrame*)wxGetApp().GetTopWindow())->Iconize();
		break;

	case BUTTON_Close:
		wxGetApp().GetTopWindow()->Close();
		break;

	default:
		break;
	}
}

void MainPanel::OnSelectIso(wxSFShapeMouseEvent& event) {
	wxFileDialog fileDialog(nullptr, _("Please select an original Shadow Of The Colossus ISO file..."),
		"", "", "ISO files (*.iso)|*.iso", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;
	
	Freeze();

	wxFileName fileName(fileDialog.GetPath());
	m_isoPath = fileName.GetFullPath();
	m_fileLabel = fileName.GetFullName();

	m_fileDesc = "ISO File";
	m_fileDescColour = { 180,180,180 };

	m_fileBmp.LoadFile("Assets\\Icon\\Folder@2x.png", wxBITMAP_TYPE_PNG);
	m_mainButton->SetLabel("VERIFY ISO");
	m_mainButton->SetId(BUTTON_VerifyIso);
	
	RepositionAll();
	Thaw();

	m_fileDescRect = { -1,-1,-1,-1 };
}

void MainPanel::DrawForeground(wxDC& dc, bool fromPaint) {
	dc.DrawBitmap(m_logo, m_logox, m_logoy);

	dc.SetTextForeground(wxColour(255, 255, 255));
	dc.SetFont(m_fileLabelFont);
	dc.DrawText(m_fileLabel, m_xFLabel, m_yFLabel);

	dc.SetTextForeground(m_fileDescColour);
	dc.SetFont(m_fileDescFont);
	dc.DrawText(m_fileDesc, m_xFDesc, m_yFDesc);

	dc.SetUserScale(m_fileBmpScale, m_fileBmpScale);
	dc.DrawBitmap(m_fileContainer, m_xFCont, m_yFCont, true);
	dc.DrawBitmap(m_fileBmp, m_xFBmp, m_yFBmp, true);
	dc.SetUserScale(1.0, 1.0);
}

void MainPanel::OnSize(wxSizeEvent& event) {
	BackgroundImageCanvas::OnSize(event);
	m_logox = (event.GetSize().x / 2) - (m_logo.GetWidth() / 2);
	m_logoy = 70;

	RepositionAll();
}

void MainPanel::OnMouseMove(wxMouseEvent& event) {
	BackgroundImageCanvas::OnMouseMove(event);

	bool isHoveringFileDesc = m_fileDescRect.Contains(event.GetPosition());
	if (isHoveringFileDesc != m_isHoveringFileDesc) {
		SetCursor((wxStockCursor)((wxCURSOR_DEFAULT * !isHoveringFileDesc) + (wxCURSOR_CLOSED_HAND * isHoveringFileDesc)));
		m_isHoveringFileDesc = isHoveringFileDesc;
	}
}