//
// Created by bogonets on 2018. 4. 12..
//

#include "SegmentationInfoBox.hpp"

SegmentationInfoBox::SegmentationInfoBox(wxWindow * parent, wxWindowID id) : wxPanel(parent, id)
{
    initialize();
}

SegmentationInfoBox::~SegmentationInfoBox()
{
    /* EMPTY */
}

void SegmentationInfoBox::initialize()
{
    initializeComponent();
    initializeSetting();
    initializeStyle();
}

void SegmentationInfoBox::initializeComponent()
{
    wxGridSizer * grid = new wxGridSizer(3,2,5,5);

    _image_name = new wxStaticText(this, wxID_ANY, "-----");
    _image_size = new wxStaticText(this, wxID_ANY, "-----");
    _mouse_x = new wxStaticText(this, wxID_ANY, "-----");
    _mouse_y = new wxStaticText(this, wxID_ANY, "-----");
    _size_box = new wxComboBox(this, wxID_ANY, wxT("100"));
    _image_scale_box = new wxComboBox(this, wxID_ANY, wxT("3"));

    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Image Name :")));
    grid->Add(_image_name, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Image Size :")));
    grid->Add(_image_size, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Mouse X :")));
    grid->Add(_mouse_x, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Mouse Y :")));
    grid->Add(_mouse_y, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Zoom :")));
    grid->Add(_size_box, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Scale :")));
    grid->Add(_image_scale_box, 0, wxLEFT, 5);

    SetSizer(grid);
}

void SegmentationInfoBox::initializeSetting()
{

}

void SegmentationInfoBox::initializeStyle()
{
    SetBackgroundColour(COLOUR_INFO);
    _size_box->SetEditable(false);
    _image_scale_box->SetEditable(false);
}