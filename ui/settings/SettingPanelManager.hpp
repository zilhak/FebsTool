#ifndef FEPSTOOL_SETTINGPANELMANAGER_HPP
#define FEPSTOOL_SETTINGPANELMANAGER_HPP

#include <wx/wx.h>

class SettingPanelManager : wxTreeCtrl
{
private:
    using Panel = std::shared_ptr<wxPanel>;

private:
    wxPanel * _target_panel;

public:
    SettingPanelManager(wxWindow * parent, wxWindowID id);
    ~SettingPanelManager();

public:
    void initializeManager(wxPanel * target_panel);
    wxTreeItemId addToList(Panel const & panel, wxString const & name, wxTreeItemId const & parent);
};


#endif //FEPSTOOL_SETTINGPANELMANAGER_HPP
