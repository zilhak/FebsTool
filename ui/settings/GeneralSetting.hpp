#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include "../../data/XmlConfig.hpp"

class GeneralSetting : public wxPanel
{
public:
    enum ID : int {

    };

public:
    DECLARE_EVENT_TABLE();

public:
    ConfigData * _config;

public:
    wxSpinCtrl * _zoom_min_ctrl;
    wxSpinCtrl * _zoom_max_ctrl;
    wxSpinCtrl * _zoom_interval_ctrl;
    wxSpinCtrl * _min_size_x;
    wxSpinCtrl * _min_size_y;
    wxSpinCtrl * _max_size_x;
    wxSpinCtrl * _max_size_y;

public:
    GeneralSetting(wxWindow * parent, ConfigData & data);
    ~GeneralSetting();

private:
    void initialize();
    void initializeTitle(wxBoxSizer * v_sizer, wxString const & str);
    void initializeInterval(wxBoxSizer * v_sizer);
    void initializeSizeLimit(wxBoxSizer * v_sizer);

private:
    void onSpinCtrl(wxSpinEvent & event);
};