#ifndef MAINPANEL_H_
#define MAINPANEL_H_
#pragma once

#include <wx\wx.h>
#include <wx\wxsf\wxShapeFramework.h>

#include "BaseClasses.h"
#include "PatchNotes.h"
#include "FrameButtons.h"
#include "TransparentButton.h"

class MainFrame;

enum
{
	BUTTON_Settings,

	BUTTON_SelectIso,
	BUTTON_VerifyIso,
	BUTTON_Install,
	BUTTON_Play
};

class MainPanel : public BackgroundImageCanvas
{
private:
	MainFrame* m_mainFrame = nullptr;

	wxBitmap m_logo;
	int m_logox = 0, m_logoy = 0;

	wxString m_isoPath;

	wxString m_fileLabel{ "No ISO selected..." }, m_fileDesc{ "View Installation Guide" };
	wxFont m_fileLabelFont{ wxFontInfo(12).FaceName("Times New Roman") },
		m_fileDescFont{ wxFontInfo(10).FaceName("Times New Roman") };
	wxRect m_fileDescRect;
	bool m_isHoveringFileDesc = false;
	wxColour m_fileDescColour{ 52, 199, 226 };
	wxBitmap m_fileContainer, m_fileBmp;

	double m_fileBmpScale = 1.0;
	int m_xFLabel = -1, m_yFLabel = -1,
		m_xFDesc = -1, m_yFDesc = -1,
		m_xFCont = -1, m_yFCont = -1,
		m_xFBmp = -1, m_yFBmp = -1;

	TransparentButton* m_mainButton = nullptr,
		* m_configButton = nullptr;
	FrameButtons* m_frameButtons = nullptr;

public:
	MainPanel(wxSFDiagramManager* manager,
		MainFrame* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxBORDER_NONE);

	inline const wxBitmap& GetBackgroundBitmap() { return m_background; }
	void RepositionAll();

	void OnFrameButtons(wxSFShapeMouseEvent& event);
	void OnSelectIso(wxSFShapeMouseEvent& event);
	void OnVerifyIso(wxSFShapeMouseEvent& event);
	void OnSettings(wxSFShapeMouseEvent& event);

	virtual void OnSize(wxSizeEvent& event) override;
	virtual void OnMouseMove(wxMouseEvent& event) override;

	virtual void DrawForeground(wxDC& dc, bool fromPaint) override;

	wxDECLARE_EVENT_TABLE();
};

#endif