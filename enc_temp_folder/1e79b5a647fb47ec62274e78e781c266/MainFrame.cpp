#include "MainFrame.h"

#include <wx\hyperlink.h>

MainFrame::MainFrame(wxWindow* parent, 
	wxWindowID id,
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	long style, 
	const wxString& name) : wxFrame(parent, id, title, pos, size, style, name) {
	SetIcon(wxICON(aaaaBTFLValusLogoNoText));
	SetBackgroundColour(wxColour(0, 0, 0));

	m_patchNotesWindow = new LeftSidebar(this, -1, wxDefaultPosition, wxSize(400, -1), wxBORDER_NONE);
	m_mainPanel = new MainPanel(&m_mainPanelManager, this, -1);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_patchNotesWindow, wxSizerFlags(0).Expand());
	sizer->Add(m_mainPanel, wxSizerFlags(1).Expand());

	wxPanel* copyrightPanel = new wxPanel(this, -1, wxDefaultPosition, wxSize(-1, 40), wxBORDER_NONE);
	copyrightPanel->SetBackgroundColour(wxColour(0, 0, 0));

	wxStaticText* copyright = new wxStaticText(copyrightPanel, -1, "Beyond The Forbidden Lands is officialy classified as a modification of \"Shadow of the Colossus\" (2005) and ownership of the original game is required to play.");
	copyright->SetFont(wxFontInfo(11).FaceName("Times New Roman"));
	copyright->SetForegroundColour(wxColour(150, 150, 150));

	wxStaticText* readDisclaimers = new wxStaticText(copyrightPanel, -1, "Read Disclaimers");
	readDisclaimers->SetFont(wxFontInfo(11).FaceName("Times New Roman"));
	readDisclaimers->SetForegroundColour(wxColour(52, 199, 226));
	readDisclaimers->Bind(wxEVT_LEFT_DOWN, [](wxMouseEvent&) { wxLaunchDefaultBrowser("http://btflgame.com"); });
	readDisclaimers->SetCursor(wxCURSOR_CLOSED_HAND);

	wxBoxSizer* copyrightSizer = new wxBoxSizer(wxHORIZONTAL);
	copyrightSizer->AddSpacer(40);
	copyrightSizer->Add(copyright, wxSizerFlags(0).CenterVertical());
	copyrightSizer->AddStretchSpacer(1);
	copyrightSizer->Add(readDisclaimers, wxSizerFlags(0).CenterVertical());
	copyrightSizer->AddSpacer(40);

	copyrightPanel->SetSizer(copyrightSizer);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(sizer, wxSizerFlags(1).Expand());
	mainSizer->Add(copyrightPanel, wxSizerFlags(0).Expand());

	SetSizer(mainSizer);
}

void MainFrame::LoadPatchNotes() {
	m_patchNotesWindow->Load();
}
