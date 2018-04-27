#include <wx/wx.h>
#include "../../data/XmlConfig.hpp"
#include <wx/filename.h>
#include <vector>

#ifndef IMAGESEARCHER_UI_IMAGEPANEL_HPP
#define IMAGESEARCHER_UI_IMAGEPANEL_HPP

class ImagePanel : public wxPanel {
public:
    wxDECLARE_EVENT_TABLE();

public:
    enum class STATUS {
        IDLE,
        MOUSE_DOWN
    };
    enum class ObjectKind {
        DETECTION,
        SEGMENTATION
    };

private:
    wxImage _original_image;
    wxBitmap _background_bitmap;
    wxBitmap _screen;
    wxFileName _image_file;
    int _image_width;
    int _image_height;
    int _bitmap_width;
    int _bitmap_height;

private:
    wxString _image_type = "car";
    int _image_depth = 3;
    int _image_diff = 0;
    double _zoom_setting = 1;

private:
    wxSize _min_init_size = wxSize(-1, -1);
    wxSize _max_init_size = wxSize(-1, -1);

private:
    bool _is_ready = false;
    int _selected_obj = -1;
    int _undo_obj = -1;
    bool _size_change = false;
    bool _show_name = false;
    bool _show_objects = true;

private:
    wxRect _current_view;
    wxRect _screen_rect;
    int _space_x = 10;
    int _space_y = 10;

private:
    wxPoint _mouse;

private:
    std::vector<Object> _obj_vector;
    std::vector<Name> _name_list;
    Object _new_object;
    Object _unpacked_object;
    ObjectKind _new_obj_kind = ObjectKind::DETECTION;

public:
    ImagePanel(wxWindow * parent, wxWindowID id);
    virtual ~ImagePanel();

public:
    void reset();

public:
    void setSize(int scale);
    void setType(wxString type) {_image_type = type;}
    void setDepth(int depth) {_image_depth = depth;}
    void setDiff(int diff) {_image_diff = diff;}
    int setBackgroundImage(wxString const & file, int scale = -1);
    void setNameList(std::vector<Name> & name_list) {_name_list = name_list;}
    void setInitSize(wxSize max, wxSize min) {_min_init_size = min; _max_init_size = max;}

public:
    void setMousePoint(int x, int y);
    void setView(int x, int y);
    bool save();
    void load();

public:
    void moveView(int x, int y);

public:
    int getImageWidth() const {return _image_width;}
    int getImageHeight() const {return _image_height;}
    wxRect getView() const { return _current_view; }

public:
    void addObject(Object const & obj);
    bool deleteObject();
    bool unpackObject();
    void previousObject();
    void nextObject();

public:
    bool startAddObject(ObjectKind kind, Object new_obj);
    void addPointToNewObject();
    void endAddObject();
    void cancelAddObject();
    bool undo();
    void pointUp();
    void pointDown();
    void pointLeft();
    void pointRight();
    void showObjectName();
    void showObjects();

public:
    bool isReady() {return _is_ready;};

private:
    void drawBackGround(wxDC & dc);
    void drawObject(wxDC & dc);
    void drawTempObject(wxDC & dc);

private:
    void onPaint(wxPaintEvent & event);
    void onMouse(wxMouseEvent & event);
    void onSize(wxSizeEvent & event);

private:
    wxPoint virtualLocationAdjust(int x, int y);
    wxPoint actualLocationAdjust(int x, int y);

public:
    wxPoint convertToActualLocation(int x, int y, bool bind_point = true);
    wxPoint convertToVirtualLocation(int x, int y, bool bind_point = true);
    wxPoint convertToVirtualLocation(wxPoint const & actual_point, bool bind_point = true);
};


#endif //IMAGESEARCHER_UI_IMAGEPANEL_HPP
