#ifndef SECONDARYPANEL_H_
#define SECONDARYPANEL_H_
#pragma once

#include "BaseClasses.h"
#include "PatchNotes.h"
#include "FrameButtons.h"
#include "TransparentButton.h"

#include <wx\wxsf\wxShapeFramework.h>
#include <wx\richtext\richtextctrl.h>

class SecondaryPanel;
class MainFrame;

class DisclaimerPanel : public ReadOnlyRTC
{
private:
	SecondaryPanel* m_seconPanel = nullptr;
	int m_bgx = 0, m_bgy = 0;

public:
	DisclaimerPanel(SecondaryPanel* parent,
		wxWindowID id = -1,
		const wxString& value = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxBORDER_NONE);

	inline void SetBackgroundX(int x) { m_bgx = x; }
	inline void SetBackgroundY(int y) { m_bgy = y; }

	virtual void PaintBackground(wxDC& dc) override;
};

enum
{
	BUTTON_Back,

	BUTTON_DisclaimerDecline,
	BUTTON_DisclaimerAgree
};

class SecondaryPanel : public BackgroundImageCanvas
{
private:
	MainFrame* m_mainFrame = nullptr;

	FrameButtons* m_frameButtons = nullptr;
	wxSFTextShape* m_title = nullptr;
	wxSFBitmapShape* m_backArrow = nullptr;

	wxBitmap m_topSeparator;

	const int TOP_SPACE = 50;
	const int BOTTOM_SPACE = 100;

	wxBoxSizer* m_verSizer = nullptr;

	DisclaimerPanel* m_disclaimer = nullptr;
	TransparentButton* m_disDecline = nullptr,
		* m_disAgree = nullptr;

public:
	SecondaryPanel(wxSFDiagramManager* manager, MainFrame* parent);

	void ShowDisclaimer();
	void ShowSettings();

	void OnFrameButtons(wxSFShapeMouseEvent& event);

	void RepositionAll();
	virtual void DrawForeground(wxDC& dc, bool fromPaint) override;

	virtual void OnSize(wxSizeEvent& event) override;
	virtual void OnMouseMove(wxMouseEvent& event) override { wxSFShapeCanvas::OnMouseMove(event); }

	wxDECLARE_EVENT_TABLE();
};
#endif