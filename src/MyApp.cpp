#include "MyApp.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
	wxInitAllImageHandlers();

	m_mainFrame = new MainFrame(nullptr, -1, "Beyond The Forbidden Lands - Launcher");
	m_mainFrame->Maximize();
	m_mainFrame->Show();

	return true;
}
