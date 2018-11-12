//
// Created by bogonets on 2018. 4. 6..
//

#include "SettingPanelManager.hpp"
#include "NameBoxSetting.hpp"
#include "GeneralSetting.hpp"
#include "KeyBindSetting.hpp"

BEGIN_EVENT_TABLE(SettingPanelManager, wxTreeCtrl)
END_EVENT_TABLE()

SettingPanelManager::SettingPanelManager(wxWindow * parent, wxWindowID id, ObjectType type)
        : wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxTR_HIDE_ROOT | wxSIMPLE_BORDER)
{
    _config = config::loadConfig(type);
}

SettingPanelManager::~SettingPanelManager()
{
    /* EMPTY */
}

void SettingPanelManager::initializeManager(wxPanel * target_panel, wxSizer * target_sizer)
{
    if (_initialized) {
        return;
    }

    wxTreeItemId root = AddRoot("Root");

    addToList(Panel(new GeneralSetting(target_panel, _config)), wxT("General Setting"), root);
    addToList(Panel(new NameBoxSetting(target_panel, _config)), wxT("Object Color Setting"), root);
    addToList(Panel(new KeyBindSetting(target_panel, _config)), wxT("Key Bind Setting"), root);

    for (auto pair : _map) {
        target_sizer->Add(pair.second.get(), 1, wxEXPAND);
        pair.second.get()->Show(false);
    }

    ExpandAll();
    _initialized = true;
}

wxTreeItemId SettingPanelManager::addToList(Panel const & panel, wxString const & name, wxTreeItemId const & parent)
{
    if (panel != nullptr) {
        panel.get()->SetName(name);
        _map.insert(std::make_pair(name, panel));
    }
    
    return AppendItem(parent, name);
}

bool SettingPanelManager::open(wxString const & name)
{
    if (_current_open_panel != "NONE") {
        _map.find(_current_open_panel)->second->Show(false);
    }

    auto pair = _map.find(name);
    if (pair != _map.end()) {
        pair->second->Show(true);
        _current_open_panel = pair->first;
    }

    return true;
}

void SettingPanelManager::saveConfig()
{
    config::saveConfig(_config);
}

