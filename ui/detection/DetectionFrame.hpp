#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/listctrl.h>
#include <wx/filename.h>

#include <ui/component/ImagePanel.hpp>
#include <ui/component/FileExplorer.hpp>

#include <set>
#include <vector>
#include <algorithm>


class DetectionFrame : public wxDialog
{
public:
    wxDECLARE_EVENT_TABLE();

public:
    enum ID {
        BUTTON_OPEN,
        BUTTON_XML,
        COMBO_SIZE,
        COMBO_TYPE,
        COMBO_SCALE,
        COMBO_DIFFICULT,
        IMAGE_VIEWER,
        FILE_EXPLORER
    };

private:
    wxDir * _dir;
    wxString _current_file;
    std::set<wxString> _image_extension;
    std::vector<wxString> _file_list;
    long _image_index;

private:
    wxPanel * _tool_bar;
    FileExplorer * _file_list_viewer;
    wxPanel * _image_info_box;
    ImagePanel * _image_viewer;

private:
    wxStaticText * _info_image_name;
    wxStaticText * _info_image_size;
    wxStaticText * _info_mouse_x;
    wxStaticText * _info_mouse_y;

private:
    wxButton * _open_button;
    wxButton * _close_button;
    wxComboBox * _size_combobox;
    wxComboBox * _type_combobox;
    wxComboBox * _scale_combobox;
    wxComboBox * _difficult_combobox;

public:
    DetectionFrame(wxString const & title);
    ~DetectionFrame();

private:
    void initialize();
    void initializeStyle();
    void initializeSetting();
    void initializeToolBar(wxBoxSizer * sizer);
    void initializeLeftMenu(wxBoxSizer * h_sizer);
    void initializeImageViewer(wxBoxSizer * h_sizer);

private:
    void makeFileList(wxString const directory);

private:
    void showImage(wxString const & file_name);

private:
    bool fileExtCheck(wxString const extension);

private: // menu event.
    void onSizeComboBox(wxCommandEvent &event);
    void onTypeComboBox(wxCommandEvent &event);
    void onScaleComboBox(wxCommandEvent &event);
    void onDifficultComboBox(wxCommandEvent &event);
    void onOpenButton(wxCommandEvent & event);
    void onListDoubleClick(wxListEvent & event);
    void onExplore();

private: // input event.
    void onMouseEvent(wxMouseEvent & event);
    void onKeyboardEvent(wxKeyEvent & event);

private:
    void prevFile();
    void nextFile();

private:
    void refresh();
};

