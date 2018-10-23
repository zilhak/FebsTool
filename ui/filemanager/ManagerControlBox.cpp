#include <ui/filemanager/ManagerControlBox.hpp>

ManagerControlBox::ManagerControlBox(wxWindow * parent, wxWindowID winid) : wxPanel(parent, winid)
{
    initialize();
}

ManagerControlBox::~ManagerControlBox()
{
    /* EMPTY */
}

void ManagerControlBox::initialize()
{
    wxBoxSizer * whole_sizer = new wxBoxSizer(wxVERTICAL);
    initializeInfo(whole_sizer);
    initializeControl(whole_sizer);
    initializeData();

    SetSizer(whole_sizer);
}

void ManagerControlBox::initializeInfo(wxSizer * sizer)
{
    wxFlexGridSizer * grid_sizer = new wxFlexGridSizer(1, 2, 5, 5);

    _text_filename = new wxStaticText(this, wxID_ANY, "-----");

    grid_sizer->Add(new wxStaticText(this, wxID_ANY, "File Name : "));
    grid_sizer->Add(_text_filename);

    sizer->Add(grid_sizer, 0, wxEXPAND);
}

void ManagerControlBox::initializeControl(wxSizer * sizer)
{
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    _spin_col = new wxSpinCtrl(this, wxID_ANY, "2", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER, 1, 8, 5);
    _spin_row = new wxSpinCtrl(this, wxID_ANY, "2", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER, 1, 8, 5);

    h_sizer->Add(new wxStaticText(this, wxID_ANY, " ROW : "));
    h_sizer->Add(_spin_row, 1);
    h_sizer->Add(new wxStaticText(this, wxID_ANY, " COL : "));
    h_sizer->Add(_spin_col, 1);

    sizer->Add(h_sizer);
}

void ManagerControlBox::initializeData()
{

}