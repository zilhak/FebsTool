

#ifndef FEPSTOOL_MANAGERPANEL_HPP
#define FEPSTOOL_MANAGERPANEL_HPP

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/listctrl.h>
#include <wx/filename.h>
#include <stack>
#include <list>

class ManagerPanel : public wxPanel
{
public:
    DECLARE_EVENT_TABLE();

private:
    long _iter = 0;
    bool _load = false;

private:
    std::vector<wxRect> _image_pos_vector;

public:
    ManagerPanel(wxWindow * parent, wxWindowID id);
    virtual ~ManagerPanel();

private:
    void initialize();

private:
    void onMouse(wxMouseEvent & event);
    void onPaint(wxPaintEvent & event);

public:
    bool teardown();
};


#endif //FEPSTOOL_MANAGERPANEL_HPP
