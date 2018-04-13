//
// Created by bogonets on 2018. 4. 12..
//

#ifndef FEPSTOOL_FILEOPENPANEL_HPP
#define FEPSTOOL_FILEOPENPANEL_HPP

#include <wx/wx.h>
#include <data/ThemeData.hpp>
#include <data/EventData.hpp>

class FileOpenPanel : public wxPanel
{
public:
    enum ButtonID : int {
        BUTTON_OPEN = 1,
        BUTTON_SETTING
    };

public:
    DECLARE_EVENT_TABLE();

private:
    wxString _file_name = "";
    wxString _file_ext = "";
    wxString _path = "";

public:
    FileOpenPanel(wxWindow * parent, wxWindowID id);
    ~FileOpenPanel();

public:
    void initialize();

public:
    void onOpenButton(wxCommandEvent & event);
    void onSettingButton(wxCommandEvent & event);
    void onOpen(wxCommandEvent & event);

public:
    wxString getFileName(bool full_name = true) const;
    wxString getDir(bool add_slash = false) const;
    wxString getFullPath() const;
};


#endif //FEPSTOOL_FILEOPENPANEL_HPP
