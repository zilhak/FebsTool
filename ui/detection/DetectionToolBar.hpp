#ifndef FEPSTOOL_DETECTIONTOOLBAR_HPP
#define FEPSTOOL_DETECTIONTOOLBAR_HPP

#include <wx/wx.h>

class DetectionToolBar : public wxPanel
{
private:
    wxComboBox * _type_box;
    wxComboBox * _diff_box;

public:
    DetectionToolBar(wxWindow * parent, wxWindowID id);
    ~DetectionToolBar();

private:
    void initialize();
    void initializeComponent();
    void initializeDefaultData();

public:
    wxString getType();
    wxString getDifficult();

public:
    void setType(wxString type);
    void setDifficult(int difficult);
};


#endif //FEPSTOOL_DETECTIONTOOLBAR_HPP
