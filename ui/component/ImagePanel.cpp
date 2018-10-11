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

    _is_loaded = true;

    return scale;
}

void ImagePanel::setMousePoint(int x, int y)
{
    _actual_mouse_pos = virtualLocationAdjust(x, y);
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
    _obj_vector = loadFromXml(_image_file.GetPath() + "/" + _image_file.GetName());
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
        _selected_obj = _obj_vector.size();
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
    drawBackGround(bg);
    drawObject(bg);
    drawTempObject(bg);
}

void ImagePanel::drawBackGround(wxDC & dc)
{
    dc.DrawBitmap(_screen, _space_x, _space_y);
}

void ImagePanel::drawObject(wxDC & dc)
{
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    for (int i = 0; i < _obj_vector.size(); ++i) {
        if (i == _selected_obj) {
            dc.SetPen(wxPen(COLOUR_IMAGE_PANEL_BOX_SELECTED, 2));
        } else {
            wxColour colour = wxColour(0x00FFFF);
            for (int j = 0; j < _name_list.size(); ++j) {
                if (_name_list[j].name == _obj_vector[i].name) {
                    colour = wxColour(wxAtol(_name_list[j].colour));
                    break;
                }
            }
            dc.SetPen(wxPen(colour, 2));
            dc.SetTextForeground(colour);
        }

        if (_show_objects) {
            if (_obj_vector[i].type == ObjectType::SEGMENTATION) {
                int list_size = _obj_vector[i].point_list.size();
                for (int j = 0; j < list_size; ++j) {
                    dc.DrawLine(convertToVirtualLocation(_obj_vector[i].point_list[j], false),
                                convertToVirtualLocation(_obj_vector[i].point_list[(j + 1) % list_size], false));
                }
            } else if (_obj_vector[i].type == ObjectType::DETECTION){
                wxRect rect = wxRect(_obj_vector[i].point_list[0], _obj_vector[i].point_list[1]);
                dc.DrawRectangle(convertToVirtualRect(rect));
                dc.DrawRectangle(convertToVirtualRect(wxRect(rect.x - 1, rect.y -1, rect.width + 2, rect.height + 2)));
            }
        }

        if (_show_name) {
            dc.SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            wxPoint middle_point = (_obj_vector[i].point_list[0] + _obj_vector[i].point_list[_obj_vector[i].point_list.size() / 2]) / 2;
            dc.DrawText(_obj_vector[i].name, convertToVirtualLocation(middle_point, false));
        }
    }
}

void ImagePanel::drawTempObject(wxDC & dc)
{
    dc.SetPen(wxPen(COLOUR_IMAGE_PANEL_BOX_SELECTED, 2));
    if (_temp_obj.type == ObjectType::DETECTION) {
        if (_status == STATUS::DRAWING_NEW_OBJECT) {
            int x1 = _temp_obj.point_list[0].x;
            int x2 = _actual_mouse_pos.x;
            int y1 = _temp_obj.point_list[0].y;
            int y2 = _actual_mouse_pos.y;

            if (x1 > x2) {
                int temp = x1;
                x1 = x2;
                x2 = temp;
            }

            if (y1 > y2) {
                int temp = y1;
                y1 = y2;
                y2 = temp;
            }
            dc.DrawRectangle(convertToVirtualRect(wxRect(x1, y1, x2 - x1, y2 - y1)));
//            dc.DrawRectangle(convertToVirtualRect(wxRect(x1 - 1, y1 - 1, x2 - x1 + 2, y2 - y1 + 2)));
        } else if (_status == STATUS::EDIT_OBJECT) {
            int x1 = _temp_obj.point_list[0].x;
            int x2 = _temp_obj.point_list[1].x;
            int y1 = _temp_obj.point_list[0].y;
            int y2 = _temp_obj.point_list[1].y;

            if (x1 > x2) {
                int temp = x1;
                x1 = x2;
                x2 = temp;
            }

            if (y1 > y2) {
                int temp = y1;
                y1 = y2;
                y2 = temp;
            }
            dc.DrawRectangle(convertToVirtualRect(wxRect(x1, y1, x2 - x1, y2 - y1)));
            dc.DrawRectangle(convertToVirtualRect(wxRect(x1 - 1, y1 - 1, x2 - x1 + 2, y2 - y1 + 2)));
        }
    } else if (_temp_obj.type == ObjectType::SEGMENTATION) {
        int list_size = _temp_obj.point_list.size();
        if (list_size < 1) {
            return;
        } else if (list_size == 1) {
            dc.DrawLine(convertToVirtualLocation(_temp_obj.point_list[0], false),
                        _actual_mouse_pos);
        } else {
            for (int i = 0; i < list_size - 1; ++i) {
                dc.DrawLine(convertToVirtualLocation(_temp_obj.point_list[i], false),
                            convertToVirtualLocation(_temp_obj.point_list[i + 1], false));
            }
            dc.DrawLine(convertToVirtualLocation(_temp_obj.point_list.back(), false),
                        _actual_mouse_pos);
            dc.DrawLine(_actual_mouse_pos,
                        convertToVirtualLocation(_temp_obj.point_list.front(), false));
        }
        if (_show_name) {
            dc.SetTextForeground(COLOUR_IMAGE_PANEL_BOX_SELECTED);
            dc.SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            dc.DrawText(_temp_obj.name, convertToVirtualLocation(_temp_obj.point_list.back(), false));
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
    } else if (_obj_vector.back().type == ObjectType::DETECTION) {
        _obj_vector.pop_back();
        _selected_obj = static_cast<int>(_obj_vector.size());
        saveToXml(_obj_vector, _image_file, ImageInfo(_image_height, _image_width, _image_depth));
    }
    Refresh();

    if (_obj_vector.empty()) {
        remove((_image_file.GetPath() + "/" + _image_file.GetName() + ".xml").c_str());
        return true;
    } else {
        return false;
    }
}

bool ImagePanel::unpackObject()
{
    if (_obj_vector.size() > _selected_obj) {
        _temp_obj = _obj_vector[_selected_obj];
        _unpacked_object = _temp_obj;
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
    _status = STATUS::IDLE;
    Refresh();
}

void ImagePanel::nextObject()
{
    if (_selected_obj < _obj_vector.size()) {
        _selected_obj++;
    } else {
        _selected_obj = 0;
    }
    _status = STATUS::IDLE;
    Refresh();
}

bool ImagePanel::startAddSegment(Object const & new_obj)
{
    if (_current_view.Contains(_actual_mouse_pos) == false || new_obj.type != ObjectType::SEGMENTATION) {
        return false;
    }

    _temp_obj = new_obj;
    _undo_obj = _selected_obj;
    _selected_obj = static_cast<int>(_obj_vector.size());

    _temp_obj.point_list.push_back(convertToActualLocation(_actual_mouse_pos.x, _actual_mouse_pos.y));

    return true;
}

void ImagePanel::addPointToNewSegment()
{
    if (_temp_obj.type != ObjectType::SEGMENTATION && _temp_obj.point_list.size() != 1) {
        return;
    }

    _temp_obj.point_list.push_back(convertToActualLocation(_actual_mouse_pos.x, _actual_mouse_pos.y));
}

void ImagePanel::endAddSegment()
{
    if (_temp_obj.type == ObjectType::SEGMENTATION && _temp_obj.point_list.size() < 3) {
        wxMessageBox(wxT("it is not polygon."));
        cancelAddSegment();
        return;
    }

    if (_temp_obj.type == ObjectType::DETECTION && _temp_obj.point_list.size() != 2) {
        cancelAddSegment();
        return;
    }

    if (_selected_obj < _obj_vector.size()) {
        _obj_vector[_selected_obj] = _temp_obj;
    } else {
        addObject(_temp_obj);
    }

    _temp_obj.point_list.clear();
    _selected_obj = _obj_vector.size();

    save();
    Refresh();
}

void ImagePanel::cancelAddSegment()
{
    if (_unpacked_object.point_list.size() > 0) {
        _obj_vector.push_back(_unpacked_object);
        _unpacked_object.point_list.clear();
    }
    _temp_obj.point_list.clear();
}

bool ImagePanel::undoSegment()
{    
    if (_selected_obj == -1) {
        return true;
    } else if (_obj_vector.size() == _selected_obj && _temp_obj.point_list.size() > 0) {
        _temp_obj.point_list.pop_back();
    } else if (_selected_obj < _obj_vector.size()) {
        unpackObject();
    }

    if (_temp_obj.point_list.size() == 0) {
        return false;
    } else {
        return true;
    }
}

bool ImagePanel::startAddTempDetection(Object const & new_obj)
{
    if (_current_view.Contains(_actual_mouse_pos) == false || new_obj.type != ObjectType::DETECTION) {
        return false;
    }

    if (_selected_obj < _obj_vector.size() && _selected_obj != -1) {
        _unpacked_object = _obj_vector[_selected_obj];
        _obj_vector.erase(_obj_vector.begin() + _selected_obj);
    } else {
        _unpacked_object.point_list.clear();
    }

    _temp_obj = new_obj;
    _undo_obj = _selected_obj;
    _selected_obj = static_cast<int>(_obj_vector.size());

    _temp_obj.point_list.push_back(_actual_mouse_pos);

    _status = STATUS::DRAWING_NEW_OBJECT;
    return true;
}

void ImagePanel::endAddTempDetection()
{
    if (_temp_obj.type != ObjectType::DETECTION) {
        return;
    }

    if (_temp_obj.point_list.size() < 2) {
        _temp_obj.point_list.push_back(_actual_mouse_pos);
    }

    _status = STATUS::EDIT_OBJECT;
}

bool ImagePanel::saveTempDetection()
{
    if (_temp_obj.type == ObjectType::DETECTION && _status == STATUS::EDIT_OBJECT) {
        _obj_vector.emplace_back(_temp_obj);
        _temp_obj.point_list.clear();
    } else {
        return false;
    }

    save();

    _status = STATUS::IDLE;

    return true;
}

void ImagePanel::cancelAddDetection()
{

}

void ImagePanel::undoDetection()
{
    _status = STATUS::IDLE;

    if (_unpacked_object.point_list.size() == 2) {
        _obj_vector.push_back(_unpacked_object);
        _unpacked_object.point_list.clear();
    }

    Refresh();
}

void ImagePanel::pointUp()
{
    if (_temp_obj.point_list.empty()) {
        return;
    }

    if (_temp_obj.type == ObjectType::SEGMENTATION && _temp_obj.point_list.back().y > 0) {
        _temp_obj.point_list.back().y--;
    } else if (_temp_obj.type == ObjectType::DETECTION && _temp_obj.point_list.front().y > 0) {
        _temp_obj.point_list.front().y--;
    }
}

void ImagePanel::pointDown()
{
    if (_temp_obj.point_list.empty()) {
        return;
    }

    if (_temp_obj.type == ObjectType::SEGMENTATION && _temp_obj.point_list.back().y < _image_height) {
        _temp_obj.point_list.back().y++;
    } else if (_temp_obj.type == ObjectType::DETECTION && _temp_obj.point_list.front().y <_image_height) {
        _temp_obj.point_list.front().y++;
    }
}

void ImagePanel::pointLeft()
{
    if (_temp_obj.point_list.empty()) {
        return;
    }

    if (_temp_obj.type == ObjectType::SEGMENTATION && _temp_obj.point_list.back().x > 0) {
        _temp_obj.point_list.back().x--;
    } else if (_temp_obj.type == ObjectType::DETECTION && _temp_obj.point_list.front().x > 0) {
        _temp_obj.point_list.front().x--;
    }
}

void ImagePanel::pointRight()
{
    if (_temp_obj.point_list.empty()) {
        return;
    }

    if (_temp_obj.type == ObjectType::SEGMENTATION && _temp_obj.point_list.back().x < _image_width) {
        _temp_obj.point_list.back().x++;
    } else if (_temp_obj.type == ObjectType::DETECTION && _temp_obj.point_list.front().x < _image_width) {
        _temp_obj.point_list.front().x++;
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

void ImagePanel::saveCropImage()
{

}

void ImagePanel::setType(wxString const & type)
{
    if (_selected_obj < _obj_vector.size() && _selected_obj != -1) {
        _obj_vector.at(_selected_obj).name = type;
    }
    _temp_obj.name = type;

    Refresh();
}

void ImagePanel::setDiff(int diff)
{
    if (_selected_obj < _obj_vector.size() && _selected_obj != -1) {
        _obj_vector.at(_selected_obj).difficult = diff;
    }
    _temp_obj.difficult = diff;

    Refresh();
}

void ImagePanel::onMouse(wxMouseEvent & event)
{
    _virtual_mouse_pos = event.GetPosition();
    _actual_mouse_pos = convertToActualLocation(_virtual_mouse_pos.x, _virtual_mouse_pos.y, false);

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

wxRect ImagePanel::convertToVirtualRect(wxRect const & actual_rect, bool bind_point)
{
    wxPoint point = convertToVirtualLocation(actual_rect.x, actual_rect.y);
    int width = actual_rect.width * _zoom_setting;
    int height = actual_rect.height * _zoom_setting;

    return wxRect(point.x, point.y, width, height);
}

wxRect ImagePanel::convertToActualRect(wxRect const & virtual_rect, bool bind_point)
{
    wxPoint point = convertToVirtualLocation(virtual_rect.x, virtual_rect.y);
    int width = static_cast<int>(static_cast<double>(virtual_rect.width) / _zoom_setting);
    int height = static_cast<int>(static_cast<double>(virtual_rect.height) / _zoom_setting);

    return wxRect(point.x, point.y, width, height);
}
