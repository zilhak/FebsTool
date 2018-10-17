#include <ui/settings/NameBoxSetting.hpp>

BEGIN_EVENT_TABLE(NameBoxSetting, wxPanel)
    EVT_BUTTON(ID::BUTTON_ADD, NameBoxSetting::onAddButton)
    EVT_BUTTON(ID::BUTTON_DEFAULT, NameBoxSetting::onDefaultButton)
//    EVT_GRID_CELL_LEFT_DCLICK(NameBoxSetting::onGridDClick)
//    EVT_GRID_CELL_CHANGED(NameBoxSetting::onGridChange)
//    EVT_GRID_CELL_LEFT_CLICK(NameBoxSetting::onGridClick)
    EVT_COMBOBOX(wxID_ANY, NameBoxSetting::onDefaultCombo)
END_EVENT_TABLE()

constexpr static const int GRID_COL_TYPE = 0;
constexpr static const int GRID_COL_COLOUR = 1;
constexpr static const int GRID_COL_DELETE = 2;

NameBoxSetting::NameBoxSetting(wxWindow * parent, ConfigData & config) : wxPanel(parent, wxID_ANY)
{
    _config = &config;
    initialize();
}

NameBoxSetting::~NameBoxSetting()
{
    /* EMPTY */
}

void NameBoxSetting::initialize()
{
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);
    initializeComponent(v_sizer);
    initializeGrid(v_sizer);
    initializeButtons(v_sizer);
    initializeSetting();
    SetSizer(v_sizer);

    refresh();
    Layout();
}

void NameBoxSetting::initializeComponent(wxSizer * v_sizer)
{
    wxPanel * default_panel = new wxPanel(this);
    wxBoxSizer * default_sizer = new wxBoxSizer(wxHORIZONTAL);
    _default_combo = new wxComboBox(default_panel, wxID_ANY, "NULL", wxDefaultPosition, wxSize(120, -1));

    default_sizer->Add(new wxStaticText(default_panel, wxID_ANY, "Default Name : "), 0, wxLEFT | wxTOP | wxALIGN_CENTER, 15);
    default_sizer->Add(_default_combo, 0, wxTOP | wxALIGN_CENTER, 15);

    default_panel->SetSizer(default_sizer);

    v_sizer->Add(default_panel);
}

void NameBoxSetting::initializeGrid(wxSizer * v_sizer)
{
    _item_grid = new wxGrid(this, wxID_ANY);
    _item_grid->CreateGrid(0, 0);
    _item_grid->SetRowLabelSize(0);

    std::vector<wxString> label_list = { wxT("Name"), wxT("Color"), wxT("Delete") };

    int label_size = label_list.size();
    _item_grid->AppendCols(label_size);
//    _item_grid->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

    for (int i = 0; i < label_size; ++i) {
        _item_grid->SetColLabelValue(i, label_list[i]);
    }

    _item_grid->SetColSize(GRID_COL_TYPE, 300);
    _item_grid->SetColSize(GRID_COL_COLOUR, 140);
    _item_grid->SetColSize(GRID_COL_DELETE, 40);

    v_sizer->Add(_item_grid, 1, wxEXPAND | wxTOP | wxBOTTOM, 10);
}

void NameBoxSetting::initializeButtons(wxSizer * v_sizer)
{
    wxPanel * button_panel = new wxPanel(this);
    wxBoxSizer * button_sizer = new wxBoxSizer(wxHORIZONTAL);


    wxButton * add_button = new wxButton(button_panel, ID::BUTTON_ADD, wxT("Add Name"));
    wxButton * default_button = new wxButton(button_panel, ID::BUTTON_DEFAULT, wxT("Reset To Default"));

    button_sizer->Add(add_button, 0, wxLEFT, 5);
    button_sizer->Add(default_button, 0, wxLEFT, 5);

    button_panel->SetSizer(button_sizer);
    v_sizer->Add(button_panel, 0, wxALIGN_RIGHT);
}

void NameBoxSetting::initializeSetting()
{
    if (_config->init) {
        Name init_name;
        std::vector<wxString> name_list = {wxT("car"), wxT("bus"), wxT("truck")};
        for (auto const & name : name_list) {
            init_name.name = name;
            init_name.colour = wxString::Format("%ul", 0x00FFFF);
            _config->class_list.push_back(init_name);
        }
        _config->default_class = name_list[0];
    }

    _default_combo->SetEditable(false);
    _default_combo->SetValue(_config->default_class);

    for (auto const & item : _config->class_list) {
        _default_combo->Append(item.name);
    }
}

void NameBoxSetting::refresh()
{
//    wxString default_text = _default_combo->GetValue();
//    _default_combo->Clear();
//    bool exist_name = false;
//    if (_item_grid->GetNumberRows() > 0) {
//        _item_grid->DeleteRows(0, _item_grid->GetNumberRows());
//    }
//
//    for (auto const & item : _config->class_list) {
//        if (default_text == item.name) {
//            exist_name = true;
//        }
//        _default_combo->Append(item.name);
//
//        int index = _item_grid->GetNumberRows();
//        _item_grid->InsertRows(index);
//        _item_grid->SetCellValue(index, GRID_COL_TYPE, item.name);
//
////        TableRenderer * render = new TableRenderer(item.colour);
////        _item_grid->SetCellRenderer(index, GRID_COL_COLOUR, render);
//        _item_grid->SetCellValue(index, GRID_COL_COLOUR, item.colour);
//
//        _item_grid->SetCellRenderer(index, GRID_COL_DELETE, new wxGridCellBoolRenderer);
//        _item_grid->SetCellValue(index, GRID_COL_TYPE, item.name);
//    }
//    _default_combo->ChangeValue(_config->default_class);
//
//    if (!exist_name) {
//        if (_config->class_list.size() > 0) {
//            _default_combo->SetValue(_config->class_list[0].name);
//        } else {
//            _default_combo->SetValue("NULL");
//        }
//    }

    Refresh();
}

void NameBoxSetting::refreshDefault()
{
    wxString default_text = _default_combo->GetValue();
    _default_combo->Clear();
    bool exist_name = false;
    for (auto const & item : _config->class_list) {
        if (default_text == item.name) {
            exist_name = true;
        }
        _default_combo->Append(item.name);
    }

    if (!exist_name) {
        if (_config->class_list.size() > 0) {
            _default_combo->SetValue(_config->class_list[0].name);
        } else {
            _default_combo->SetValue("NULL");
        }
    }
}

void NameBoxSetting::onAddButton(wxCommandEvent & event)
{
    int i = 0;
    wxString new_name;
    bool exist_name = true;
    while (exist_name) {
        exist_name = false;
        ++i;
        new_name = wxString::Format("ITEM%d", i);
        for (auto const & item : _config->class_list) {
            if (new_name == item.name) {
                exist_name = true;
            }
        }
    }

    Name new_item;
    new_item.name = new_name;
    new_item.colour = wxString::Format("%ul", 0x00FFFF);

    _config->class_list.push_back(new_item);

    refresh();
}

void NameBoxSetting::onDefaultButton(wxCommandEvent & event)
{
    _config->class_list.clear();

    Name init_name;
    std::vector<wxString> name_list = {wxT("car"), wxT("bus"), wxT("truck")};
    for (auto const & name : name_list) {
        init_name.name = name;
        init_name.colour = wxString::Format("%ul", 0x00FFFF);
        _config->class_list.push_back(init_name);
    }

    refresh();
}

void NameBoxSetting::onDefaultCombo(wxCommandEvent & event)
{
    _config->default_class = _default_combo->GetValue();
}

void NameBoxSetting::onGridChange(wxGridEvent & event)
{
    int row = event.GetRow();
    int col = event.GetCol();

    if (col == GRID_COL_TYPE) {
        wxString name = _item_grid->GetCellValue(row, col);
        _config->class_list[row].name = name;

        refreshDefault();
    }
}

void NameBoxSetting::onGridClick(wxGridEvent & event)
{
//    int row = event.GetRow();
//    int col = event.GetCol();
//
//    if (col == GRID_COL_DELETE) {
//        wxString name = _item_grid->GetCellValue(row, GRID_COL_TYPE);
//        wxMessageDialog * confirm = new wxMessageDialog(this, wxString("[") + name + wxT("] Delete item?"), wxT("Delete"), wxYES_NO);
//        if (confirm->ShowModal() == wxID_YES) {
//            for (int i = 0; i < _config->class_list.size(); ++i) {
//                if (_config->class_list[i].name == name) {
//                    _config->class_list.erase(_config->class_list.begin() + i);
//                    break;
//                }
//            }
//            refresh();
//            return;
//        }
//    }
//    else if (col == GRID_COL_COLOUR) {
//        _item_grid->SelectRow(row);
//        return;
//    }
//    event.Skip();
}

void NameBoxSetting::onGridDClick(wxGridEvent & event)
{
//    int row = event.GetRow();
//    int col = event.GetCol();
//
//    if (col == GRID_COL_COLOUR) {
//        std::shared_ptr<wxColourDialog> picker = std::shared_ptr<wxColourDialog>(new wxColourDialog(this));
//
//        if (picker->ShowModal() != wxID_OK) {
//            return;
//        }
//
//        wxColour colour = picker->GetColourData().GetColour();
//        _item_grid->SetCellValue(row, col, wxString::Format("%ul", colour.GetRGB()));
////        _item_grid->SetCellRenderer(row, col, new TableRenderer(_item_grid->GetCellValue(row, col)));
//
//        wxString name = _item_grid->GetCellValue(row, GRID_COL_TYPE);
//        for (auto & item : _config->class_list) {
//            if (item.name == name) {
//                item.colour = wxString::Format("%ul", colour.GetRGB());
//                break;
//            }
//        }
//
//        return;
//    } else if (col == GRID_COL_TYPE) {
//        event.Skip();
//    } else {
//        return;
//    }
}

//TableRenderer::TableRenderer(wxString const & col_str) : _col_str(col_str)
//{
//    /* EMPTY */
//}
//
//TableRenderer::~TableRenderer()
//{
//    /* EMPTY */
//}
//
//void TableRenderer::Draw(wxGrid & grid,
//          wxGridCellAttr & attr,
//          wxDC & dc,
//          wxRect const & rect,
//          int row,
//          int col,
//          bool isSelected)
//{
//    wxGridCellStringRenderer::Draw(grid, attr, dc ,rect, row, col, isSelected);
//
//    wxColour _colour = wxColour(wxAtol(_col_str));
//
//    dc.SetPen(_colour);
//    dc.SetBrush(_colour);
//    dc.DrawRectangle(rect);
//}
