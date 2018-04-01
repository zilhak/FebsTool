#include <ui/component/ImagePanel.hpp>

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
    _click = false;
    _check = BoundingBox();
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
    _rect_vector = loadFromXml(_image_file.GetPath(true) + _image_file.GetName());
    if (_rect_vector.size() < 1) {
        _current_object_index = 0;
        _xml_exists = false;
    } else {
        _current_object_index = static_cast<int>(_rect_vector.size());
        _xml_exists = true;
    }
}

bool ImagePanel::save()
{
    if (_background_bitmap.IsOk() && mode == STATUS::IDLE) {
        int x1;
        int y1;
        int x2;
        int y2;

        if (!_click) {
            return false;
        }

        if (_check.x1 > _check.x2) {
            x1 = static_cast<int>(static_cast<double>(_check.x2 - _image_x) / _scale_setting);
            x2 = static_cast<int>(static_cast<double>(_check.x1 - _image_x) / _scale_setting);
        } else {
            x1 = static_cast<int>(static_cast<double>(_check.x1 - _image_x) / _scale_setting);
            x2 = static_cast<int>(static_cast<double>(_check.x2 - _image_x) / _scale_setting);
        }

        if (_check.y1 > _check.y2) {
            y1 = static_cast<int>(static_cast<double>(_check.y2 - _image_y) / _scale_setting);
            y2 = static_cast<int>(static_cast<double>(_check.y1 - _image_y) / _scale_setting);
        } else {
            y1 = static_cast<int>(static_cast<double>(_check.y1 - _image_y) / _scale_setting);
            y2 = static_cast<int>(static_cast<double>(_check.y2 - _image_y) / _scale_setting);
        }

        if (_current_object_index != _rect_vector.size()) {
            _rect_vector.erase(_rect_vector.begin() + _current_object_index);
        }
        BoundingBox check (_image_type, x1, y1, x2, y2, _image_diff);
        _rect_vector.push_back(check);
        _current_object_index = static_cast<int>(_rect_vector.size());

        saveToXml(_rect_vector, _image_file, ImageInfo(_image_height, _image_width, _image_depth));
        _click = false;
        Refresh();

        return true;
    } else {
        return false;
    }
}

void ImagePanel::saveCropImage()
{

    wxRect sub_rect;
    if (_background_bitmap.IsOk() && mode == STATUS::IDLE) {
        int x1;
        int y1;
        int x2;
        int y2;

        if (_check.x1 > _check.x2) {
            x1 = static_cast<int>(static_cast<double>(_check.x2 - _image_x) / _scale_setting);
            x2 = static_cast<int>(static_cast<double>(_check.x1 - _image_x) / _scale_setting);
        } else {
            x1 = static_cast<int>(static_cast<double>(_check.x1 - _image_x) / _scale_setting);
            x2 = static_cast<int>(static_cast<double>(_check.x2 - _image_x) / _scale_setting);
        }

        if (_check.y1 > _check.y2) {
            y1 = static_cast<int>(static_cast<double>(_check.y2 - _image_y) / _scale_setting);
            y2 = static_cast<int>(static_cast<double>(_check.y1 - _image_y) / _scale_setting);
        } else {
            y1 = static_cast<int>(static_cast<double>(_check.y1 - _image_y) / _scale_setting);
            y2 = static_cast<int>(static_cast<double>(_check.y2 - _image_y) / _scale_setting);
        }

        sub_rect = wxRect(x1, y1, x2 - x1, y2 - y1);
    }

    wxImage image = _background_bitmap.ConvertToImage();
    wxImage sub_image = image.GetSubImage(sub_rect);

    sub_image.SaveFile(_image_file.GetPath(true) + "crop_" + _image_file.GetFullName());
}

void ImagePanel::onPaint(wxPaintEvent & event)
{
    if (!_background_bitmap.IsOk()) {
        return;
    }

    wxPaintDC dc(this);
    dc.DrawBitmap(_background_bitmap , _image_x, _image_y);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    int i = 0;
    for (BoundingBox const & rect: _rect_vector) {
        if (i == _current_object_index) {
            if (_click) {
                i++;
                continue;
            }
            dc.SetPen(*wxCYAN_PEN);
            dc.SetTextForeground(*wxCYAN);
        } else {
            dc.SetPen(*wxYELLOW_PEN);
            dc.SetTextForeground(*wxYELLOW);
        }

        int x = _image_x + static_cast<int>(static_cast<double>(rect.x1) * _scale_setting);
        int y = _image_y + static_cast<int>(static_cast<double>(rect.y1) * _scale_setting);
        int width = static_cast<int>(static_cast<double>(rect.x2 - rect.x1) * _scale_setting);
        int height = static_cast<int>(static_cast<double>(rect.y2 - rect.y1) * _scale_setting);

        dc.DrawText(wxString(rect.type + "(" + std::to_string(rect.difficult) + ")"), x + 1, y + 1);
        dc.DrawRectangle(x, y, width, height);
        dc.DrawRectangle(x - 1, y - 1, width + 2, height + 2);
        i++;
    }

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

    if (_click) {
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
    }

}

void ImagePanel::onMouse(wxMouseEvent & event)
{
    event.ResumePropagation(wxEVENT_PROPAGATE_MAX);
    if (mode == STATUS::IDLE) {
        _mouse_x = event.GetX();
        _mouse_y = event.GetY();
    }

    if (event.LeftDown() && mode == STATUS::IDLE) {
        _check.x1 = event.GetX();
        _check.x2 = event.GetX();
        _check.y1 = event.GetY();
        _check.y2 = event.GetY();
        _click = true;
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
    event.Skip();
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

void ImagePanel::deleteObject()
{
    if(_rect_vector.size() > _current_object_index) {
        _rect_vector.erase(_rect_vector.begin() + _current_object_index);
        _current_object_index = static_cast<int>(_rect_vector.size());
        _click = false;
        saveToXml(_rect_vector, _image_file, ImageInfo(_image_height, _image_width, _image_depth));
    } else if (_rect_vector.size() > 0) {
        _rect_vector.pop_back();
        _current_object_index = static_cast<int>(_rect_vector.size());
        saveToXml(_rect_vector, _image_file, ImageInfo(_image_height, _image_width, _image_depth));
    }
    Refresh();
}

void ImagePanel::previousObject()
{
    if (_current_object_index > 0) {
        _current_object_index--;
    } else {
        _current_object_index = static_cast<int>(_rect_vector.size());
    }
    _click = false;
    Refresh();
}

void ImagePanel::nextObject()
{
    if (_current_object_index < _rect_vector.size()) {
        _current_object_index++;
    } else {
        _current_object_index = 0;
    }
    _click = false;
    Refresh();
}