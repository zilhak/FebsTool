

#include "KeyBindSetting.hpp"

KeyBindSetting::KeyBindSetting(wxWindow * parent, ConfigData & data) : wxPanel(parent)
{
    initialize();
}

KeyBindSetting::~KeyBindSetting()
{
    /* EMPTY */
}

void KeyBindSetting::initialize()
{
    wxBoxSizer * whole_sizer = new wxBoxSizer(wxVERTICAL);

    initializeComponent(whole_sizer);
    initializeMessage(whole_sizer);
    initializeButtons(whole_sizer);
    initializeData();

    SetSizer(whole_sizer);
}

void KeyBindSetting::initializeComponent(wxSizer * sizer)
{
    wxScrolledWindow * component_panel = new wxScrolledWindow(this, wxID_ANY);
    wxFlexGridSizer * grid_sizer = new wxFlexGridSizer(3, 10, 10);

    grid_sizer->Add(new wxStaticText(component_panel, wxID_ANY, wxT("test")));

    component_panel->SetSizer(grid_sizer);

    sizer->Add(component_panel, 1, wxEXPAND | wxALL, 10);
}

void KeyBindSetting::initializeMessage(wxSizer * sizer)
{
    sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Click!!")), 0, wxALIGN_CENTER);
}

void KeyBindSetting::initializeButtons(wxSizer * sizer)
{
    wxPanel * buttons_panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer * buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton * default_button = new wxButton(buttons_panel, wxID_ANY, "Default");
    wxButton * cancel_button = new wxButton(buttons_panel, wxID_ANY, "Cancel");

    buttons_sizer->Add(default_button, 0);
    buttons_sizer->Add(cancel_button, 0, wxLEFT, 10);

    buttons_panel->SetSizer(buttons_sizer);

    sizer->Add(buttons_panel, 0, wxALIGN_RIGHT);
}

void KeyBindSetting::initializeData()
{

}