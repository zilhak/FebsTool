//
// Created by bogonets on 2018. 4. 12..
//

#ifndef FEPSTOOL_SEGMENTATIONINFOBOX_HPP
#define FEPSTOOL_SEGMENTATIONINFOBOX_HPP

#include <wx/wx.h>
#include "../../data/ThemeData.hpp"

class SegmentationInfoBox : public wxPanel
{
public:
    enum ID {
        ZOOM_BOX
    };

private:
    wxStaticText * _image_name;
    wxStaticText * _image_size;
    wxStaticText * _mouse_x;
    wxStaticText * _mouse_y;
    wxComboBox * _size_box;
    wxComboBox * _image_scale_box;

private:
    int _min;
    int _max;
    int _interval;

public:
    SegmentationInfoBox(wxWindow * parent, wxWindowID id);
    ~SegmentationInfoBox();

private:
    void initialize();
    void initializeComponent();
    void initializeSetting();
    void initializeStyle();

public:
    void setZoomBox(int min, int max, int interval);
    void setImageName(wxString const & name);
    void setImageSize(wxString const & size);
    void setMouseInfo(wxPoint const & point);

public:
    void zoomIn();
    void zoomOut();
    void changeZoomBox(int ratio);

public:
    int getZoom();
    int getImageScale();
};


#endif //FEPSTOOL_SEGMENTATIONINFOBOX_HPP
