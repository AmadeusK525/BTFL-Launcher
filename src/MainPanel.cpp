#include "MainPanel.h"

#include "MainFrame.h"
#include "MyApp.h"

#include <wx\filename.h>

wxBEGIN_EVENT_TABLE(MainPanel, wxSFShapeCanvas)

EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Help, MainPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Minimize, MainPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Close, MainPanel::OnFrameButtons)

EVT_SF_SHAPE_LEFT_DOWN(BUTTON_SelectIso, MainPanel::OnSelectIso)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_VerifyIso, MainPanel::OnVerifyIso)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Settings, MainPanel::OnSettings)

wxEND_EVENT_TABLE()

MainPanel::MainPanel(wxSFDiagramManager* manager,
	MainFrame* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style) : BackgroundImageCanvas(manager, parent, id, pos, size, style)
{
	m_mainFrame = parent;

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
	m_configButton->SetId(BUTTON_Settings);
	m_configButton->SetBitmap(wxBitmap("Assets\\Icon\\Settings@2x.png", wxBITMAP_TYPE_PNG));
	m_configButton->SetPadding(15, 15);
	manager->AddShape(m_configButton, nullptr, wxDefaultPosition, true, false);

	m_frameButtons = (FrameButtons*)manager->AddShape(CLASSINFO(FrameButtons), false);
	m_frameButtons->Init();
}

void MainPanel::RepositionAll()
{
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

void MainPanel::OnFrameButtons(wxSFShapeMouseEvent& event)
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

	default:
		break;
	}
}

void MainPanel::OnSelectIso(wxSFShapeMouseEvent& event)
{
	wxFileDialog fileDialog(nullptr, _("Please select an original Shadow Of The Colossus ISO file..."),
		"", "", "ISO files (*.iso)|*.iso", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if ( fileDialog.ShowModal() == wxID_CANCEL )
		return;

	wxFileName fileName(fileDialog.GetPath());
	wxString ext = fileName.GetExt();
	if ( ext != ".iso" && ext != "iso" )
	{
		wxMessageBox("You need to select a .iso file.");
		return;
	}

	m_mainFrame->ShowDisclaimer();

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

void MainPanel::OnVerifyIso(wxSFShapeMouseEvent& event)
{
	if ( !m_mainFrame->HasUserAgreedToDisclaimer() )
	{
		m_mainFrame->ShowDisclaimer();
		return;
	}

	
}

void MainPanel::OnSettings(wxSFShapeMouseEvent& event)
{
	m_mainFrame->ShowSettings();
}

void MainPanel::DrawForeground(wxDC& dc, bool fromPaint)
{
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

void MainPanel::OnSize(wxSizeEvent& event)
{
	BackgroundImageCanvas::OnSize(event);
	m_logox = (event.GetSize().x / 2) - (m_logo.GetWidth() / 2);
	m_logoy = 70;

	RepositionAll();
}

void MainPanel::OnMouseMove(wxMouseEvent& event)
{
	BackgroundImageCanvas::OnMouseMove(event);

	bool isHoveringFileDesc = m_fileDescRect.Contains(event.GetPosition());
	if ( isHoveringFileDesc != m_isHoveringFileDesc )
	{
		SetCursor((wxStockCursor)((wxCURSOR_DEFAULT * !isHoveringFileDesc) + (wxCURSOR_CLOSED_HAND * isHoveringFileDesc)));
		m_isHoveringFileDesc = isHoveringFileDesc;
	}
}