#pragma once
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/colordlg.h>
#include "../../data/XmlConfig.hpp"
#include <memory>

class NameBoxSetting : public wxPanel
{
public:
    enum ID : int {
        BUTTON_ADD,
        BUTTON_DEFAULT
    };

public:
    DECLARE_EVENT_TABLE();

private:
    ConfigData * _config;
    wxComboBox * _default_combo;
    wxGrid * _item_grid;

public:
    NameBoxSetting(wxWindow * parent, ConfigData & config);
    ~NameBoxSetting();

private:
    void initialize();
    void initializeData();
    void initializeComponent(wxSizer * v_sizer);
    void initializeGrid(wxSizer * v_sizer);
    void initializeButtons(wxSizer * v_sizer);
    void initializeSetting();

private:
    void onAddButton(wxCommandEvent & event);
    void onDefaultButton(wxCommandEvent & event);
    void onDefaultCombo(wxCommandEvent & event);
    void onGridChange(wxGridEvent & event);
    void onGridClick(wxGridEvent & event);
    void onGridDClick(wxGridEvent & event);

private:
    void refresh();
    void refreshDefault();
};

//
//class TableRenderer : public wxGridCellStringRenderer
//{
//private:
//    wxString _col_str;
//
//public:
//    TableRenderer(wxString const & col_str);
//    virtual ~TableRenderer();
//
//public:
//    virtual void Draw(wxGrid & grid,
//                      wxGridCellAttr & attr,
//                      wxDC & dc,
//                      wxRect const & rect,
//                      int row,
//                      int col,
//                      bool isSelected) wxOVERRIDE;
//};