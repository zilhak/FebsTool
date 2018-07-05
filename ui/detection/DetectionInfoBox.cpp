//
// Created by bogonets on 2018. 4. 3..
//

#include "DetectionInfoBox.hpp"

DetectionInfoBox::DetectionInfoBox(wxWindow * parent, wxWindowID id) : wxPanel(parent, id)
{
    initialize();
}

DetectionInfoBox::~DetectionInfoBox()
{
    /* EMPTY */
}

void DetectionInfoBox::initialize()
{
    initializeSetting();
    initializeComponent();
    initializeStyle();
}

void DetectionInfoBox::initializeSetting()
{
    _min = 20;
    _max = 500;
    _interval = 10;
}

void DetectionInfoBox::initializeComponent()
{
    wxGridSizer * grid = new wxGridSizer(6,2,3,3);

    _folder_name = new wxStaticText(this, wxID_ANY, "-----");
    _image_name = new wxStaticText(this, wxID_ANY, "-----");
    _image_size = new wxStaticText(this, wxID_ANY, "-----");
    _mouse_x = new wxStaticText(this, wxID_ANY, "-----");
    _mouse_y = new wxStaticText(this, wxID_ANY, "-----");
    _size_box = new wxComboBox(this, ID::ZOOM_BOX, wxT("100%"));
    _image_scale_box = new wxComboBox(this, wxID_ANY, wxT("3"));

    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Folder Name :")));
    grid->Add(_folder_name, 0, wxLEFT, 5);
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

void DetectionInfoBox::initializeStyle()
{
    /* EMPTY */
}

void DetectionInfoBox::setZoomBox(int min, int max, int interval)
{
    _min = min;
    _max = max;
    _interval = interval;

    int i = 25;
    for (; i <= 500; i += 25) {
        _size_box->Append(wxString::Format("%d%%",i));
    }
}

void DetectionInfoBox::setFolderName(wxString const & name)
{
    _folder_name->SetLabel(name);
}

void DetectionInfoBox::setImageName(wxString const & name)
{

}

void DetectionInfoBox::setImageSize(wxString const & size)
{

}

void DetectionInfoBox::setMouseInfo(wxPoint const & point)
{

}

void DetectionInfoBox::zoomIn()
{

}

void DetectionInfoBox::zoomOut()
{

}

void DetectionInfoBox::changeZoomBox(int ratio)
{

}

int DetectionInfoBox::getZoom()
{
    return 0;
}

int DetectionInfoBox::getImageScale()
{
    return 0;
}
