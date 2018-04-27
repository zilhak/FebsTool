#include "HelpDialog.hpp"

BEGIN_EVENT_TABLE(HelpDialog, wxDialog)
    EVT_PAINT(HelpDialog::onPaint)
END_EVENT_TABLE()

HelpDialog::HelpDialog(wxWindow * parent) : 
                    wxDialog(parent, wxID_ANY, wxT("Help"), wxDefaultPosition, wxSize(710,500))
{
    wxImage help_image = wxImage("manual.jpg");
    _bitmap = wxBitmap(help_image.Scale(695, 460, wxIMAGE_QUALITY_HIGH));
}


HelpDialog::~HelpDialog()
{
    /* EMPTY */
}

void HelpDialog::onPaint(wxPaintEvent & event)
{
    wxPaintDC dc(this);

    dc.DrawBitmap(_bitmap, wxPoint(0,0));
}
