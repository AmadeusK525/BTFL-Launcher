#ifndef SECONDARYPANEL_H_
#define SECONDARYPANEL_H_
#pragma once

#include "BaseClasses.h"
#include "PatchNotes.h"
#include "FrameButtons.h"

#include <wx\wxsf\wxShapeFramework.h>
#include <wx\richtext\richtextctrl.h>

class SecondaryPanel;
class MainFrame;

class DisclaimerPanel : public ReadOnlyRTC {
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

enum {
	BUTTON_Back
};

class SecondaryPanel : public BackgroundImageCanvas {
private:
	MainFrame* m_mainFrame = nullptr;

	DisclaimerPanel* m_disclaimer = nullptr;
	FrameButtons* m_frameButtons = nullptr;
	wxSFTextShape* m_title = nullptr;
	wxSFBitmapShape* m_backArrow = nullptr;

	const int TOP_SPACE = 50;

	wxBitmap m_topSeparator;

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