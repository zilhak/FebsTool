
#ifndef FEPSTOOL_KEYBINDSETTING_HPP
#define FEPSTOOL_KEYBINDSETTING_HPP

#include <wx/wx.h>
#include <data/FebsStruture.hpp>


class KeyBindSetting : public wxPanel
{
private:
    enum BUTTON_ID {
        DEFAULT,
        CANCEL
    };

public:
    KeyBindSetting(wxWindow * parent, ConfigData & data);
    ~KeyBindSetting();

public:
    void initialize();
    void initializeComponent(wxSizer * sizer);
    void initializeMessage(wxSizer * sizer);
    void initializeButtons(wxSizer * sizer);
    void initializeData();
};


#endif //FEPSTOOL_KEYBINDSETTING_HPP
