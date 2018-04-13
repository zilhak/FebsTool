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

    _name_box = new wxComboBox(this, wxID_ANY, "NULL");
    _pose_box = new wxComboBox(this, wxID_ANY, "NULL");
    _trun_box = new wxComboBox(this, wxID_ANY, "NULL");
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
}