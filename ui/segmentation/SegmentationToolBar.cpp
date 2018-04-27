#include "SegmentationToolBar.hpp"

SegmentationToolBar::SegmentationToolBar(wxWindow * parent, wxWindowID id) : wxPanel(parent, id)
{
    initialize();
}

SegmentationToolBar::~SegmentationToolBar()
{
    /* EMPTY */
}

void SegmentationToolBar::initialize()
{
    initializeComponent();
    initializeSetting();
    initializeStyle();
}

void SegmentationToolBar::initializeComponent()
{
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    _name_box = new wxComboBox(this, wxID_ANY, "NULL", wxDefaultPosition, wxSize(100, -1));
    _pose_box = new wxComboBox(this, wxID_ANY, "0");
    _trun_box = new wxComboBox(this, wxID_ANY, "0");
    _diff_box = new wxComboBox(this, wxID_ANY, "0");

    h_sizer->Add(new wxStaticText(this, wxID_ANY, "name :"), 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    h_sizer->Add(_name_box, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    h_sizer->Add(new wxStaticText(this, wxID_ANY, "pose :"), 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    h_sizer->Add(_pose_box, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    h_sizer->Add(new wxStaticText(this, wxID_ANY, "truncate :"), 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    h_sizer->Add(_trun_box, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    h_sizer->Add(new wxStaticText(this, wxID_ANY, "difficult :"), 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    h_sizer->Add(_diff_box, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

    SetSizer(h_sizer);
}

void SegmentationToolBar::initializeSetting()
{
    /* EMPTY */
}

void SegmentationToolBar::initializeStyle()
{
    SetBackgroundColour(COLOUR_TOOLBAR);
    _name_box->SetEditable(false);
    _pose_box->SetEditable(false);
    _trun_box->SetEditable(false);
    _diff_box->SetEditable(false);

    _pose_box->Disable();
    _trun_box->Disable();
    _diff_box->Disable();
}

void SegmentationToolBar::setNameBox(std::vector<Name> const & name_list, wxString const & name)
{
    _name_box->Clear();

    _name_list = name_list;
    for (auto const & type_name : name_list) {
        _name_box->Append(type_name.name);
    }
    _name_box->SetValue(name);
}

wxString SegmentationToolBar::getName()
{
    return _name_box->GetValue();
}

wxString SegmentationToolBar::getColour()
{
    for (auto const & item : _name_list) {
        if (item.name == _name_box->GetValue()) {
            return item.colour;
        }
    }

    return "65535";
}

wxString SegmentationToolBar::getPose()
{
    return _pose_box->GetValue();
}

wxString SegmentationToolBar::getTruncated()
{
    return _trun_box->GetValue();
}

int SegmentationToolBar::getDifficult()
{
    return wxAtoi(_diff_box->GetValue());
}
