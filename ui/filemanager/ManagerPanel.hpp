

#ifndef FEPSTOOL_MANAGERPANEL_HPP
#define FEPSTOOL_MANAGERPANEL_HPP

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/listctrl.h>
#include <wx/filename.h>
#include <wx/dcbuffer.h>
#include <ETC/RunningTimeChecker.hpp>

#include <stack>
#include <list>

class ManagerPanel : public wxPanel
{
public:
    DECLARE_EVENT_TABLE();

private:
    long _iter = 0;
    bool _load = false;
    wxString _path;

private:
    int _row = 5;
    int _col = 5;
    int _width = 0;
    int _height = 0;
    int _space_x = 5;
    int _space_y = 5;

private:
    std::vector<wxRect> _image_pos_vector;
    std::vector<wxImage> _image_vector;
    std::vector<wxBitmap> _resized_bitmap_vector;

public:
    ManagerPanel(wxWindow * parent, wxWindowID id);
    virtual ~ManagerPanel();

private:
    void initialize();

private:
    void onMouse(wxMouseEvent & event);
    void onPaint(wxPaintEvent & event);

private:
    void drawImages(wxDC & dc);

public:
    void setPath(wxString const & path);
    void setImages(std::vector<wxString> const & file_list);
    void setGrid(int row, int col);
    void convertImage(std::vector<wxImage> & input, std::vector<wxBitmap> & output);

public:
    int getImageRectIndex(wxPoint const & pos);

public:
    bool teardown();
};


#endif //FEPSTOOL_MANAGERPANEL_HPP
