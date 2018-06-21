//
// Created by bogonets on 2018. 6. 21..
//

#ifndef FEPSTOOL_FILEMANAGERFRAME_HPP
#define FEPSTOOL_FILEMANAGERFRAME_HPP

#include <wx/wx.h>

class FileManagerFrame : public wxDialog
{
public:
    FileManagerFrame(wxWindow * parent, wxWindowID id);
    virtual ~FileManagerFrame();

private:
    void initialize();
    void initializeOpen(wxSizer * sizer);
    void initializeLeftPanel(wxSizer * sizer);
    void initializeRightPanel(wxSizer * sizer);

};


#endif //FEPSTOOL_FILEMANAGERFRAME_HPP
