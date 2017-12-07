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
    _image_file.Assign(wxString::Format("%s/%s",filepath, filename));
    load();

    wxImage image(_image_file.GetFullPath(), wxBITMAP_TYPE_JPEG);
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

void ImagePanel::load()
{
    _temp_rect = loadFromXml(_image_file.GetPath(true) + _image_file.GetName());
}

void ImagePanel::save()
{
    if (_background_bitmap.IsOk() && mode == STATUS::IDLE) {
        cyRect check (
                static_cast<int>(static_cast<double>(_check.x1 - _image_x) / _scale_setting),
                static_cast<int>(static_cast<double>(_check.y1 - _image_y) / _scale_setting),
                static_cast<int>(static_cast<double>(_check.x2 - _image_x) / _scale_setting),
                static_cast<int>(static_cast<double>(_check.y2 - _image_y) / _scale_setting)
        );
        saveToXml(check, _image_file, _image_height, _image_width, 1);
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

    if (_check.x1 != _check.x2) {
        _temp_rect = _check;
        if (_check.x1 > _check.x2) {
            _temp_rect.x1 = _check.x2;
            _temp_rect.x2 = _check.x1;
        }
        if (_check.y1 > _check.y2) {
            _temp_rect.y1 = _check.y2;
            _temp_rect.y2 = _check.y1;
        }
        dc.SetPen(*wxCYAN_PEN);
        dc.DrawRectangle(_temp_rect.x1, _temp_rect.y1,
                         _temp_rect.x2 - _temp_rect.x1, _temp_rect.y2 - _temp_rect.y1);
        dc.DrawRectangle(_temp_rect.x1 - 1, _temp_rect.y1 - 1,
                         _temp_rect.x2 - _temp_rect.x1 + 2, _temp_rect.y2 - _temp_rect.y1 + 2);
    } else if (wxFileExists(_image_file.GetPath(true) + _image_file.GetName() + ".xml")) {
        dc.SetPen(*wxYELLOW_PEN);
        int x = static_cast<int>(static_cast<double>(_temp_rect.x1 + _image_x) * _scale_setting);
        int y = static_cast<int>(static_cast<double>(_temp_rect.y1 + _image_y) * _scale_setting);
        int width = static_cast<int>(static_cast<double>(_temp_rect.x2 - _temp_rect.x1) * _scale_setting);
        int height = static_cast<int>(static_cast<double>(_temp_rect.y2 + _temp_rect.y1) * _scale_setting);

        dc.DrawRectangle(x, y, width, height);
        dc.DrawRectangle(x - 1, y - 1, width + 2, height + 2);
        std::cout << "x:" << x << " y:" << y << " width:" << width << " height:" << height << std::endl;
    }
}

void ImagePanel::onMouse(wxMouseEvent & event)
{
    if (mode == STATUS::IDLE) {
        _mouse_x = event.GetX();
        _mouse_y = event.GetY();
    }

    if (event.LeftDown() && mode == STATUS::IDLE) {
        _check.x1 = event.GetX();
        _check.x2 = event.GetX();
        _check.y1 = event.GetY();
        _check.y2 = event.GetY();
        mode = STATUS::MOUSE_DOWN;
    } else if (event.LeftIsDown() && mode == STATUS::MOUSE_DOWN) {
        _check.x2 = event.GetX();
        _check.y2 = event.GetY();
    } else if (event.LeftUp() && mode == STATUS::MOUSE_DOWN) {
        mode = STATUS::IDLE;
        _mouse_x = event.GetX();
        _mouse_y = event.GetY();
    }
    locationAdjust(_check.x1, _check.y1);
    locationAdjust(_check.x2, _check.y2);
    locationAdjust(_mouse_x, _mouse_y);
    Refresh();
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