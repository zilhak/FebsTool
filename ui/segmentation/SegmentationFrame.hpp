#include <wx/wx.h>

#ifndef FEPSTOOL_SEGMENTATIONFRAME_HPP
#define FEPSTOOL_SEGMENTATIONFRAME_HPP


class SegmentationFrame : public wxDialog
{
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

};


#endif //FEPSTOOL_SEGMENTATIONFRAME_HPP
