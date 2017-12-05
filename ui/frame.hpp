#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/listctrl.h>
#include <wx/filename.h>

#include "ui/ImagePanel.hpp"
#include "ui/FileExplorer.hpp"

#include <set>

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
    std::set<wxString> _image_extension;

private:
    wxPanel * _tool_bar;
    FileExplorer * _file_list;
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
    void initializeSetting();
    void initializeToolBar(wxBoxSizer * sizer);
    void initializeImageViewer(wxBoxSizer * sizer);

private:
    void addToListCtrl(wxString const directory);

private:
    bool fileExtCheck(wxString const extension);

private: // menu event.
    void onScaleComboBox(wxCommandEvent & event);
    void onOpenButton(wxCommandEvent & event);
    void onListDoubleClick(wxListEvent & event);
    void onExplore();

private: // input event.
    void onMouseEvent(wxMouseEvent & event);
    void onKeyboardEvent(wxKeyEvent & event);

private:
    void prevFile();
    void nextFile();
};

