#include <ui/component/ImagePanel.hpp>
#include "../../data/ThemeData.hpp"
#include <wx/dcbuffer.h>
#include <mutex>

wxBEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_PAINT(ImagePanel::onPaint)
    EVT_MOUSE_EVENTS(ImagePanel::onMouse)
    EVT_SIZE(ImagePanel::onSize)
wxEND_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id)
{
    SetBackgroundColour(wxColour(0xFFFFFF));
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

ImagePanel::~ImagePanel()
{
    /* EMPTY */
}

void ImagePanel::reset()
{
    _current_view.x = 0;
    _current_view.y = 0;
}

void ImagePanel::setSize(int scale)
{
    _zoom_setting = static_cast<double>(scale) / 100;

    _background_bitmap = wxBitmap(_original_image.Scale(static_cast<int>(static_cast<double>(_image_width) * _zoom_setting),
        static_cast<int>(static_cast<double>(_image_height) * _zoom_setting),
        wxIMAGE_QUALITY_HIGH));

    _bitmap_width = _background_bitmap.GetWidth();
    _bitmap_height = _background_bitmap.GetHeight();

    setView(_current_view.x, _current_view.y);
}

int ImagePanel::setBackgroundImage(wxString const & file, int scale)
{
    reset();

    _image_file.Assign(wxString::Format("%s", file));

    load();

    _original_image = wxImage(_image_file.GetFullPath(), wxBITMAP_TYPE_JPEG);

    _image_height = _original_image.GetHeight();
    _image_width = _original_image.GetWidth();

    if (scale != -1) {
        _zoom_setting = static_cast<double>(scale) / 100;

        _background_bitmap = wxBitmap(_original_image.Scale(static_cast<int>(static_cast<double>(_image_width) * _zoom_setting),
            static_cast<int>(static_cast<double>(_image_height) * _zoom_setting),
            wxIMAGE_QUALITY_HIGH));

        _bitmap_width = _background_bitmap.GetWidth();
        _bitmap_height = _background_bitmap.GetHeight();

        setView(_current_view.x, _current_view.y);

        
    }

    _is_ready = true;

    return scale;
}

void ImagePanel::setMousePoint(int x, int y)
{
    _mouse = virtualLocationAdjust(x, y);
}

void ImagePanel::setView(int x, int y)
{
    int screen_width = this->GetClientSize().GetWidth() - (2 * _space_x);
    int screen_height = this->GetClientSize().GetHeight() - (2 * _space_y);

    int adjust_width = screen_width;
    int adjust_height = screen_height;

    if (_bitmap_width < screen_width) {
        adjust_width = _bitmap_width;
        x = 0;
    }
    else if (_bitmap_width < x + screen_width) {
        x = _bitmap_width - adjust_width;
    }

    if (_bitmap_height < screen_height) {
        adjust_height = _bitmap_height;
        y = 0;
    }
    else if (_bitmap_height < y + screen_height) {
        y = _bitmap_height - adjust_height;
    }

    _current_view = wxRect(x, y, adjust_width, adjust_height);
    _screen = _background_bitmap.GetSubBitmap(_current_view);

    Refresh();
}

void ImagePanel::moveView(int x, int y)
{
    int move_x;
    int move_y;

    if (_current_view.x + x + _current_view.width > _bitmap_width) {
        move_x = _bitmap_width - _current_view.width;
    }
    else if (_current_view.x + x < 0) {
        move_x = 0;
    }
    else {
        move_x = _current_view.x + x;
    }

    if (_current_view.y + y + _current_view.height > _bitmap_height) {
        move_y = _bitmap_height - _current_view.height;
    }
    else if (_current_view.y + y < 0) {
        move_y = 0;
    }
    else {
        move_y = _current_view.y + y;
    }

    setView(move_x, move_y);
}

void ImagePanel::load()
{
    _obj_vector = loadFromXml(_image_file.GetPath() + "\\" + _image_file.GetName());
    if (_obj_vector.size() < 1) {
        _selected_obj = 0;
    } else {
        _selected_obj = static_cast<int>(_obj_vector.size());
    }
}

bool ImagePanel::save()
{
    if (_background_bitmap.IsOk()) {
        saveToXml(_obj_vector, _image_file, ImageInfo(_image_height, _image_width, _image_depth));
        Refresh();
        return true;
    } else {
        return false;
    }
}

void ImagePanel::onPaint(wxPaintEvent & event)
{
    if (!_screen.IsOk()) {
        return;
    }
    wxPaintDC dc(this);
    wxBufferedDC bg(&dc);
    bg.Clear();
    if (_size_change) {
        dc.Clear();
        _size_change = false;
    }
    drawBackGround(bg);
    drawObject(bg);
    drawTempObject(bg);
}

void ImagePanel::onMouse(wxMouseEvent & event)
{
    event.ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event.Skip();
}

void ImagePanel::onSize(wxSizeEvent & event)
{
    if (!_background_bitmap.IsOk()) {
        return;
    }
    setView(_current_view.x, _current_view.y);
    Refresh();
}

void ImagePanel::drawBackGround(wxDC & dc)
{
    dc.DrawBitmap(_screen, _space_x, _space_y);
}

void ImagePanel::drawObject(wxDC & dc)
{
    if (!_show_objects) {
        return;
    }

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    for (int i = 0; i < _obj_vector.size(); ++i) {
        if (i == _selected_obj) {
            dc.SetPen(wxPen(COLOUR_IMAGE_PANEL_BOX_SELECTED, 2));
            dc.SetTextForeground(COLOUR_IMAGE_PANEL_BOX_SELECTED);
        } else {
            wxColour colour = wxColour(0x00FFFF);
            for (int j = 0; j < _name_list.size(); ++j) {
                if (_name_list[j].name == _obj_vector[i].type) {
                    colour = wxColour(wxAtol(_name_list[j].colour));
                    break;
                }
            }
            dc.SetPen(wxPen(colour, 2));
            dc.SetTextForeground(colour);
        }

        if (_obj_vector[i].point_list.size() > 2) {
            int list_size = _obj_vector[i].point_list.size();
            for (int j = 0; j < list_size; ++j) {
                dc.DrawLine(convertToVirtualLocation(_obj_vector[i].point_list[j], false),
                            convertToVirtualLocation(_obj_vector[i].point_list[(j + 1) % list_size], false));
            }
        } else {
            // TODO: add rect algorithm
        }
        if (_show_name) {
            dc.SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            wxPoint middle_point = (_obj_vector[i].point_list[0] + _obj_vector[i].point_list[_obj_vector[i].point_list.size() / 2]) / 2;
            dc.DrawText(_obj_vector[i].type, convertToVirtualLocation(middle_point, false));
        }
    }
}

void ImagePanel::drawTempObject(wxDC & dc)
{
    dc.SetPen(wxPen(COLOUR_IMAGE_PANEL_BOX_SELECTED, 2));
    if (_new_obj_kind == ObjectKind::DETECTION) {

    } else if (_new_obj_kind == ObjectKind::SEGMENTATION) {
        int list_size = _new_object.point_list.size();
        if (list_size < 1) {
            return;
        } else if (list_size == 1) {
            dc.DrawLine(convertToVirtualLocation(_new_object.point_list[0], false),
                        _mouse);
        } else {
            for (int i = 0; i < list_size - 1; ++i) {
                dc.DrawLine(convertToVirtualLocation(_new_object.point_list[i], false),
                            convertToVirtualLocation(_new_object.point_list[i + 1], false));
            }
            dc.DrawLine(convertToVirtualLocation(_new_object.point_list.back(), false),
                        _mouse);
            dc.DrawLine(_mouse,
                        convertToVirtualLocation(_new_object.point_list.front(), false));
        }
        if (_show_name) {
            dc.SetTextForeground(COLOUR_IMAGE_PANEL_BOX_SELECTED);
            dc.SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            dc.DrawText(_new_object.type, convertToVirtualLocation(_new_object.point_list.back(), false));
        }
    }
}

void ImagePanel::addObject(Object const & obj)
{
    _obj_vector.push_back(obj);
    Refresh();
}

bool ImagePanel::deleteObject()
{
    if(_obj_vector.size() > _selected_obj) {
        _obj_vector.erase(_obj_vector.begin() + _selected_obj);
        _selected_obj = static_cast<int>(_obj_vector.size());
        saveToXml(_obj_vector, _image_file, ImageInfo(_image_height, _image_width, _image_depth));
    } 
    Refresh();

    if (_obj_vector.empty()) {
        remove(_image_file.GetPath() + "\\" + _image_file.GetName() + ".xml");
        return true;
    } else {
        return false;
    }
}

bool ImagePanel::unpackObject()
{
    if (_obj_vector.size() > _selected_obj) {
        _new_object = _obj_vector[_selected_obj];
        _unpacked_object = _new_object;
        if (_new_object.point_list.size() > 2) {
            _new_obj_kind = ObjectKind::SEGMENTATION;
        } else {
            _new_obj_kind = ObjectKind::DETECTION;
        }
        _obj_vector.erase(_obj_vector.begin() + _selected_obj);
        _selected_obj = static_cast<int>(_obj_vector.size());
        
        Refresh();
        return true;
    }

    return false;
}

void ImagePanel::previousObject()
{
    if (_selected_obj > 0) {
        _selected_obj--;
    } else {
        _selected_obj = static_cast<int>(_obj_vector.size());
    }
    Refresh();
}

void ImagePanel::nextObject()
{
    if (_selected_obj < _obj_vector.size()) {
        _selected_obj++;
    } else {
        _selected_obj = 0;
    }
    Refresh();
}

bool ImagePanel::startAddObject(ObjectKind kind, Object new_obj)
{
    if (_current_view.Contains(_mouse) == false) {
        return false;
    }
    _selected_obj = _obj_vector.size();
    _new_object = new_obj;
    _new_obj_kind = kind;
    _undo_obj = _selected_obj;

    _new_object.point_list.push_back(convertToActualLocation(_mouse.x, _mouse.y));

    return true;
}

void ImagePanel::addPointToNewObject()
{
    _new_object.point_list.push_back(convertToActualLocation(_mouse.x, _mouse.y));
}

void ImagePanel::endAddObject()
{
    if (_new_object.point_list.size() < 3) {
        wxMessageBox(wxT("���� ���� : ������ �ƴմϴ�."));
        cancelAddObject();
        return;
    }

    if (_selected_obj < _obj_vector.size()) {
        _obj_vector[_selected_obj] = _new_object;
    } else {
        addObject(_new_object);
    }

    _new_object.point_list.clear();
    _selected_obj = _obj_vector.size();

    save();
    Refresh();
}

void ImagePanel::cancelAddObject()
{
    if (_unpacked_object.point_list.size() > 0) {
        _obj_vector.push_back(_unpacked_object);
        _unpacked_object.point_list.clear();
    }
    _new_object.point_list.clear();
}

bool ImagePanel::undo()
{    
    if (_selected_obj == -1) {
        return true;
    } else if (_obj_vector.size() == _selected_obj && _new_object.point_list.size() > 0) {
        _new_object.point_list.pop_back();
    } else if (_selected_obj < _obj_vector.size()) {
        unpackObject();
    }

    if (_new_object.point_list.size() == 0) {
        return false;
    }
}

void ImagePanel::pointUp()
{
    if (_new_object.point_list.empty()) {
        return;
    }
    if (_new_object.point_list.back().y > 0) {
        _new_object.point_list.back().y--;
    }
}

void ImagePanel::pointDown()
{
    if (_new_object.point_list.empty()) {
        return;
    }
    if (_new_object.point_list.back().y < _image_height) {
        _new_object.point_list.back().y++;
    }
}

void ImagePanel::pointLeft()
{
    if (_new_object.point_list.empty()) {
        return;
    }
    if (_new_object.point_list.back().x > 0) {
        _new_object.point_list.back().x--;
    }
}

void ImagePanel::pointRight()
{
    if (_new_object.point_list.empty()) {
        return;
    }
    if (_new_object.point_list.back().x < _image_width) {
        _new_object.point_list.back().x++;
    }
}

void ImagePanel::showObjectName()
{
    _show_name = !_show_name;
    Refresh();
}

void ImagePanel::showObjects()
{
    _show_objects = !_show_objects;
    Refresh();
}

wxPoint ImagePanel::virtualLocationAdjust(int x, int y)
{
    if (x < _space_x) {
        x = _space_x;
    } else if (x > _space_x + _current_view.width) {
        x = _space_x + _current_view.width;
    }

    if (y < _space_y) {
        y = _space_y;
    } else if (y > _space_y + _current_view.height) {
        y = _space_y + _current_view.height;
    }

    return wxPoint(x, y);
}

wxPoint ImagePanel::actualLocationAdjust(int x, int y)
{
    if (x < 0) {
        x = 0;
    }
    else if (x > _image_width) {
        x = _image_width;
    }

    if (y < 0) {
        y = 0;
    }
    else if (y > _image_height) {
        y = _image_height;
    }
    return wxPoint(x, y);
}

wxPoint ImagePanel::convertToActualLocation(int x, int y, bool bind_point)
{
    wxPoint result;

    result.x = static_cast<double>(_current_view.x + x - _space_x) / _zoom_setting;
    result.y = static_cast<double>(_current_view.y + y - _space_y) / _zoom_setting;

    if (bind_point) {
        return actualLocationAdjust(result.x, result.y);
    }

    return result;
}

wxPoint ImagePanel::convertToVirtualLocation(int x, int y, bool bind_point)
{
    wxPoint result;

    result.x = _space_x - _current_view.x + (x * _zoom_setting);
    result.y = _space_y - _current_view.y + (y * _zoom_setting);

    if (bind_point) {
        return virtualLocationAdjust(result.x, result.y);
    }

    return result;
}

wxPoint ImagePanel::convertToVirtualLocation(wxPoint const & actual_point, bool bind_point)
{
    return convertToVirtualLocation(actual_point.x, actual_point.y, bind_point);
}
