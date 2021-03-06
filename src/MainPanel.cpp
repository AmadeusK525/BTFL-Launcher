#include "MainPanel.h"

#include "MainFrame.h"
#include "MyApp.h"
#include "IsoChecking.h"

#include <thread>
#include <time.h>

#include "wxmemdbg.h"

bool CustomGauge::UpdateValue(int currentUnit)
{
	if ( currentUnit > m_units )
		currentUnit = m_units;

	m_currentUnit = currentUnit;
	Refresh();

	return m_currentUnit >= m_units;
}

void CustomGauge::DrawBar(wxDC& dc)
{
	wxRect rect = GetBoundingBox();
	rect.Deflate(2);
	rect.SetWidth((m_currentUnit * rect.GetWidth()) / m_units);

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxBrush(m_barColour));

	dc.DrawRoundedRectangle(rect, GetRadius());
}

void CustomGauge::DrawNormal(wxDC& dc)
{
	wxSFRoundRectShape::DrawNormal(dc);
	DrawBar(dc);

}

void CustomGauge::DrawHover(wxDC & dc)
{
	wxSFRoundRectShape::DrawHover(dc);
	DrawBar(dc);
}

wxBEGIN_EVENT_TABLE(MainPanel, wxSFShapeCanvas)

EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Help, MainPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Minimize, MainPanel::OnFrameButtons)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Close, MainPanel::OnFrameButtons)

EVT_SF_SHAPE_LEFT_DOWN(BUTTON_SelectIso, MainPanel::OnSelectIso)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_VerifyIso, MainPanel::OnVerifyIso)
EVT_SF_SHAPE_LEFT_DOWN(BUTTON_Settings, MainPanel::OnSettings)

EVT_TIMER(TIMER_Gauge, MainPanel::OnGaugeTimer)

wxEND_EVENT_TABLE()

MainPanel::MainPanel(wxSFDiagramManager* manager,
	MainFrame* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style) : BackgroundImageCanvas(manager, parent, id, pos, size, style), m_gaugeTimer(this, TIMER_Gauge)
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
	wxRect shapeRect = m_mainButton->GetBoundingBox();

	m_mainButton->MoveTo(40, size.y - shapeRect.height - 40);
	wxRect shapeBB = m_mainButton->GetBoundingBox();

	int yPadding;
	m_mainButton->GetPadding(nullptr, &yPadding);
	m_configButton->RecalculateSelf(wxSize(shapeRect.height - (yPadding * 2), shapeRect.height - (yPadding * 2)));
	m_configButton->MoveTo(shapeBB.GetTopRight() + wxPoint(15, 0));

	shapeRect = m_frameButtons->GetBoundingBox();
	m_frameButtons->MoveTo(size.x - shapeRect.width - 10, 10);

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
	if ( m_fileDesc == "View Installation Guide" )
		m_fileDescRect = { wxPoint(m_xFDesc, m_yFDesc), textSize };
	else
		m_fileDescRect = { -1,-1,-1,-1 };

	if ( m_gauge )
	{
		m_gauge->SetRectSize(350, 8);
		m_gauge->MoveTo(size.x - 350 - 20, size.y - 48);
		shapeRect = m_gauge->GetBoundingBox();

		m_gaugeLabel->MoveTo(shapeRect.x, shapeRect.y - m_gaugeLabel->GetRectSize().y - 10);
		m_gaugeProgress->MoveTo(shapeRect.GetRight() - m_gaugeProgress->GetRectSize().x,
			shapeRect.y - m_gaugeProgress->GetRectSize().y - 10);
	}
}

void MainPanel::VerifyIso()
{
	if ( !m_iso.Exists() )
		return;

	// Create the gauge and make the main button, which right now is for
	// verifying the iso, uninteractible.
	CreateGauge();
	m_mainButton->Enable(false);
	m_mainFrame->Refresh();
	m_mainFrame->Update();

	// Start the gauge timer so that updates to m_nextGaugeValue are
	// taken into account.
	m_gaugeTimer.Start(1);

	// Start a new thread so we don't hog the program when verifying the iso.
	std::thread thread(
		[&]()
		{
			m_nextGaugeLabel = "Verifying ISO validity...";
			m_nextGaugeValue = 4;

			int gaugeValue = 4;
			bool isIsoValid = false;

			srand(time(0));

			for ( iso::ISO_Region region = iso::ISO_Usa; region < iso::ISO_Invalid; region = (iso::ISO_Region)(region + 1) )
			{
				// Generate a random number for the gauge but keeping in mind the fact that
				// It has to take into account the current loop we're in.
				int max = (100 / iso::NUMBER_OF_ISOS) * (region + 1);
				m_nextGaugeValue = (rand() % (max - gaugeValue)) + gaugeValue;
				gaugeValue = m_nextGaugeValue;
				
				// Instead of calling iso::GetFileHash() once and storing the value, call it
				// multiple times so that the process artificially takes a longer time.
				if ( iso::IsHashFromIso(iso::GetFileHash(m_iso.GetFullPath()), region) )
					isIsoValid = true;
			}

			// Change m_gaugeResult so that the function OnGaugeFinished() knows what to
			// do next.
			if ( isIsoValid )
				m_gaugeResult = GAUGE_VerifyValid;
			else
				m_gaugeResult = GAUGE_VerifyInvalid;

			// And set the gauge to 100 to trigger the function call.
			m_nextGaugeValue = 100;
			m_nextGaugeLabel = "Processing results...";
		}
	);

	thread.detach();
}

void MainPanel::CreateGauge()
{
	if ( m_gauge )
		return;

	wxSFDiagramManager* manager = GetDiagramManager();

	m_gauge = new CustomGauge(wxDefaultPosition, wxDefaultPosition, 100, 2.0, manager);
	m_gauge->SetBorder(wxPen(wxColour(50, 50, 50)));
	m_gauge->SetFill(wxBrush(wxColour(50, 50, 50)));
	manager->AddShape(m_gauge, nullptr, wxDefaultPosition, true);

	m_gaugeLabel = (wxSFTextShape*)manager->AddShape(CLASSINFO(wxSFTextShape), false);
	m_gaugeLabel->SetBorder(*wxTRANSPARENT_PEN);
	m_gaugeLabel->SetFill(*wxTRANSPARENT_BRUSH);
	m_gaugeLabel->SetTextColour(wxColour(255, 255, 255, 150));
	m_gaugeLabel->SetFont(wxFontInfo(12).FaceName("Times New Roman"));
	m_gaugeLabel->SetText("...");
	m_gaugeLabel->SetStyle(0);

	m_gaugeProgress = (wxSFTextShape*)manager->AddShape(CLASSINFO(wxSFTextShape), false);
	m_gaugeProgress->SetBorder(*wxTRANSPARENT_PEN);
	m_gaugeProgress->SetFill(*wxTRANSPARENT_BRUSH);
	m_gaugeProgress->SetTextColour(wxColour(255, 255, 255, 255));
	m_gaugeProgress->SetFont(wxFontInfo(12).FaceName("Times New Roman"));
	m_gaugeProgress->SetText("0% Complete");
	m_gaugeProgress->SetStyle(0);

	RepositionAll();
}

void MainPanel::DestroyGauge()
{
	if ( !m_gauge )
		return;
	
	wxSFDiagramManager* manager = GetDiagramManager();
	manager->RemoveShape(m_gauge, false);
	manager->RemoveShape(m_gaugeLabel, false);
	manager->RemoveShape(m_gaugeProgress, false);

	m_gauge = nullptr;
	m_gaugeLabel = nullptr;
	m_gaugeProgress = nullptr;
}

void MainPanel::UpdateGauge(int currentUnit, const wxString & message)
{
	if ( !m_gauge )
		return;

	m_gauge->UpdateValue(currentUnit);
	m_gaugeLabel->SetText(message);
	m_gaugeProgress->SetText(std::to_string(m_gauge->GetCurrentPercent()) + "% Complete");

	RepositionAll();
	Refresh();
}

void MainPanel::OnGaugeTimer(wxTimerEvent& event)
{
	if ( m_nextGaugeValue != -1 )
	{
		UpdateGauge(m_nextGaugeValue, m_nextGaugeLabel);
		m_nextGaugeValue = -1;

		if ( m_gauge->GetCurrentPercent() >= 100 )
		{
			m_gaugeTimer.Stop();
			OnGaugeFinished();
		}
	}
}

void MainPanel::OnGaugeFinished()
{
	switch ( m_gaugeResult )
	{
	case GAUGE_VerifyValid:
		// If the iso is valid, change current state of m_mainButton, transforming it
		// into a button to download and install the game.
		m_mainButton->SetLabel("INSTALL");
		m_mainButton->SetBitmap(wxBitmap("Assets\\Icon\\Download@2x.png", wxBITMAP_TYPE_PNG));
		m_mainButton->SetId(BUTTON_Install);
		m_mainButton->Enable(true);

		// Update file labels to display what will be installed next.
		m_fileLabel = "Shadow Of The Colossus";
		m_fileDesc = "PlayStation 2 ISO (NTSC)";
		m_fileBmp.LoadFile("Assets\\Icon\\Check@2x.png", wxBITMAP_TYPE_PNG);
		break;

	case GAUGE_VerifyInvalid:
		// Just re-enable the m_mainButton but leave it as a verify button,
		// Since the iso was invalid.
		m_mainButton->Enable(true);
		break;
	}

	DestroyGauge();
	RepositionAll();
}

void MainPanel::OnFrameButtons(wxSFShapeMouseEvent& event)
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

	// Double check to see if the file really is an iso file, in case the
	// wxFileDialog didn't take care of it.
	wxString ext = fileName.GetExt();
	if ( ext != ".iso" && ext != "iso" )
	{
		wxMessageBox("You need to select a .iso file.");
		return;
	}

	m_iso = fileName;

	// Show disclaimer before doing everything else if the user hasn't agreed to it yet.
	if ( !m_mainFrame->HasUserAgreedToDisclaimer() )
		m_mainFrame->ShowDisclaimer();

	// Update GUI components to make the iso file selection clear to the user.
	m_fileLabel = fileName.GetFullName();

	m_fileDesc = "ISO File";
	m_fileDescColour = { 180,180,180 };

	m_fileBmp.LoadFile("Assets\\Icon\\Folder@2x.png", wxBITMAP_TYPE_PNG);
	m_mainButton->SetLabel("VERIFY ISO");
	m_mainButton->SetId(BUTTON_VerifyIso);

	RepositionAll();
	Thaw();
}

void MainPanel::OnVerifyIso(wxSFShapeMouseEvent& event)
{
	// If the user still hasn't agreed to the disclaimer, make them agree to it again.
	// VerifyIso() will only be called once they do.
	if ( !m_mainFrame->HasUserAgreedToDisclaimer() )
	{
		m_mainFrame->ShowDisclaimer();
		return;
	}

	VerifyIso();
}

void MainPanel::OnSettings(wxSFShapeMouseEvent& event)
{
	m_mainFrame->ShowSettings();
}

void MainPanel::DrawForeground(wxDC& dc, bool fromPaint)
{
	// Draw the logo independent of the background image.
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