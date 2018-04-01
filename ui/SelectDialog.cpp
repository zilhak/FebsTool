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
    wxBoxSizer * h_sizer = new wxBoxSizer(wxVERTICAL);

    wxButton * button_classify = new wxButton(this, ButtonID::CLASSIFY, "Classify");
    wxButton * button_detection = new wxButton(this, ButtonID::DETECTION, "Detection");
    wxButton * button_segmentation = new wxButton(this, ButtonID::SEGMENTATION, "Segmentation");
    wxButton * button_filemanager = new wxButton(this, ButtonID::FILE_MANAGER, "File Manager");

    h_sizer->Add(button_classify, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    h_sizer->Add(button_detection, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    h_sizer->Add(button_segmentation, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    h_sizer->Add(button_filemanager, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    SetSizer(h_sizer);
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