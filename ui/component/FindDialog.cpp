
#include <ui/component/FindDialog.hpp>
#include <wx/filename.h>
#include <x264.h>

constexpr static int const BORDER = 5;

wxBEGIN_EVENT_TABLE(FindDialog, wxDialog)
    EVT_BUTTON(FINDDIALOG_ID::OK, FindDialog::onClickOK)
    EVT_CHAR_HOOK(FindDialog::onCharHook)
wxEND_EVENT_TABLE()

FindDialog::FindDialog(wxWindow *parent, wxWindowID id, const wxString & title, const std::vector<wxString> & file_list, const wxString & kind_img, const wxPoint & pos,
                       const wxSize & size, long style, const wxString & name)
          : wxDialog(parent, id, title, pos, size, style, name), _file_list(file_list), _img_kind(kind_img)
{
    initialize();
}

FindDialog::~FindDialog()
{
    //empty.
}

void FindDialog::initialize()
{
    if (_img_kind == wxT("key")) {
        _img_kind = wxT("_key_");
    } else if ( _img_kind == wxT("r")) {
        _img_kind = wxT("_r01_");
    }

    wxString original_file_name = _file_list.at(0);
    _find_name = original_file_name.Remove(original_file_name.find(wxT("_key_"), 0));

    wxFileName file(_file_list.at(0));
    wxBoxSizer * hszer = new wxBoxSizer(wxHORIZONTAL);
    _find_ctrl = new wxTextCtrl(this, FINDDIALOG_ID::FIND, wxEmptyString, wxDefaultPosition, wxSize(250, 25));
    _ok = new wxButton(this, FINDDIALOG_ID::OK, wxT("Find"));

    hszer->Add(_find_ctrl, 0, wxALL, BORDER);
    hszer->AddSpacer(BORDER);
    hszer->Add(_ok, 0, wxALL, BORDER);

    this->SetSizerAndFit(hszer);
}

wxString FindDialog::makeFindName()
{
    wxString result;
    wxString ctrl_value = _find_ctrl->GetValue();
    //if (wxAtoi(ctrl_value) < 10) {
    //    _result_number = wxT("0000") + _find_ctrl->GetValue();
    //} else if (wxAtoi(ctrl_value) < 100) {
    //    _result_number = wxT("000") + _find_ctrl->GetValue();
    //} else if (wxAtoi(ctrl_value) < 1000) {
    //    _result_number = wxT("00") + _find_ctrl->GetValue();
    //} else if (wxAtoi(ctrl_value) < 10000) {
    //    _result_number = wxT("0") + _find_ctrl->GetValue();
    //} else {
    //    _result_number = _find_ctrl->GetValue();
    //}
    _result_number = wxString::Format(wxT("%5d"), _find_ctrl->GetValue());
    std::cout << "Find Image Number : " << _result_number.ToStdString() << std::endl;

    result = _find_name + _img_kind + _result_number;

    return result;
}

void FindDialog::onClickOK(wxCommandEvent & event)
{
    _find_str = makeFindName();
    wxDialog::EndModal(EXIT_SUCCESS);
}

void FindDialog::onCharHook(wxKeyEvent & event)
{
    if (event.GetKeyCode() == WXK_RETURN) {
        _find_str = makeFindName();
        wxDialog::EndModal(EXIT_SUCCESS);
    } else if (event.GetKeyCode() == WXK_ESCAPE) {
        wxDialog::EndModal(EXIT_FAILURE);
    }
    event.Skip();
}
