//
// Created by bogonets on 2018. 4. 12..
//

#include <wx-3.1/wx/filename.h>
#include <ui/component/FileOpenPanel.hpp>

BEGIN_EVENT_TABLE(FileOpenPanel, wxPanel)
    EVT_BUTTON(ButtonID::BUTTON_OPEN, FileOpenPanel::onOpenButton)
    EVT_BUTTON(ButtonID::BUTTON_SETTING, FileOpenPanel::onSettingButton)
    EVT_COMMAND(wxID_ANY, FILE_OPEN_EVENT, FileOpenPanel::onOpen)
END_EVENT_TABLE()

FileOpenPanel::FileOpenPanel(wxWindow * parent, wxWindowID id) : wxPanel(parent, id)
{
    initialize();
}

FileOpenPanel::~FileOpenPanel()
{
    /* EMPTY */
}

void FileOpenPanel::initialize()
{
    SetBackgroundColour(COLOUR_TOOLBAR);
    wxButton * open = new wxButton(this, ButtonID::BUTTON_OPEN, wxT("Open"));
    wxButton * setting = new wxButton(this, ButtonID::BUTTON_SETTING, wxT("Setting"));

    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    h_sizer->Add(open, 1, wxALL | wxALIGN_CENTER, 5);
    h_sizer->Add(setting, 1, wxALL | wxALIGN_CENTER, 5);

    SetSizer(h_sizer);
}

void FileOpenPanel::onOpenButton(wxCommandEvent & event)
{
    wxFileDialog * find = new wxFileDialog(this,
                                           _("Open jpg file"),
                                           "",
                                           "",
                                           "JPG files (*.jpg;*.jpeg)|*.jpg;*.jpeg");
    if (find->ShowModal() == wxID_CANCEL) {
        return;
    }

    wxFileName file_name(wxString::FromUTF8(find->GetPath()));
    _file_name = wxString::FromUTF8(file_name.GetName().ToStdString().c_str());
    _file_ext = file_name.GetExt();
    _path = file_name.GetPath();

    wxCommandEvent open_event(FILE_OPEN_EVENT, GetId());
    open_event.SetEventObject(this);
    ProcessWindowEvent(open_event);
}

void FileOpenPanel::onSettingButton(wxCommandEvent & event)
{
    
}

void FileOpenPanel::onOpen(wxCommandEvent & event)
{
    std::cout << "here" << std::endl;
    std::cout << event.ShouldPropagate() <<std::endl;
    event.Skip();
}

wxString FileOpenPanel::getFileName(bool full_name) const
{
    if (full_name) {
        return _file_name + "." + _file_ext;
    } else {
        return _file_name;
    }
}

wxString FileOpenPanel::getDir(bool add_slash) const
{
    if (add_slash) {
        return _path + "/";
    } else {
        return _path;
    }
}

wxString FileOpenPanel::getFullPath() const
{
    return getDir(true) + getFileName();
}
