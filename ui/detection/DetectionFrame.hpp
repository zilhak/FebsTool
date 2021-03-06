#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/listctrl.h>
#include <wx/filename.h>

#include <ui/component/ImagePanel.hpp>
#include <ui/component/FileExplorer.hpp>
#include <ui/detection/DetectionInfoBox.hpp>
#include <ui/detection/DetectionToolBar.hpp>
#include <ui/settings/SettingFrame.hpp>
#include <data/KeyboardInputManager.hpp>

#include <set>
#include <vector>
#include <algorithm>
#include <deque>


class DetectionFrame : public wxDialog
{
public:
    wxDECLARE_EVENT_TABLE();

public:
    enum ButtonID {
        OPEN,
        SETTING
    };
    enum ID {
        COMBO_SIZE,
        IMAGE_VIEWER,
    };

private:
    wxString _dir;
    wxString _current_file;
    std::set<wxString> _image_extension;
    std::vector<wxString> _folder_name_list;
    std::deque<std::pair<wxString, wxString>> _undo_map;

private:
    int _previous_mouse_x;
    int _previous_mouse_y;

private:
    FileExplorer * _file_list;
    DetectionToolBar * _toolbar;
    DetectionInfoBox * _infobox;
    ImagePanel * _image_panel;
    KeyboardInputManager _input_manager;

public:
    DetectionFrame(wxString const & title);
    ~DetectionFrame();

private:
    void initialize();
    void initializeStyle();
    void initializeSetting();
    void initializeToolBar(wxBoxSizer * v_sizer);
    void initializeLeftMenu(wxBoxSizer * h_sizer);
    void initializeImageViewer(wxBoxSizer * sizer);

private: // menu event.
    void onZoomBox(wxCommandEvent &event);
    void onOpenButton(wxCommandEvent & event);
    void onSettingButton(wxCommandEvent & event);
    void onSelectFile(wxListEvent & event);

private: // input event.
    void onMouseEvent(wxMouseEvent & event);
    void onKeyboardEvent(wxKeyEvent & event);

private:
    void prev();
    void next();

private:
    void setImagePanel();
    void applySetting();

private:
    int getAction(int key);

private:
    void moveFile(wxString const & folder_name);
    void undo();
};

