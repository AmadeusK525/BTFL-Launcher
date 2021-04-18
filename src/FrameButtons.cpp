#include "FrameButtons.h"

XS_IMPLEMENT_CLONABLE_CLASS(FrameButtons, wxSFGridShape)

void FrameButtons::Init() {
	AcceptChild("All");
	RemoveStyle(sfsHOVERING);
	SetCellSpace(2);

	SetFill(*wxTRANSPARENT_BRUSH);
	SetBorder(wxPen(wxColour(255, 255, 255, 80), 1));

	wxVector<wxSFShapeBase*> temp;

	wxSFBitmapShape* helpButton = (wxSFBitmapShape*)GetParentCanvas()->GetDiagramManager()->AddShape(CLASSINFO(wxSFBitmapShape), false);
	helpButton->CreateFromFile("Assets\\Icon\\Help.png", wxBITMAP_TYPE_PNG);
	helpButton->SetId(BUTTON_Help);

	wxSFBitmapShape* minimizeButton = (wxSFBitmapShape*)GetParentCanvas()->GetDiagramManager()->AddShape(CLASSINFO(wxSFBitmapShape), false);
	minimizeButton->CreateFromFile("Assets\\Icon\\Minimize.png", wxBITMAP_TYPE_PNG);
	minimizeButton->SetId(BUTTON_Minimize);

	wxSFBitmapShape* closeButton = (wxSFBitmapShape*)GetParentCanvas()->GetDiagramManager()->AddShape(CLASSINFO(wxSFBitmapShape), false);
	closeButton->CreateFromFile("Assets\\Icon\\Close.png", wxBITMAP_TYPE_PNG);
	closeButton->SetId(BUTTON_Close);

	temp.push_back(helpButton);
	temp.push_back(minimizeButton);
	temp.push_back(closeButton);
	temp.push_back(this);

	AppendToGrid(helpButton);
	AppendToGrid(minimizeButton);
	AppendToGrid(closeButton);

	for (wxSFShapeBase*& shape : temp) {
		shape->AddStyle(sfsEMIT_EVENTS);
		shape->RemoveStyle(sfsHIGHLIGHTING);
		shape->RemoveStyle(sfsPOSITION_CHANGE);
		shape->RemoveStyle(sfsSIZE_CHANGE);
		shape->RemoveStyle(sfsSHOW_HANDLES);
	}

	Update();
}