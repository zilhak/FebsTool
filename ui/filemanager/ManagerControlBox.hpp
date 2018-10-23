//
// Created by bogonets on 2018. 6. 21..
//

#ifndef FEPSTOOL_MANAGERCONTROLBOX_HPP
#define FEPSTOOL_MANAGERCONTROLBOX_HPP

#include <wx/wx.h>
#include <wx/spinctrl.h>

class ManagerControlBox : public wxPanel
{
public:
    wxStaticText * _text_filename;

public:
    wxSpinCtrl * _spin_row;
    wxSpinCtrl * _spin_col;

public:
    ManagerControlBox(wxWindow * parent, wxWindowID winid);
    virtual ~ManagerControlBox();

private:
    void initialize();
    void initializeInfo(wxSizer * sizer);
    void initializeControl(wxSizer * sizer);
    void initializeData();


};


#endif //FEPSTOOL_MANAGERCONTROLBOX_HPP
