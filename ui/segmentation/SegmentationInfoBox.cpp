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
    wxGridSizer * grid = new wxGridSizer(6,2,5,5);

    _image_name = new wxStaticText(this, wxID_ANY, "-----");
    _image_size = new wxStaticText(this, wxID_ANY, "-----");
    _mouse_x = new wxStaticText(this, wxID_ANY, "-----");
    _mouse_y = new wxStaticText(this, wxID_ANY, "-----");
    _size_box = new wxComboBox(this, ID::ZOOM_BOX, wxT("100%"));
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
    _image_scale_box->Disable();
}

void SegmentationInfoBox::setZoomBox(int min, int max, int interval)
{
    _min = min;
    _max = max;
    _interval = interval;

    int i = 25;
    for (; i <= 500; i += 25) {
        _size_box->Append(wxString::Format("%d%%",i));
    }
}

void SegmentationInfoBox::setImageName(wxString const & name)
{
    _image_name->SetLabel(name);
}

void SegmentationInfoBox::setImageSize(wxString const & size)
{
    _image_size->SetLabel(size);
}

void SegmentationInfoBox::setMouseInfo(wxPoint const & point)
{
    _mouse_x->SetLabel(wxString::Format("%d",point.x));
    _mouse_y->SetLabel(wxString::Format("%d",point.y));
}

void SegmentationInfoBox::zoomIn()
{
    int cur_zoom = getZoom() + _interval;

    if (cur_zoom > _max) {
        cur_zoom = _max;
    }

    changeZoomBox(cur_zoom);
}

void SegmentationInfoBox::zoomOut()
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

void SegmentationInfoBox::changeZoomBox(int ratio)
{
    if (ratio < 10 || _max < ratio) {
        return;
    }

    _size_box->ChangeValue(wxString::Format("%d%%", ratio));
}

int SegmentationInfoBox::getZoom()
{
    wxString value = _size_box->GetValue();

    return wxAtoi(value.SubString(0, value.length() - 1));
}

int SegmentationInfoBox::getImageScale()
{
    wxString value = _size_box->GetValue();

    return wxAtoi(value.SubString(0, value.length() - 1));
}
