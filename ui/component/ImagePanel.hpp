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
        DRAWING_NEW_OBJECT,
        EDIT_OBJECT
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
    int _image_depth = 3;
    double _zoom_setting = 1;

private: // init size setting
    wxSize _min_init_size = wxSize(-1, -1);
    wxSize _max_init_size = wxSize(-1, -1);

private: // setting status
    bool _is_loaded = false;
    int _selected_obj = -1;
    int _undo_obj = -1;
    int _space_x = 10;
    int _space_y = 10;

private: // program status value
    bool _show_name = false;
    bool _show_objects = true;

private: // status value
    wxRect _current_view;
    wxRect _screen_rect;
    wxPoint _virtual_mouse_pos;
    wxPoint _actual_mouse_pos;
    STATUS _status = STATUS::IDLE;

private: // Objects
    std::vector<Object> _obj_vector;
    std::vector<Name> _name_list;
    Object _temp_obj;
    Object _unpacked_object;

public:
    ImagePanel(wxWindow * parent, wxWindowID id);
    virtual ~ImagePanel();

public:
    void reset();

public:
    void setType(wxString const & type);
    void setDiff(int diff);
    void setSize(int scale);
    void setDepth(int depth) {_image_depth = depth;}
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
    wxPoint getActualMouse() const { return _actual_mouse_pos; }
    wxRect getView() const { return _current_view; }

public:
    void addObject(Object const & obj);
    bool deleteObject();
    bool unpackObject();
    void previousObject();
    void nextObject();

public: // Add segmentation object
    bool startAddSegment(Object const & new_obj);
    void addPointToNewSegment();
    void endAddSegment();
    void cancelAddSegment();
    bool undoSegment();

public: // Add detection object
    bool startAddTempDetection(Object const & new_obj);
    void endAddTempDetection();
    bool saveTempDetection();
    void cancelAddDetection();
    void undoDetection();

public: // on Add object event
    void pointUp();
    void pointDown();
    void pointLeft();
    void pointRight();
    void showObjectName();
    void showObjects();

public:
    bool isLoaded() {return _is_loaded;};

private: // draw.
    void drawBackGround(wxDC & dc);
    void drawObject(wxDC & dc);
    void drawTempObject(wxDC & dc);

private: // events.
    void onPaint(wxPaintEvent & event);
    void onMouse(wxMouseEvent & event);
    void onSize(wxSizeEvent & event);

private: // location adjust
    wxPoint virtualLocationAdjust(int x, int y);
    wxPoint actualLocationAdjust(int x, int y);

public: // point calculation
    wxPoint convertToActualLocation(int x, int y, bool bind_point = true);
    wxPoint convertToVirtualLocation(int x, int y, bool bind_point = true);
    wxPoint convertToVirtualLocation(wxPoint const & actual_point, bool bind_point = true);
    wxRect convertToVirtualRect(wxRect const & actual_rect, bool bind_point = true);
    wxRect convertToActualRect(wxRect const & virtual_rect, bool bind_point = true);

public: // action
    void saveCropImage();
};


#endif //IMAGESEARCHER_UI_IMAGEPANEL_HPP
