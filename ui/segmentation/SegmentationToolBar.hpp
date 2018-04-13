//
// Created by bogonets on 2018. 4. 12..
//

#ifndef FEPSTOOL_SEGMENTATIONTOOLBAR_HPP
#define FEPSTOOL_SEGMENTATIONTOOLBAR_HPP

#include <wx/wx.h>
#include <data/ThemeData.hpp>

class SegmentationToolBar : public wxPanel
{
private:
    wxComboBox * _name_box;
    wxComboBox * _pose_box;
    wxComboBox * _trun_box;
    wxComboBox * _diff_box;

public:
    SegmentationToolBar(wxWindow * parent, wxWindowID id);
    ~SegmentationToolBar();

public:
    void initialize();
    void initializeComponent();
    void initializeSetting();
    void initializeStyle();

public:
    wxString getName();
    wxString getPose();
    wxString getTruncated();
    wxString getDifficult();
};


#endif //FEPSTOOL_SEGMENTATIONTOOLBAR_HPP
