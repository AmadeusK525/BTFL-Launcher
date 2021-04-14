#include "PatchNotes.h"
#include "MainFrame.h"

#include <wx\richtext\richtextxml.h>
#include <wx\dcbuffer.h>
#include <wx\dcgraph.h>
#include <wx\filesys.h>
#include <wx\fs_inet.h>
#include <wx\sstream.h>

PatchNotesWindow::PatchNotesWindow(wxWindow* parent,
	wxWindowID id,
	const wxString& value,
	const wxPoint& pos,
	const wxSize& size,
	long style) : wxRichTextCtrl(parent, id, value, pos, size, style) {
	SetCursor(wxCURSOR_DEFAULT);
	SetTextCursor(wxCURSOR_DEFAULT);
	Bind(wxEVT_SET_FOCUS, [](wxFocusEvent&) {});
	Bind(wxEVT_LEFT_DOWN, [](wxMouseEvent&) {});
	Bind(wxEVT_CHAR, [](wxKeyEvent&) {});

	m_shadowBitmap.LoadFile("Assets\\Scroll Shadow\\Large@2x.png", wxBITMAP_TYPE_PNG);
}

void PatchNotesWindow::PaintAboveContent(wxDC& dc) {
	int yo;
	CalcUnscrolledPosition(0, 0, nullptr, &yo);
	wxSize size = GetClientSize();

	double scale = (double)size.x / m_shadowBitmap.GetWidth();
	dc.SetUserScale(scale, scale);
	dc.DrawBitmap(m_shadowBitmap, wxPoint(0, (double)(size.y + yo - ((double)m_shadowBitmap.GetHeight() * scale)) / scale), true);
	dc.SetUserScale(1.0, 1.0);
}


///////////////////////////////////////////////////////////////////////
/////////////////////////// HyperlinkPanel ////////////////////////////
///////////////////////////////////////////////////////////////////////


wxBEGIN_EVENT_TABLE(HyperlinkPanel, wxPanel)

EVT_SIZE(HyperlinkPanel::OnSize)
EVT_PAINT(HyperlinkPanel::OnPaint)

EVT_LEFT_DOWN(HyperlinkPanel::OnLeftDown)
EVT_ENTER_WINDOW(HyperlinkPanel::OnEnterWindow)
EVT_LEAVE_WINDOW(HyperlinkPanel::OnLeaveWindow)

wxEND_EVENT_TABLE()

HyperlinkPanel::HyperlinkPanel(wxWindow* parent, const wxString& url, const wxBitmap& bmp, const wxSize& size, const wxPoint& pos, long style) :
	wxPanel(parent, -1, pos, size, style) {
	m_bitmap = bmp;
	m_url = url;
	m_bgRatio = (double)m_bitmap.GetWidth() / m_bitmap.GetHeight();

	SetBackgroundStyle(wxBG_STYLE_PAINT);
	Bind(wxEVT_ERASE_BACKGROUND, [](wxEraseEvent&) {});

	SetCursor(wxCURSOR_CLOSED_HAND);
}

void HyperlinkPanel::RecalculateSelf() {
	wxSize size = GetClientSize();
	double curRatio = (double)size.x / size.y;
	int paddingx2 = m_padding * 2;

	if (curRatio > m_bgRatio) {
		m_bgScale = (double)(size.x - paddingx2) / m_bitmap.GetWidth();
		m_bgx = (((size.x + paddingx2) - ((double)m_bitmap.GetWidth() * m_bgScale)) / 2) / m_bgScale;
		m_bgy = m_padding;
	} else {
		m_bgScale = (double)(size.y - paddingx2) / m_bitmap.GetHeight();
		m_bgy = (((size.y + paddingx2) - ((double)m_bitmap.GetHeight() * m_bgScale)) / 2) / m_bgScale;
		m_bgx = m_padding;
	}

	Refresh();
}

void HyperlinkPanel::OnSize(wxSizeEvent& event) {
	RecalculateSelf();
}

void HyperlinkPanel::OnPaint(wxPaintEvent& event) {
	wxAutoBufferedPaintDC dc(this);

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxBrush(GetBackgroundColour()));
	dc.DrawRectangle(wxPoint(0, 0), GetSize());

	wxGCDC gdc(dc);
	gdc.GetGraphicsContext()->Scale(m_bgScale, m_bgScale);
	gdc.DrawBitmap(m_bitmap, m_bgx, m_bgy, true);
	gdc.GetGraphicsContext()->Scale(1.0 / m_bgScale, 1.0 / m_bgScale);

	if (!m_isHovering) {
		wxSize size = GetSize();
		size.x += 20;
		size.y += 20;
		gdc.SetBrush(wxBrush(wxColour(0, 0, 0, 125)));
		gdc.DrawRectangle(wxPoint(-10, -10), size);
	}
}

void HyperlinkPanel::OnLeftDown(wxMouseEvent& event) {
	wxLaunchDefaultBrowser(m_url);
}

void HyperlinkPanel::OnEnterWindow(wxMouseEvent& event) {
	m_isHovering = true;
	Refresh();
}

void HyperlinkPanel::OnLeaveWindow(wxMouseEvent& event) {
	m_isHovering = false;
	Refresh();
}


///////////////////////////////////////////////////////////////////////
///////////////////////////// LeftSidebar /////////////////////////////
///////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(LeftSidebar, wxScrolledWindow)
EVT_MOTION(LeftSidebar::OnMove)
EVT_PAINT(LeftSidebar::OnPaint)
END_EVENT_TABLE()

LeftSidebar::LeftSidebar(wxWindow* parent,
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style) : wxPanel(parent, id, pos, size, style) {
	m_mainFrame = (MainFrame*)parent;

	SetBackgroundColour(wxColour(0, 0, 0));

	wxFileSystem::AddHandler(new wxInternetFSHandler);
	wxRichTextBuffer::AddHandler(new wxRichTextXMLHandler);

	m_rtc = new PatchNotesWindow(this, -1, "", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	m_rtc->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);
	m_rtc->SetScrollRate(15, 15);

	wxRichTextAttr attr;
	attr.SetFont(wxFontInfo(12).FaceName("Times New Roman"));
	attr.SetAlignment(wxTEXT_ALIGNMENT_JUSTIFIED);
	attr.SetLeftIndent(64);
	attr.SetRightIndent(64);
	attr.SetLineSpacing(10);
	attr.SetTextColour(wxColour(210, 210, 210));

	m_rtc->SetBasicStyle(attr);
	m_rtc->SetBackgroundColour(wxColour(0, 0, 0));
	m_rtc->SetFontScale(1.13);

	m_rtc->SetValue("\nFetching latest content...");

	m_scrollbar = new CustomRTCScrollbar(this, m_rtc, -1);
	wxBoxSizer* rtcSizer = new wxBoxSizer(wxHORIZONTAL);
	rtcSizer->AddSpacer(5);
	rtcSizer->Add(m_scrollbar, wxSizerFlags(0).Expand());
	rtcSizer->AddSpacer(5);
	rtcSizer->Add(m_rtc, wxSizerFlags(1).Expand());

	HyperlinkPanel* website = new HyperlinkPanel(this, "http://btflgame.com", wxBitmap("Assets\\Icon\\Website@2x.png", wxBITMAP_TYPE_PNG), wxSize(26, 26));
	website->SetBackgroundColour(wxColour(0, 0, 0));
	HyperlinkPanel* discord = new HyperlinkPanel(this, "https://discord.gg/8QHmrxJBwW", wxBitmap("Assets\\Icon\\Discord@2x.png", wxBITMAP_TYPE_PNG), wxSize(26, 26));
	discord->SetBackgroundColour(wxColour(0, 0, 0));
	HyperlinkPanel* facebook = new HyperlinkPanel(this, "http://facebook.com", wxBitmap("Assets\\Icon\\Facebook@2x.png", wxBITMAP_TYPE_PNG), wxSize(26, 26));
	facebook->SetBackgroundColour(wxColour(0, 0, 0));
	HyperlinkPanel* twitter = new HyperlinkPanel(this, "http://twitter.com/btfl_game", wxBitmap("Assets\\Icon\\Twitter@2x.png", wxBITMAP_TYPE_PNG), wxSize(26, 26));
	twitter->SetBackgroundColour(wxColour(0, 0, 0));
	HyperlinkPanel* youtube = new HyperlinkPanel(this, "https://www.youtube.com/channel/UCVLLghUk9B_rLddv7qNMS6w", wxBitmap("Assets\\Icon\\YouTube@2x.png", wxBITMAP_TYPE_PNG), wxSize(26, 26));
	youtube->SetBackgroundColour(wxColour(0, 0, 0));

	wxBoxSizer* socialSizer = new wxBoxSizer(wxHORIZONTAL);
	socialSizer->Add(website, wxSizerFlags(0).Border(wxALL, 10));
	socialSizer->Add(discord, wxSizerFlags(0).Border(wxALL, 10));
	socialSizer->Add(facebook, wxSizerFlags(0).Border(wxALL, 10));
	socialSizer->Add(twitter, wxSizerFlags(0).Border(wxALL, 10));
	socialSizer->Add(youtube, wxSizerFlags(0).Border(wxALL, 10));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(5);
	sizer->Add(rtcSizer, wxSizerFlags(1).Expand());
	sizer->Add(socialSizer, wxSizerFlags(0).CenterHorizontal());

	SetSizer(sizer);
}

bool LeftSidebar::Load() {
	wxFileSystem fs;
	wxFSFile* file = fs.OpenFile("http://btflgame.com/patch_notes/all.xml");

	if (file) {
		wxStringInputStream* stream = (wxStringInputStream*)(file->GetStream());

		if (stream->CanRead()) {
			wxRichTextBuffer buf;
			buf.LoadFile(*stream, wxRICHTEXT_TYPE_XML);
			buf.SetBasicStyle(m_rtc->GetBasicStyle());

			m_rtc->GetBuffer() = buf;
			m_rtc->Invalidate();
			m_rtc->Refresh();
			m_rtc->Update();
			m_scrollbar->RecalculateSelf();

			return true;
		}
	}

	return false;
}

void LeftSidebar::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	
	wxSize size = GetClientSize();
	wxRect rtcRect(m_rtc->GetRect());

	int middleX = size.x / 2;

	wxColour gray(100, 100, 100);
	wxColour black(0, 0, 0);

	wxSize gradientHalfSize(middleX, 1);

	dc.GradientFillLinear(wxRect(wxPoint(0, rtcRect.GetBottom() + 1), gradientHalfSize), black, gray);
	dc.GradientFillLinear(wxRect(wxPoint(middleX, rtcRect.GetBottom() + 1), gradientHalfSize), gray, black);

	dc.GradientFillLinear(wxRect(wxPoint(0, size.y - 1), gradientHalfSize), black, gray);
	dc.GradientFillLinear(wxRect(wxPoint(middleX, size.y - 1), gradientHalfSize), gray, black);
}

void LeftSidebar::OnMove(wxMouseEvent& event) {

}
