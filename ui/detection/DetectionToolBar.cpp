//
// Created by bogonets on 2018. 4. 3..
//

#include "DetectionToolBar.hpp"

DetectionToolBar::DetectionToolBar(wxWindow * parent, wxWindowID id) : wxPanel(parent, id)
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
    wxBoxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);

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
    _type_box->Append(wxT("car"));
    _type_box->Append(wxT("bus"));
    _type_box->Append(wxT("truck"));
    _type_box->Select(0);

    _diff_box->SetEditable(false);
    _diff_box->Append(wxT("0"));
    _diff_box->Append(wxT("1"));
    _diff_box->Append(wxT("2"));
    _diff_box->Select(0);
}

wxString DetectionToolBar::getType()
{
    return _type_box->GetValue();
}

int DetectionToolBar::getDifficult()
{
    return wxAtoi(_diff_box->GetValue());
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

void DetectionToolBar::setType(int num)
{
    if (_type_box->GetCount() > num) {
        _type_box->Select(num);
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

void DetectionToolBar::setNameBox(std::vector<Name> const & class_list, wxString const & default_class)
{
    _type_box->Clear();

    for (auto const & name : class_list) {
        _type_box->Append(name.name);
    }

    _type_box->SetValue(default_class);
}