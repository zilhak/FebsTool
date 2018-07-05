//
// Created by bogonets on 2018. 6. 21..
//

#include "FileManagerFrame.hpp"

FileManagerFrame::FileManagerFrame(wxString const & title) :
    wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    initialize();
}

FileManagerFrame::~FileManagerFrame()
{

}
void FileManagerFrame::initialize()
{
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);

    h_sizer->Add(v_sizer, 0, wxEXPAND);


    SetMinSize(wxSize(800, 600));
    SetSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
                   wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));

    initializeOpen(v_sizer);
    initializeLeftPanel(v_sizer);
    initializeRightPanel(h_sizer);
}

void FileManagerFrame::initializeOpen(wxSizer * sizer)
{
    wxPanel * open_panel = new wxPanel(this, wxID_ANY);

    sizer->Add(open_panel);
}

void FileManagerFrame::initializeLeftPanel(wxSizer * sizer)
{
    _control_box = new ManagerControlBox(this, wxID_ANY);

    sizer->Add(_control_box, 1, wxEXPAND);
}

void FileManagerFrame::initializeRightPanel(wxSizer * sizer)
{
    _screen = new ManagerPanel(this, wxID_ANY);

    sizer->Add(_screen, 1, wxEXPAND);
}


