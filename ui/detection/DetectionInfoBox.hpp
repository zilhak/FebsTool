//
// Created by bogonets on 2018. 4. 3..
//

#ifndef FEPSTOOL_DETECTIONINFOBOX_HPP
#define FEPSTOOL_DETECTIONINFOBOX_HPP

#include <wx/wx.h>

class DetectionInfoBox : public wxPanel
{
public:
    enum ID {
        ZOOM_BOX
    };

private:
    wxStaticText * _folder_name;
    wxStaticText * _folder_image_count;
    wxStaticText * _folder_image_idx;
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
    DetectionInfoBox(wxWindow * parent, wxWindowID id = wxID_ANY);
    ~DetectionInfoBox();

private:
    void initialize();
    void initializeSetting();
    void initializeComponent();
    void initializeStyle();

public:
    void setZoomBox(int min, int max, int interval);
    void setFolderName(wxString const & name);
    void setFolderSize(int size);
    void setImageIndex(int index);
    void setImageName(wxString const & name);
    void setImageSize(wxString const & size);
    void setMouseInfo(wxPoint const & point);

public:
    void decreaseFolderSize();

public:
    void zoomIn();
    void zoomOut();
    void changeZoomBox(int ratio);

public:
    int getZoom();
    int getImageScale();
};


#endif //FEPSTOOL_DETECTIONINFOBOX_HPP
