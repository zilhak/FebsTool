//
// Created by bogonets on 2018. 4. 12..
//

#ifndef FEPSTOOL_SEGMENTATIONTOOLBAR_HPP
#define FEPSTOOL_SEGMENTATIONTOOLBAR_HPP

#include <wx/wx.h>
#include "../../data/ThemeData.hpp"
#include "../../data/XmlConfig.hpp"

class SegmentationToolBar : public wxPanel
{
private:
    std::vector<Name> _name_list;

private:
    wxComboBox * _name_box;
    wxComboBox * _pose_box;
    wxComboBox * _trun_box;
    wxComboBox * _diff_box;

public:
    SegmentationToolBar(wxWindow * parent, wxWindowID id);
    ~SegmentationToolBar();

private:
    void initialize();
    void initializeComponent();
    void initializeSetting();
    void initializeStyle();

public:
    void setNameBox(std::vector<Name> const & name_list, wxString const & name);

public:
    wxString getName();
    wxString getColour();
    wxString getPose();
    wxString getTruncated();
    int getDifficult();
};


#endif //FEPSTOOL_SEGMENTATIONTOOLBAR_HPP
