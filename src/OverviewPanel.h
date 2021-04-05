#ifndef OVERVIEWPANEL_H_
#define OVERVIEWPANEL_H_
#pragma once

#include <wx\panel.h>
#include <wx\custombgwin.h>

class OverviewPanel : public wxCustomBackgroundWindow<wxPanel> {
private:
	wxImage m_background{};
	double m_bgRatio = 0.0;

public:
	OverviewPanel(wxWindow* parent);

	void OnSize(wxSizeEvent& event);

	DECLARE_EVENT_TABLE()
};

inline wxImage amGetScaledImage(int maxWidth, int maxHeight, wxImage& image, double ratio) {
    int width = image.GetWidth();
    int height = image.GetHeight();

    int neww, xoffset;
    int newh, yoffset;

    if (width > height) {
        neww = maxWidth;
        newh = maxWidth / ratio;
        xoffset = 0;
        yoffset = (maxHeight / 2) - (newh / 2);
    } else {
        newh = maxHeight;
        neww = maxHeight * ratio;
        yoffset = 0;
        xoffset = (maxWidth / 2) - (neww / 2);
    }

    return image.Scale(neww, newh, wxIMAGE_QUALITY_HIGH).Size(wxSize(maxWidth, maxHeight), wxPoint(xoffset, yoffset), 0, 0, 0);
}

#endif