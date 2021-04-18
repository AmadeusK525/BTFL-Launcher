#include "MyApp.h"
#include "MainFrame.h"

#include "IsoChecking.h"
#include <wx\filename.h>

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
	wxInitAllImageHandlers();
	wxBitmap shape("Assets\\FrameShape.png", wxBITMAP_TYPE_PNG);

	// ***For Visual Studio Project only***
	wxFileName exec = argv[0];
	exec.RemoveLastDir();
	wxSetWorkingDirectory(exec.GetPath());

	m_mainFrame = new MainFrame(nullptr, -1, "Beyond The Forbidden Lands - Launcher", 
		wxDefaultPosition, shape.GetSize(), wxFRAME_SHAPED | wxBORDER_NONE);
	m_mainFrame->Show();
	m_mainFrame->SetShape(wxRegion(shape, wxColour(255, 255, 255)));
	m_mainFrame->CenterOnScreen();
	m_mainFrame->Layout();
	m_mainFrame->Update();

	m_mainFrame->LoadPatchNotes();

	return true;
}