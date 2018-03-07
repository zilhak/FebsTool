//
// Created by zilhak on 18. 3. 7.
//
#ifndef IMAGESEARCHER_SELECTDIALOG_HPP
#define IMAGESEARCHER_SELECTDIALOG_HPP


#include <wx/wx.h>

class SelectDialog : public wxDialog
{
public:
    enum ButtonID : int {
        CLASSIFY = 1100,
        DETECTION,
        SEGMENTATION,
        FILE_MANAGER
    };

public:
    wxDECLARE_EVENT_TABLE();

private:
    int _selected_button;

public:
    SelectDialog();
    ~SelectDialog();

public:
    void initialize();

public:
    void onDialogButton(wxCommandEvent & event);

public:
    int getSelectedID();
};


#endif //IMAGESEARCHER_SELECTDIALOG_HPP
