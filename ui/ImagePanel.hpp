#include <wx/wx.h>
#include "data/XmlConfig.hpp"

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
    wxBitmap _background_bitmap = wxBitmap();
    int _image_width;
    int _image_height;
    wxString _image_filepath;
    wxString _image_filename;
    int _image_x = 15;
    int _image_y = 15;

private:
    int _mouse_x;
    int _mouse_y;
    int _x1 = -1;
    int _y1;
    int _x2;
    int _y2;
    double _scale_setting = 1;
    STATUS mode = STATUS::IDLE;
    bool _is_ready = false;

public:
    ImagePanel(wxWindow * parent, wxWindowID id);
    virtual ~ImagePanel();

public:
    void changeScale(double scale);
    void save();
    void setBackgroundImage(wxString filepath, wxString filename);

public:
    bool isReady() {return _is_ready;};

private:
    void onPaint(wxPaintEvent & event);
    void onMouse(wxMouseEvent & event);

private:
    void locationAdjust(int & x, int & y);
};


#endif //IMAGESEARCHER_UI_IMAGEPANEL_HPP
