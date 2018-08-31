#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

class FindDialog : public wxDialog
{
public:
    wxDECLARE_EVENT_TABLE();

public:
    enum FINDDIALOG_ID {
        FIND = 90,
        OK
    };

private:
    wxTextCtrl * _find_ctrl;
    wxButton * _ok;

private:
    wxString _find_str = wxEmptyString;
    wxString _find_name;
    wxString _img_kind;
    wxString _result_number;
    std::vector<wxString> _file_list;

public:
    FindDialog(wxWindow * parent, wxWindowID id,
               const wxString& title,
               const std::vector<wxString> & file_list,
               const wxString & kind_image,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxDEFAULT_DIALOG_STYLE,
               const wxString& name = wxDialogNameStr);
    virtual ~FindDialog();

public:
    void initialize();

public:
    wxString getFindValue() const { return _find_str; }
    wxString makeFindName();

public:
    void onClickOK(wxCommandEvent & event);
    void onCharHook(wxKeyEvent & event);
};

#ifndef IMAGESEARCHER_UI_FINDDIALOG_H
#define IMAGESEARCHER_UI_FINDDIALOG_H

#endif //IMAGESEARCHER_UI_FINDDIALOG_H
