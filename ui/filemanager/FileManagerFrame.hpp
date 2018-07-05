//
// Created by bogonets on 2018. 6. 21..
//

#ifndef FEPSTOOL_FILEMANAGERFRAME_HPP
#define FEPSTOOL_FILEMANAGERFRAME_HPP

#include <wx/wx.h>
#include <ui/filemanager/ManagerControlBox.hpp>
#include <ui/filemanager/ManagerPanel.hpp>

class FileManagerFrame : public wxDialog
{
private:
    ManagerControlBox * _control_box;
    ManagerPanel * _screen;

public:
    FileManagerFrame(wxString const & title);
    virtual ~FileManagerFrame();

private:
    void initialize();
    void initializeOpen(wxSizer * sizer);
    void initializeLeftPanel(wxSizer * sizer);
    void initializeRightPanel(wxSizer * sizer);

};


#endif //FEPSTOOL_FILEMANAGERFRAME_HPP
