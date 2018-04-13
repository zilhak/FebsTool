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
public:
    DECLARE_EVENT_TABLE();

private:
    FileOpenPanel * _open;
    SegmentationToolBar * _toolbar;
    SegmentationInfoBox * _infobox;
    FileExplorer * _file_list;
    ImagePanel * _image_panel;

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
    void onFileOpen(wxCommandEvent & event);
};


#endif //FEPSTOOL_SEGMENTATIONFRAME_HPP
