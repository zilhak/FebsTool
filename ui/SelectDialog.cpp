//
// Created by zilhak on 18. 3. 7.
//

#include <ui/SelectDialog.hpp>

BEGIN_EVENT_TABLE(SelectDialog, wxDialog)
    EVT_BUTTON(wxID_ANY, SelectDialog::onDialogButton)
END_EVENT_TABLE()

SelectDialog::SelectDialog() : wxDialog(nullptr, wxID_ANY, "Select Tool")
{
    initialize();
}

SelectDialog::~SelectDialog()
{
    /* EMPTY */
}

void SelectDialog::initialize()
{
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);

//    wxButton * button_classify = new wxButton(this, ButtonID::CLASSIFY, wxT("Classify"), wxDefaultPosition, wxSize(200,50));
    wxButton * button_detection = new wxButton(this, ButtonID::DETECTION, "Detection", wxDefaultPosition, wxSize(200,50));
    wxButton * button_segmentation = new wxButton(this, ButtonID::SEGMENTATION, "Segmentation", wxDefaultPosition, wxSize(200,50));
    wxButton * button_filemanager = new wxButton(this, ButtonID::FILE_MANAGER, "File Manager", wxDefaultPosition, wxSize(200,50));

//    v_sizer->Add(button_classify, 0, wxTOP | wxLEFT | wxRIGHT, 10);
    v_sizer->Add(button_detection, 0, wxTOP | wxLEFT | wxRIGHT, 10);
    v_sizer->Add(button_segmentation, 0, wxTOP | wxLEFT | wxRIGHT, 10);
    v_sizer->Add(button_filemanager, 0, wxALL, 10);

    SetSizer(v_sizer);

    Fit();
}

void SelectDialog::onDialogButton(wxCommandEvent & event)
{
    _selected_button = event.GetId();
    EndModal(wxID_OK);
}

int SelectDialog::getSelectedID()
{
    return _selected_button;
}