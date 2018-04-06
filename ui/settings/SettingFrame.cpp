//
// Created by bogonets on 2018. 4. 6..
//

#include "SettingFrame.hpp"

SettingFrame::SettingFrame(wxWindow * parent, wxWidowID id) : wxDialog(parent, id)
{
    initialize();
}

SettingFrame::~SettingFrame()
{
    /* EMPTY */
}

void SettingFrame::initialize()
{
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    initializeList(h_sizer);
    initializePanel(h_sizer);
}

void SettingFrame::initializeList(wxBoxSizer * h_sizer)
{
    
}

void SettingFrame::initializePanel(wxBoxSizer * h_sizer)
{

}

