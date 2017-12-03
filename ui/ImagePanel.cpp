#include "ImagePanel.hpp"

wxBEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_PAINT(ImagePanel::onPaint)
    EVT_MOUSE_EVENTS(ImagePanel::onMouse)
wxEND_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id)
{
    /* EMPTY */
}

ImagePanel::~ImagePanel()
{
    /* EMPTY */
}

void ImagePanel::setBackgroundImage(wxString filepath, wxString filename)
{
    _image_filepath = filepath;
    _image_filename = filename;
    _background_bitmap = wxBitmap(filepath + "/" + filename);

    _image_height = _background_bitmap.GetHeight();
    _image_width = _background_bitmap.GetWidth();

    _is_ready = true;
    Refresh();
}

void ImagePanel::save()
{
    if (_background_bitmap.IsOk() && mode == STATUS::IDLE) {
        saveToXml(_x1 - _image_x,
                  _y1 - _image_y,
                  _x2 - _image_x,
                  _y2 - _image_y,
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
    if (mode == STATUS::IDLE && _image_y < _mouse_y && _mouse_y < _image_y + _background_bitmap.GetHeight()) {
        dc.DrawLine(_image_x,
                    _mouse_y,
                    _image_x + _background_bitmap.GetWidth(),
                    _mouse_y);
    }
    if (mode == STATUS::IDLE && _image_x < _mouse_x && _mouse_x < _image_x + _background_bitmap.GetWidth()) {
        dc.DrawLine(_mouse_x,
                    _image_y,
                    _mouse_x,
                    _image_y + _background_bitmap.GetHeight());
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
    } else if (x > _image_x + _image_width) {
        x = _image_x + _image_width;
    }
    if (y < _image_y) {
        y = _image_y;
    } else if (y > _image_y + _image_height) {
        y = _image_y + _image_height;
    }
}