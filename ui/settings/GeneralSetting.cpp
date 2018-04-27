#include "GeneralSetting.hpp"

BEGIN_EVENT_TABLE(GeneralSetting, wxPanel)
    EVT_SPINCTRL(wxID_ANY, GeneralSetting::onSpinCtrl)
END_EVENT_TABLE()


GeneralSetting::GeneralSetting(wxWindow * parent, ConfigData & data) : wxPanel(parent)
{
    _config = &data;
    initialize();
}

GeneralSetting::~GeneralSetting()
{
    /* EMPTY */
}

void GeneralSetting::initialize()
{
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);

    initializeTitle(v_sizer, wxT(""));
    initializeInterval(v_sizer);
    initializeTitle(v_sizer, wxT("Limit Image Size"));
    initializeSizeLimit(v_sizer);

    SetSizer(v_sizer);
}

void GeneralSetting::initializeTitle(wxBoxSizer * v_sizer, wxString const & str)
{
    v_sizer->Add(new wxStaticText(this, wxID_ANY, str), 0, wxTOP | wxLEFT, 10);
    v_sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
}

void GeneralSetting::initializeInterval(wxBoxSizer * v_sizer)
{
    wxPanel * panel = new wxPanel(this, wxID_ANY);
    wxGridSizer * grid_sizer = new wxGridSizer(2, wxSize(5,5));

    _zoom_min_ctrl = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 1000, _config->zoom_min);
    _zoom_max_ctrl = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 20, 5000, _config->zoom_max);
    _zoom_interval_ctrl = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 1, 2500, _config->zoom_interval);
    
    grid_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("�� �ּҰ� : ")), 0, wxLEFT, 5);
    grid_sizer->Add(_zoom_min_ctrl, 0, wxLEFT, 5);
    grid_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("�� �ִ밪 : ")), 0, wxLEFT, 5);
    grid_sizer->Add(_zoom_max_ctrl, 0, wxLEFT, 5);
    grid_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("�� ����(��) : ")), 0, wxLEFT, 5);
    grid_sizer->Add(_zoom_interval_ctrl, 0, wxLEFT, 5);

    panel->SetSizer(grid_sizer);

    v_sizer->Add(panel, 0, wxTOP | wxBottom, 10);
}

void GeneralSetting::initializeSizeLimit(wxBoxSizer * v_sizer)
{
    wxPanel * panel = new wxPanel(this, wxID_ANY);
    wxGridSizer * grid_sizer = new wxGridSizer(2, wxSize(5,5));

    _min_size_x = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -1, 2000, -1);
    _min_size_y = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -1, 2000, -1);
    _max_size_x = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -1, 10000, -1);
    _max_size_y = new wxSpinCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -1, 10000, -1);
    
    grid_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("�ּ� ���� :")), 0, wxLEFT, 5);
    grid_sizer->Add(_min_size_x, 0, wxLEFT, 5);
    grid_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("�ּ� ���� :")), 0, wxLEFT, 5);
    grid_sizer->Add(_min_size_y, 0, wxLEFT, 5);
    grid_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("�ִ� ���� :")), 0, wxLEFT, 5);
    grid_sizer->Add(_max_size_x, 0, wxLEFT, 5);
    grid_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("�ִ� ���� :")), 0, wxLEFT, 5);
    grid_sizer->Add(_max_size_y, 0, wxLEFT, 5);

    panel->SetSizer(grid_sizer);

    v_sizer->Add(panel, 0, wxTOP | wxBOTTOM | wxALIGN_CENTER, 10);
    panel->Hide();
}

void GeneralSetting::onSpinCtrl(wxSpinEvent & event)
{
    _config->zoom_min = _zoom_min_ctrl->GetValue();
    _config->zoom_max = _zoom_max_ctrl->GetValue();
    _config->zoom_interval = _zoom_interval_ctrl->GetValue();
    _config->minimum_size.x = _min_size_x->GetValue();
    _config->minimum_size.y = _min_size_y->GetValue();
    _config->maximum_size.x = _max_size_x->GetValue();
    _config->maximum_size.y = _max_size_y->GetValue();
}
