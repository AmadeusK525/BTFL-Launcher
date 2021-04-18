#pragma once

#include <wx\wxsf\wxShapeFramework.h>

enum {
	BUTTON_Help,
	BUTTON_Minimize,
	BUTTON_Close
};

class FrameButtons : public wxSFGridShape {
public:
	FrameButtons() = default;
	void Init();

	XS_DECLARE_CLONABLE_CLASS(FrameButtons);
};

