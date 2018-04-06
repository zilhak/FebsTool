#ifndef FEPSTOOL_SETTINGFRAME_HPP
#define FEPSTOOL_SETTINGFRAME_HPP

#include <wx/wx.h>

class SettingFrame : public wxDialog
{
private:


public:
    SettingFrame();
    ~SettingFrame();

private:
    void initialize();
    void initializeList(wxBoxSizer * h_sizer);
    void initializePanel(wxBoxSizer * h_sizer);
};


#endif //FEPSTOOL_SETTINGFRAME_HPP
