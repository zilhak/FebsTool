//
// Created by bogonets on 2018. 6. 21..
//

#include "FileManagerFrame.hpp"

FileManagerFrame::FileManagerFrame(wxString const & title) : wxDialog(NULL, wxID_ANY, wxT("File Manager"))
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

    initializeOpen(v_sizer);
    initializeLeftPanel(v_sizer);
    initializeRightPanel(h_sizer);
}

void FileManagerFrame::initializeOpen(wxSizer * sizer)
{

}

void FileManagerFrame::initializeLeftPanel(wxSizer * sizer)
{

}

void FileManagerFrame::initializeRightPanel(wxSizer * sizer)
{

}


