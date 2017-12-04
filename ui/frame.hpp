#include <wx/wx.h>
#include <wx/dir.h>
#include "ui/ImagePanel.hpp"

class Frame : public wxFrame
{
public:
    wxDECLARE_EVENT_TABLE();

public:
    enum ID {
        BUTTON_OPEN,
        BUTTON_CLOSE,
        COMBO_SCALE
    };

private:
    wxDir * _dir;
    wxString _current_file;
    wxString _temp_file;

private:
    wxPanel * _tool_bar;
    ImagePanel * _image_viewer;

private:
    wxButton * _open_button;
    wxButton * _close_button;
    wxComboBox * _scale_combobox;

private:
    bool _use_prev = false;

public:
    Frame(const wxString & title);
    ~Frame();

private:
    void initialize();
    void initializeStyle();
    void initializeToolBar(wxBoxSizer * sizer);
    void initializeImageViewer(wxBoxSizer * sizer);

private: // menu event.
    void onScaleComboBox(wxCommandEvent & event);
    void onOpenButton(wxCommandEvent & event);
    void onExplore();

private: // input event.
    void onMouseEvent(wxMouseEvent & event);
    void onKeyboardEvent(wxKeyEvent & event);

private:
    void prevFile();
    void nextFile();
};

