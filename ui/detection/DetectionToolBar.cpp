//
// Created by bogonets on 2018. 4. 3..
//

#include "DetectionToolBar.hpp"

DetectionToolBar::DetectionToolBar()
{
    initialize();
}

DetectionToolBar::~DetectionToolBar()
{
    /* EMPTY */
}

void DetectionToolBar::initialize()
{
    initializeComponent();
    initializeDefaultData();
}

void DetectionToolBar::initializeComponent()
{
    wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

    _type_box = new wxComboBox(this, wxID_ANY);
    _diff_box = new wxComboBox(this, wxID_ANY);

    sizer->Add(new wxStaticText(this, wxID_ANY, "Type:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    sizer->Add(_type_box, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Difficult:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    sizer->Add(_diff_box, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);

    SetSizer(sizer);
}

void DetectionToolBar::initializeDefaultData()
{
    _type_box->SetEditable(false);
    _type_box->Append(wxT("mouse"));
    _type_box->Append(wxT("smoke"));
    _type_box->Append(wxT("explosion"));
    _type_box->Select(0);

    _diff_box->SetEditable(false);
    _diff_box->Append(wxT("0"));
    _diff_box->Append(wxT("1"));
    _diff_box->Append(wxT("2"));
    _diff_box->Select(0);
}

wxString DetectionToolBar::getType()
{
    return wxString();
}

wxString DetectionToolBar::getDifficult()
{
    return wxString();
}

void DetectionToolBar::setType(wxString type)
{
    for (int i = 0; i < _type_box->GetCount(); ++i) {
        if (_type_box->GetString(i) == type) {
            _type_box->Select(i);
            return;
        }
    }
}

void DetectionToolBar::setDifficult(int difficult)
{
    for (int i = 0; i < _diff_box->GetCount(); ++i) {
        if (_diff_box->GetString(i) == wxString::Format("%d", difficult)) {
            _diff_box->Select(i);
            return;
        }
    }
}

