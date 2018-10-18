#ifndef FEPSTOOL_SETTINGPANELMANAGER_HPP
#define FEPSTOOL_SETTINGPANELMANAGER_HPPuto inclue

#include <wx/wx.h>
#include <wx/treectrl.h>
#include "../../data/XmlConfig.hpp"
#include <memory>
#include <map>

class SettingPanelManager : public wxTreeCtrl
{
private:
    using Panel = std::shared_ptr<wxPanel>;
    using PanelMap = std::map<wxString, Panel>;

private:
    DECLARE_EVENT_TABLE();

private:
    wxPanel * _target_panel;
    PanelMap _map;
    wxString _current_open_panel = "NONE";
    ConfigData _config;

private:
    bool _initialized = false;

public:
    SettingPanelManager(wxWindow * parent, wxWindowID id, ObjectType type = ObjectType::SEGMENTATION);
    ~SettingPanelManager();

public:
    void initializeManager(wxPanel * target_panel, wxSizer * target_sizer);

public:
    wxTreeItemId addToList(Panel const & panel, wxString const & name, wxTreeItemId const & parent);
    bool open(wxString const & name);
    void saveConfig();
};


#endif //FEPSTOOL_SETTINGPANELMANAGER_HPP
