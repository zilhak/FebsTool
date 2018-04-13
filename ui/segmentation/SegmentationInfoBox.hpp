//
// Created by bogonets on 2018. 4. 12..
//

#ifndef FEPSTOOL_SEGMENTATIONINFOBOX_HPP
#define FEPSTOOL_SEGMENTATIONINFOBOX_HPP

#include <wx/wx.h>
#include <data/ThemeData.hpp>

class SegmentationInfoBox : public wxPanel
{
private:
    wxStaticText * _image_name;
    wxStaticText * _image_size;
    wxStaticText * _mouse_x;
    wxStaticText * _mouse_y;
    wxComboBox * _size_box;
    wxComboBox * _image_scale_box;


public:
    SegmentationInfoBox(wxWindow * parent, wxWindowID id);
    ~SegmentationInfoBox();

public:
    void initialize();
    void initializeComponent();
    void initializeSetting();
    void initializeStyle();
};


#endif //FEPSTOOL_SEGMENTATIONINFOBOX_HPP
