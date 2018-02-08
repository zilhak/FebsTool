#include <wx/wx.h>
#include "data/XmlConfig.hpp"
#include <wx/filename.h>

#ifndef IMAGESEARCHER_UI_IMAGEPANEL_HPP
#define IMAGESEARCHER_UI_IMAGEPANEL_HPP

class ImagePanel : public wxPanel {
public:
    wxDECLARE_EVENT_TABLE();

public:
    enum STATUS {
        IDLE,
        MOUSE_DOWN
    };

private:
    wxBitmap _background_bitmap;
    wxFileName _image_file;
    int _image_width;
    int _image_height;
    int _bitmap_width; // arranged width
    int _bitmap_height; // arranged hieght
    int _image_x = 15;
    int _image_y = 15;
    wxString _image_type = "car";
    int _image_depth = 3;
    int _image_diff = 1;

private:
    int _mouse_x;
    int _mouse_y;
    cyRect _check;
    cyRect _temp_rect;
    bool _xml_exists = false;
    double _scale_setting = 1;
    STATUS mode = STATUS::IDLE;
    bool _click = false;
    bool _is_ready = false;

public:
    ImagePanel(wxWindow * parent, wxWindowID id);
    virtual ~ImagePanel();

public:
    void setSize(double scale) {_scale_setting = scale;}
    void setType(wxString type) {_image_type = type;}
    void setDepth(int depth) {_image_depth = depth;}
    void setDiff(int diff) {_image_diff = diff;}
    void save();
    void load();
    void setBackgroundImage(wxString filepath, wxString filename);
    void saveCropImage();

public:
    int getImageWidth() const {return _image_width;}
    int getImageHeight() const {return _image_height;}
    void firstPointUp();
    void firstPointDown();
    void firstPointLeft();
    void firstPointRight();

public:
    bool isReady() {return _is_ready;};

private:
    void onPaint(wxPaintEvent & event);
    void onMouse(wxMouseEvent & event);

private:
    void locationAdjust(int & x, int & y);
};


#endif //IMAGESEARCHER_UI_IMAGEPANEL_HPP
