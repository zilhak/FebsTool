#ifndef FEPSTOOL_SETTINGFRAME_HPP
#define FEPSTOOL_SETTINGFRAME_HPP

#include <wx/wx.h>
#include "SettingPanelManager.hpp"

class SettingFrame : public wxDialog
{
private:
    DECLARE_EVENT_TABLE();

private:
    SettingPanelManager * _list_manager;
    wxPanel * _target_panel;
    ObjectType _type = ObjectType::SEGMENTATION;

public:
    SettingFrame(wxWindow * parent, wxWindowID id, ObjectType type = ObjectType::SEGMENTATION);
    ~SettingFrame();

private:
    void initialize();
    void initializeList(wxBoxSizer * h_sizer);
    void initializePanel(wxBoxSizer * h_sizer);
    void initializeTree(wxBoxSizer * h_sizer);

public:
    void save();

private:
    void onSelect(wxTreeEvent & event);
};


#endif //FEPSTOOL_SETTINGFRAME_HPP
