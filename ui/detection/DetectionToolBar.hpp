#ifndef FEPSTOOL_DETECTIONTOOLBAR_HPP
#define FEPSTOOL_DETECTIONTOOLBAR_HPP

#include <wx/wx.h>
#include <data/FebsStruture.hpp>

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
    int getDifficult();

public:
    void setType(wxString type);
    void setType(int num);
    void setDifficult(int difficult);

public:
    void setNameBox(std::vector<Name> const & class_list, wxString const & default_class);
};


#endif //FEPSTOOL_DETECTIONTOOLBAR_HPP
