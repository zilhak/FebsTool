#include <wx/wx.h>

class HelpDialog : public wxDialog
{
public:
    DECLARE_EVENT_TABLE();

private:
    wxBitmap _bitmap;

public:
    HelpDialog(wxWindow * parent);
    ~HelpDialog();

public:
    void onPaint(wxPaintEvent & event);
};

