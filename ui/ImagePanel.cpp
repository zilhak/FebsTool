#include "ImagePanel.hpp"

wxBEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_PAINT(ImagePanel::onPaint)
    EVT_MOUSE_EVENTS(ImagePanel::onMouse)
wxEND_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id)
{
    /* EMPTY */
    ::wxInitAllImageHandlers();
}

ImagePanel::~ImagePanel()
{
    /* EMPTY */
}

void ImagePanel::setBackgroundImage(wxString filepath, wxString filename)
{
    _x1 = -1;
    _y1 = -1;
    _x2 = -1;
    _y2 = -1;

    _image_filepath = filepath;
    _image_filename = filename;

    wxString path = wxString::Format("%s/%s",filepath, filename);
    wxImage image(path, wxBITMAP_TYPE_JPEG);
    _image_height = image.GetHeight();
    _image_width = image.GetWidth();
    image.Rescale(static_cast<int>(static_cast<double>(image.GetWidth()) * _scale_setting),
                  static_cast<int>(static_cast<double>(image.GetHeight()) * _scale_setting),
                  wxIMAGE_QUALITY_HIGH);
    _background_bitmap = wxBitmap(image);
    _bitmap_width = _background_bitmap.GetWidth();
    _bitmap_height = _background_bitmap.GetHeight();
    _is_ready = true;
    Refresh();
}

void ImagePanel::save()
{
    if (_background_bitmap.IsOk() && mode == STATUS::IDLE) {
        saveToXml(static_cast<int>(static_cast<double>(_x1 - _image_x) / _scale_setting),
                  static_cast<int>(static_cast<double>(_y1 - _image_y) / _scale_setting),
                  static_cast<int>(static_cast<double>(_x2 - _image_x) / _scale_setting),
                  static_cast<int>(static_cast<double>(_y2 - _image_y) / _scale_setting),
                  std::string(_image_filepath.c_str()),
                  std::string(_image_filename.c_str()),
                  _image_height, _image_width, 1);
    }
}

void ImagePanel::changeScale(double scale)
{
    _scale_setting = scale;
}

void ImagePanel::onPaint(wxPaintEvent & event)
{
    if (!_background_bitmap.IsOk()) {
        return;
    }

    wxPaintDC dc(this);
    dc.DrawBitmap(_background_bitmap , _image_x, _image_y);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxRED_PEN);
    if (mode == STATUS::IDLE && _image_y < _mouse_y && _mouse_y < _image_y + _bitmap_height) {
        dc.DrawLine(_image_x,
                    _mouse_y,
                    _image_x + _bitmap_width,
                    _mouse_y);
    }
    if (mode == STATUS::IDLE && _image_x < _mouse_x && _mouse_x < _image_x + _bitmap_width) {
        dc.DrawLine(_mouse_x,
                    _image_y,
                    _mouse_x,
                    _image_y + _bitmap_height);
    }
    if (_x1 != -1) {
        dc.SetPen(*wxCYAN_PEN);
        dc.DrawRectangle(_x1, _y1, _x2 - _x1, _y2 - _y1);
        dc.DrawRectangle(_x1 - 1, _y1 - 1, _x2 - _x1 + 2, _y2 - _y1 + 2);
    }
}

void ImagePanel::onMouse(wxMouseEvent & event)
{
    if (mode == STATUS::IDLE) {
        _mouse_x = event.GetX();
        _mouse_y = event.GetY();
    }

    if (event.LeftDown() && mode == STATUS::IDLE) {
        _x1 = event.GetX();
        _x2 = event.GetX();
        _y1 = event.GetY();
        _y2 = event.GetY();
        mode = STATUS::MOUSE_DOWN;
    } else if (event.LeftIsDown() && mode == STATUS::MOUSE_DOWN) {
        _x2 = event.GetX();
        _y2 = event.GetY();
    } else if (event.LeftUp() && mode == STATUS::MOUSE_DOWN) {
        int tmp = 0;
        if (_x2 < _x1) {
            tmp = _x1;
            _x1 = _x2;
            _x2 = tmp;
        }
        if (_y2 < _y1) {
            tmp = _y1;
            _y1 = _y2;
            _y2 = tmp;
        }
        mode = STATUS::IDLE;
        _mouse_x = event.GetX();
        _mouse_y = event.GetY();
    }
    locationAdjust(_x1, _y1);
    locationAdjust(_x2, _y2);
    locationAdjust(_mouse_x, _mouse_y);
    Refresh();
    event.Skip(false);
}

void ImagePanel::locationAdjust(int & x, int & y)
{
    if (x < _image_x) {
        x = _image_x;
    } else if (x > _image_x + _bitmap_width) {
        x = _image_x + _bitmap_width;
    }
    if (y < _image_y) {
        y = _image_y;
    } else if (y > _image_y + _bitmap_height) {
        y = _image_y + _bitmap_height;
    }
}