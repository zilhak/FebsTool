#include <wx/wx.h>

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

public:

public:
    ImagePanel(wxWindow * parent, wxWindowID id);
    virtual ~ImagePanel();

public:
    void setBackgroundImage();
    void onPaint(wxPaintEvent & event);
    void onMouse(wxMouseEvent & event);
};


#endif //IMAGESEARCHER_UI_IMAGEPANEL_HPP
