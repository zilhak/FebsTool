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
    wxFlexGridSizer * grid = new wxFlexGridSizer(10,2,3,3);

    grid->AddGrowableCol(0);
    grid->AddGrowableCol(1);

    _folder_name = new wxStaticText(this, wxID_ANY, "-----");
    _folder_image_count = new wxStaticText(this, wxID_ANY, "-----");
    _folder_image_idx = new wxStaticText(this, wxID_ANY, "-----");
    _image_name = new wxStaticText(this, wxID_ANY, "-----");
    _image_size = new wxStaticText(this, wxID_ANY, "-----");
    _mouse_x = new wxStaticText(this, wxID_ANY, "-----");
    _mouse_y = new wxStaticText(this, wxID_ANY, "-----");
    _object_size = new wxStaticText(this, wxID_ANY, "0 x 0 x 0 x 0");
    _size_box = new wxComboBox(this, ID::ZOOM_BOX, wxT("100%"));
    _image_scale_box = new wxComboBox(this, wxID_ANY, wxT("3"));

    for (int i = 1; i <= 3; ++i) {
        _image_scale_box->Append(std::to_string(i));
    }

    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Folder Name :")));
    grid->Add(_folder_name, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Folder Size :")));
    grid->Add(_folder_image_count, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Image Index :")));
    grid->Add(_folder_image_idx, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Image Name :")));
    grid->Add(_image_name, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Image Size :")));
    grid->Add(_image_size, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Mouse X :")));
    grid->Add(_mouse_x, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Mouse Y :")));
    grid->Add(_mouse_y, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Object :")));
    grid->Add(_object_size, 0, wxLEFT, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Zoom :")), wxALIGN_CENTER_VERTICAL);
    grid->Add(_size_box, 0, wxLEFT | wxEXPAND, 5);
    grid->Add(new wxStaticText(this, wxID_ANY, wxT("Scale :")), wxALIGN_CENTER_VERTICAL);
    grid->Add(_image_scale_box, 0, wxLEFT | wxEXPAND, 5);

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

    for (int i = min; i <= max; i += interval) {
        _size_box->Append(wxString::Format("%d%%",i));
    }
}

void DetectionInfoBox::setFolderName(wxString const & name)
{
    _folder_name->SetLabel(name);
}

void DetectionInfoBox::setFolderSize(int size)
{
    _folder_image_count->SetLabel(wxString::Format("%d", size));
}

void DetectionInfoBox::setImageIndex(int index)
{
    _folder_image_idx->SetLabel(wxString::Format("%d", index));
}

void DetectionInfoBox::setImageName(wxString const & name)
{
    _image_name->SetLabel(name);
}

void DetectionInfoBox::setImageSize(wxString const & size)
{
    _image_size->SetLabel(size);
}

void DetectionInfoBox::setMouseInfo(wxPoint const & point)
{
    _mouse_x->SetLabel(std::to_string(point.x));
    _mouse_y->SetLabel(std::to_string(point.y));
}

void DetectionInfoBox::setObjectInfo(wxRect const & rect)
{
    _object_size->SetLabel(wxString::Format("%d x %d x %d x %d", rect.x, rect.y, rect.width, rect.height));
}

void DetectionInfoBox::decreaseFolderSize()
{
    int folder_size = wxAtoi(_folder_image_count->GetLabelText());
    if (folder_size > 0) {
        --folder_size;
    }
    _folder_image_count->SetLabel(std::to_string(folder_size));
}

void DetectionInfoBox::zoomIn()
{
    int cur_zoom = getZoom() + _interval;

    if (cur_zoom > _max) {
        cur_zoom = _max;
    }

    changeZoomBox(cur_zoom);
}

void DetectionInfoBox::zoomOut()
{
    int cur_zoom = getZoom() - _interval;

    if (cur_zoom < 10) {
        cur_zoom = 10;
    }
    if (cur_zoom < _min) {
        cur_zoom = _min;
    }

    changeZoomBox(cur_zoom);
}

void DetectionInfoBox::changeZoomBox(int ratio)
{
    if (ratio < 10 || _max < ratio) {
        return;
    }

    _size_box->ChangeValue(wxString::Format("%d%%", ratio));
}

int DetectionInfoBox::getZoom()
{
    wxString value = _size_box->GetValue();

    return wxAtoi(value.SubString(0, value.length() - 1));
}

int DetectionInfoBox::getImageScale()
{
    wxString value = _image_scale_box->GetValue();

    return wxAtoi(value);
}
