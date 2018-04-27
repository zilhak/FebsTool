#include "SettingFrame.hpp"

BEGIN_EVENT_TABLE(SettingFrame, wxDialog)
    EVT_TREE_SEL_CHANGED(wxID_ANY, SettingFrame::onSelect)
END_EVENT_TABLE()

SettingFrame::SettingFrame(wxWindow * parent, wxWindowID id) : wxDialog(parent, id, wxT("Setting"))
{
    SetMinSize(wxSize(640, 480));
    SetSize(wxSize(640, 480));

    initialize();
}

SettingFrame::~SettingFrame()
{
    _list_manager->saveConfig();
}

void SettingFrame::initialize()
{
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    initializeList(h_sizer);
    initializePanel(h_sizer);

    SetSizer(h_sizer);
}

void SettingFrame::initializeList(wxBoxSizer * h_sizer)
{
    _list_manager = new SettingPanelManager(this, wxID_ANY);

    h_sizer->Add(_list_manager, 0, wxALL | wxEXPAND, 10);
}

void SettingFrame::initializePanel(wxBoxSizer * h_sizer)
{
    wxBoxSizer * panel_sizer = new wxBoxSizer(wxVERTICAL);
    _target_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);

    _target_panel->SetSizer(panel_sizer);

    h_sizer->Add(_target_panel, 1, wxEXPAND | wxALL, 10);

    _list_manager->initializeManager(_target_panel, panel_sizer);
    _list_manager->open(wxT("General Setting"));
    _target_panel->Layout();
}

void SettingFrame::onSelect(wxTreeEvent & event)
{
    _list_manager->open(_list_manager->GetItemText(event.GetItem()));
    _target_panel->Layout();
}
