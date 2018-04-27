#include <wx/wx.h>

#include <ui/segmentation/SegmentationToolBar.hpp>
#include <ui/segmentation/SegmentationInfoBox.hpp>
#include <ui/component/FileOpenPanel.hpp>
#include <ui/component/FileExplorer.hpp>
#include <ui/component/ImagePanel.hpp>
#include <data/ThemeData.hpp>
#include <data/EventData.hpp>

#ifndef FEPSTOOL_SEGMENTATIONFRAME_HPP
#define FEPSTOOL_SEGMENTATIONFRAME_HPP


class SegmentationFrame : public wxDialog
{
private:
    enum ButtonID {
        OPEN = 11,
        SETTING,
        HELP
    };
    enum STATUS {
        NOT_LOADED = 20,
        IDLE,
        DRAWING
    };

public:
    DECLARE_EVENT_TABLE();

private:
    wxPanel * _open;
    SegmentationToolBar * _toolbar;
    SegmentationInfoBox * _infobox;
    FileExplorer * _file_list;
    ImagePanel * _image_panel;

private:
    wxString _dir;

private:
    int _zoom_value = 10;
    int _previous_mouse_x;
    int _previous_mouse_y;

private:
    STATUS _status = STATUS::NOT_LOADED;
    std::vector<wxPoint> _draw_points;

public:
    SegmentationFrame(wxString const & title);
    ~SegmentationFrame();

private:
    void initialize();
    void initializeStyle();
    void initializeSetting();
    void initializeToolBar(wxBoxSizer * sizer);
    void initializeLeftMenu(wxBoxSizer * h_sizer);
    void initializeImageViewer(wxBoxSizer * h_sizer);

private:
    Object createNewObject();
    void setImagePanel();
    void prev();
    void next();

private:
    void applySetting();

private:
    void onOpenButton(wxCommandEvent & event);
    void onSettingButton(wxCommandEvent & event);
    void onHelpButton(wxCommandEvent & event);
    void onMouse(wxMouseEvent & event);
    void onKeyboard(wxKeyEvent & event);
    void onSelectFile(wxListEvent & event);
    void onZoom(wxCommandEvent & event);

};

#endif //FEPSTOOL_SEGMENTATIONFRAME_HPP
