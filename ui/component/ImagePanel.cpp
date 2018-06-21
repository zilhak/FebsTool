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
                if (_name_list[j].name == _obj_vector[i].name) {
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
            dc.DrawText(_obj_vector[i].name, convertToVirtualLocation(middle_point, false));
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
            dc.DrawText(_new_object.name, convertToVirtualLocation(_new_object.point_list.back(), false));
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
        remove((_image_file.GetPath() + "/" + _image_file.GetName() + ".xml").c_str());
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
        wxMessageBox(wxT("it is not polygon."));
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
//#include <ui/component/ImagePanel.hpp>
//
//wxBEGIN_EVENT_TABLE(ImagePanel, wxPanel)
//    EVT_PAINT(ImagePanel::onPaint)
//    EVT_MOUSE_EVENTS(ImagePanel::onMouse)
//wxEND_EVENT_TABLE()
//
//ImagePanel::ImagePanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id)
//{
//    SetBackgroundColour(wxColour(0xFFFFFF));
//}
//
//ImagePanel::~ImagePanel()
//{
//    /* EMPTY */
//}
//
//void ImagePanel::setBackgroundImage(wxString filepath, wxString filename)
//{
//    _click = false;
//    _check = BoundingBox();
//    _image_file.Assign(wxString::Format("%s/%s",filepath, filename));
//    load();
//
//    wxImage image(_image_file.GetFullPath(), wxBITMAP_TYPE_JPEG);
//    _image_height = image.GetHeight();
//    _image_width = image.GetWidth();
//    image.Rescale(static_cast<int>(static_cast<double>(image.GetWidth()) * _scale_setting),
//                  static_cast<int>(static_cast<double>(image.GetHeight()) * _scale_setting),
//                  wxIMAGE_QUALITY_HIGH);
//    _background_bitmap = wxBitmap(image);
//    _bitmap_width = _background_bitmap.GetWidth();
//    _bitmap_height = _background_bitmap.GetHeight();
//    _is_ready = true;
//    Refresh();
//}
//
//void ImagePanel::load()
//{
//    _rect_vector = loadFromXml(_image_file.GetPath(true) + _image_file.GetName());
//    if (_rect_vector.size() < 1) {
//        _current_object_index = 0;
//        _xml_exists = false;
//    } else {
//        _current_object_index = static_cast<int>(_rect_vector.size());
//        _xml_exists = true;
//    }
//}
//
//bool ImagePanel::save()
//{
//    if (_background_bitmap.IsOk() && mode == STATUS::IDLE) {
//        int x1;
//        int y1;
//        int x2;
//        int y2;
//
//        if (!_click) {
//            return false;
//        }
//
//        if (_check.x1 > _check.x2) {
//            x1 = static_cast<int>(static_cast<double>(_check.x2 - _image_x) / _scale_setting);
//            x2 = static_cast<int>(static_cast<double>(_check.x1 - _image_x) / _scale_setting);
//        } else {
//            x1 = static_cast<int>(static_cast<double>(_check.x1 - _image_x) / _scale_setting);
//            x2 = static_cast<int>(static_cast<double>(_check.x2 - _image_x) / _scale_setting);
//        }
//
//        if (_check.y1 > _check.y2) {
//            y1 = static_cast<int>(static_cast<double>(_check.y2 - _image_y) / _scale_setting);
//            y2 = static_cast<int>(static_cast<double>(_check.y1 - _image_y) / _scale_setting);
//        } else {
//            y1 = static_cast<int>(static_cast<double>(_check.y1 - _image_y) / _scale_setting);
//            y2 = static_cast<int>(static_cast<double>(_check.y2 - _image_y) / _scale_setting);
//        }
//
//        if (_current_object_index != _rect_vector.size()) {
//            _rect_vector.erase(_rect_vector.begin() + _current_object_index);
//        }
//        BoundingBox check (_image_type, x1, y1, x2, y2, _image_diff);
//        _rect_vector.push_back(check);
//        _current_object_index = static_cast<int>(_rect_vector.size());
//
//        saveToXml(_rect_vector, _image_file, ImageInfo(_image_height, _image_width, _image_depth));
//        _click = false;
//        Refresh();
//
//        return true;
//    } else {
//        return false;
//    }
//}
//
void ImagePanel::saveCropImage()
{
//    wxRect sub_rect;
//    if (_background_bitmap.IsOk() && mode == STATUS::IDLE) {
//        int x1;
//        int y1;
//        int x2;
//        int y2;
//
//        if (_check.x1 > _check.x2) {
//            x1 = static_cast<int>(static_cast<double>(_check.x2 - _image_x) / _scale_setting);
//            x2 = static_cast<int>(static_cast<double>(_check.x1 - _image_x) / _scale_setting);
//        } else {
//            x1 = static_cast<int>(static_cast<double>(_check.x1 - _image_x) / _scale_setting);
//            x2 = static_cast<int>(static_cast<double>(_check.x2 - _image_x) / _scale_setting);
//        }
//
//        if (_check.y1 > _check.y2) {
//            y1 = static_cast<int>(static_cast<double>(_check.y2 - _image_y) / _scale_setting);
//            y2 = static_cast<int>(static_cast<double>(_check.y1 - _image_y) / _scale_setting);
//        } else {
//            y1 = static_cast<int>(static_cast<double>(_check.y1 - _image_y) / _scale_setting);
//            y2 = static_cast<int>(static_cast<double>(_check.y2 - _image_y) / _scale_setting);
//        }
//
//        sub_rect = wxRect(x1, y1, x2 - x1, y2 - y1);
//    }
//
//    wxImage image = _background_bitmap.ConvertToImage();
//
//    wxString crop_path = _image_file.GetPath(wxPATH_GET_SEPARATOR) + "CropFiles/";
//
//    if (!wxDirExists(crop_path)) {
//        wxMkdir(crop_path);
//    }
//
//    int i = 1;
//    while (wxFileExists(crop_path + "crop_" + _image_file.GetName() + wxString::Format("_%03d.", i) + _image_file.GetExt())) {
//        ++i;
//    }
//    image.GetSubImage(sub_rect).SaveFile(crop_path + "crop_" + _image_file.GetName()+ wxString::Format("_%03d.", i) + _image_file.GetExt());
}
//
//void ImagePanel::onPaint(wxPaintEvent & event)
//{
//    if (!_background_bitmap.IsOk()) {
//        return;
//    }
//
//    wxPaintDC dc(this);
//    dc.DrawBitmap(_background_bitmap , _image_x, _image_y);
//
//    dc.SetBrush(*wxTRANSPARENT_BRUSH);
//    int i = 0;
//    for (BoundingBox const & rect: _rect_vector) {
//        if (i == _current_object_index) {
//            if (_click) {
//                i++;
//                continue;
//            }
//            dc.SetPen(*wxCYAN_PEN);
//            dc.SetTextForeground(*wxCYAN);
//        } else {
//            dc.SetPen(*wxYELLOW_PEN);
//            dc.SetTextForeground(*wxYELLOW);
//        }
//
//        int x = _image_x + static_cast<int>(static_cast<double>(rect.x1) * _scale_setting);
//        int y = _image_y + static_cast<int>(static_cast<double>(rect.y1) * _scale_setting);
//        int width = static_cast<int>(static_cast<double>(rect.x2 - rect.x1) * _scale_setting);
//        int height = static_cast<int>(static_cast<double>(rect.y2 - rect.y1) * _scale_setting);
//
//        dc.DrawText(wxString(rect.name + "(" + std::to_string(rect.difficult) + ")"), x + 1, y + 1);
//        dc.DrawRectangle(x, y, width, height);
//        dc.DrawRectangle(x - 1, y - 1, width + 2, height + 2);
//        i++;
//    }
//
//    dc.SetPen(*wxRED_PEN);
//    if (mode == STATUS::IDLE && _image_y < _mouse_y && _mouse_y < _image_y + _bitmap_height) {
//        dc.DrawLine(_image_x,
//                    _mouse_y,
//                    _image_x + _bitmap_width,
//                    _mouse_y);
//    }
//    if (mode == STATUS::IDLE && _image_x < _mouse_x && _mouse_x < _image_x + _bitmap_width) {
//        dc.DrawLine(_mouse_x,
//                    _image_y,
//                    _mouse_x,
//                    _image_y + _bitmap_height);
//    }
//
//    if (_click) {
//        _temp_rect = _check;
//        if (_check.x1 > _check.x2) {
//            _temp_rect.x1 = _check.x2;
//            _temp_rect.x2 = _check.x1;
//        }
//        if (_check.y1 > _check.y2) {
//            _temp_rect.y1 = _check.y2;
//            _temp_rect.y2 = _check.y1;
//        }
//        dc.SetPen(*wxCYAN_PEN);
//        dc.DrawRectangle(_temp_rect.x1, _temp_rect.y1,
//                         _temp_rect.x2 - _temp_rect.x1, _temp_rect.y2 - _temp_rect.y1);
//        dc.DrawRectangle(_temp_rect.x1 - 1, _temp_rect.y1 - 1,
//                         _temp_rect.x2 - _temp_rect.x1 + 2, _temp_rect.y2 - _temp_rect.y1 + 2);
//    }
//
//}
//
//void ImagePanel::onMouse(wxMouseEvent & event)
//{
//    event.ResumePropagation(wxEVENT_PROPAGATE_MAX);
//    if (mode == STATUS::IDLE) {
//        _mouse_x = event.GetX();
//        _mouse_y = event.GetY();
//    }
//
//    if (event.LeftDown() && mode == STATUS::IDLE) {
//        _check.x1 = event.GetX();
//        _check.x2 = event.GetX();
//        _check.y1 = event.GetY();
//        _check.y2 = event.GetY();
//        _click = true;
//        mode = STATUS::MOUSE_DOWN;
//    } else if (event.LeftIsDown() && mode == STATUS::MOUSE_DOWN) {
//        _check.x2 = event.GetX();
//        _check.y2 = event.GetY();
//    } else if (event.LeftUp() && mode == STATUS::MOUSE_DOWN) {
//        mode = STATUS::IDLE;
//        _mouse_x = event.GetX();
//        _mouse_y = event.GetY();
//    }
//    locationAdjust(_check.x1, _check.y1);
//    locationAdjust(_check.x2, _check.y2);
//    locationAdjust(_mouse_x, _mouse_y);
//    event.Skip();
//    Refresh();
//}
//
//void ImagePanel::locationAdjust(int & x, int & y)
//{
//    if (x < _image_x) {
//        x = _image_x;
//    } else if (x > _image_x + _bitmap_width) {
//        x = _image_x + _bitmap_width;
//    }
//    if (y < _image_y) {
//        y = _image_y;
//    } else if (y > _image_y + _bitmap_height) {
//        y = _image_y + _bitmap_height;
//    }
//}
//
//void ImagePanel::deleteObject()
//{
//    if(_rect_vector.size() > _current_object_index) {
//        _rect_vector.erase(_rect_vector.begin() + _current_object_index);
//        _current_object_index = static_cast<int>(_rect_vector.size());
//        _click = false;
//        saveToXml(_rect_vector, _image_file, ImageInfo(_image_height, _image_width, _image_depth));
//    } else if (_rect_vector.size() > 0) {
//        _rect_vector.pop_back();
//        _current_object_index = static_cast<int>(_rect_vector.size());
//        saveToXml(_rect_vector, _image_file, ImageInfo(_image_height, _image_width, _image_depth));
//    }
//    Refresh();
//}
//
