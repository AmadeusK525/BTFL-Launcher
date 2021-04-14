#include "MainFrame.h"

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

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(sizer, wxSizerFlags(1).Expand());
	mainSizer->AddSpacer(60);

	SetSizer(mainSizer);
}

void MainFrame::LoadPatchNotes() {
	m_patchNotesWindow->Load();
}
