#include "PatchNotes.h"
#include "OverviewPanel.h"

#include <wx\dcbuffer.h>
#include <wx\dcgraph.h>

BEGIN_EVENT_TABLE(PatchNotesWindow, wxScrolledWindow)

EVT_PAINT(PatchNotesWindow::OnPaint)
EVT_SCROLLWIN(PatchNotesWindow::OnScroll)

END_EVENT_TABLE()

PatchNotesWindow::PatchNotesWindow(wxWindow* parent,
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style) : wxScrolledWindow(parent, id, pos, size, style) {
	m_overviewPanel = (OverviewPanel*)parent;
}

void PatchNotesWindow::DoReposition(bool calcContent, bool calcTitle) {
	if (calcContent) {
		CalcWrappedText(&m_content, &m_contentToDraw, wxFontInfo(12), &m_contentSize);
		m_contentSize.y += 40;
	}

	if (calcTitle)
		CalcWrappedText(&m_title, &m_titleToDraw, wxFontInfo(40).FaceName("Times New Roman"), &m_titleSize);

	m_separatorPos = {m_titleSize.x + 20, ((m_titleSize.y / 3) * 2) + 15};
	m_contentY = m_titleSize.y + 15;
	SetVirtualSize(m_contentSize + wxSize(0, m_titleSize.y));
}

void PatchNotesWindow::OnPaint(wxPaintEvent& event) {
	wxAutoBufferedPaintDC dc(this);
	wxSize clientSize = GetClientSize();

	dc.SetUserScale(m_bgScale, m_bgScale);
	dc.DrawBitmap(m_overviewPanel->GetBackgroundBitmap(), m_bgx, m_bgy);
	dc.SetUserScale(1.0, 1.0);

	wxGCDC gdc(dc);
	gdc.SetPen(wxPen(wxColour(70, 40, 30), 10));
	gdc.SetBrush(wxBrush(wxColour(102, 83, 67, 200)));
	gdc.DrawRoundedRectangle(wxPoint(5, 5), clientSize - wxSize(10, 10), 20.0);

	int xo, yo;
	CalcUnscrolledPosition(0, 0, &xo, &yo);

	DoPrepareDC(gdc);
	gdc.SetClippingRegion(10 + xo, 10 + yo, clientSize.x - 20, clientSize.y - 20);
	gdc.SetTextForeground(wxColour(255, 255, 255));

	gdc.SetFont(wxFontInfo(40).FaceName("Times New Roman"));
	gdc.DrawText(m_titleToDraw, 15, 15);

	gdc.SetPen(wxPen(wxColour(255, 255, 255), 3, wxPENSTYLE_LONG_DASH));
	gdc.DrawLine(m_separatorPos, wxPoint(clientSize.x - 20, m_separatorPos.y));

	gdc.SetFont(wxFontInfo(12));
	gdc.DrawText(m_contentToDraw, 15, 15 + m_contentY);
}

void PatchNotesWindow::OnScroll(wxScrollWinEvent& event) {
	//Refresh();
	event.Skip();
}

void PatchNotesWindow::CalcWrappedText(wxString* str, wxString* strToDraw, const wxFont& font, wxSize* size) {
	*strToDraw = *str;

	if (strToDraw->IsEmpty())
		return;

	wxClientDC dc(this);
	wxGCDC gdc(dc);
	gdc.SetFont(font);

	wxString currentString{ "" };
	wxSize textSize;

	int maxWidth = GetClientSize().x - 40;
	int begin = 0;
	int end = 0;
	int strLen = str->Length();

	bool first = true;
	int maxChar = 0;

	while (end < strLen) {
		if (end >= begin + maxChar || first) {
			end += 2;
		} else
			end += maxChar;

		if (end > strLen)
			end = strLen;

		currentString = strToDraw->SubString(begin, end);
		textSize = gdc.GetMultiLineTextExtent(currentString);

		if (textSize.x >= maxWidth) {
			if (first) {
				maxChar = end - 5;

				size_t found = currentString.rfind("\n");
				if (found != std::string::npos)
					maxChar -= found;

				first = false;
			}

			size_t found = strToDraw->rfind(" ", end);

			if (found != std::string::npos) {
				strToDraw->replace(found, 1, "\n");
				begin = found;
			} else {
				strToDraw->replace(begin + maxChar, 2, "-\n");
				begin += maxChar + 1;
			}

			end = begin;
		}
	}

	if (size)
		*size = gdc.GetMultiLineTextExtent(*strToDraw);
}