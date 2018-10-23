#include "FileManagerFrame.hpp"

BEGIN_EVENT_TABLE(FileManagerFrame, wxDialog)
    EVT_BUTTON(BUTTON_ID::OPEN, FileManagerFrame::onOpen)
END_EVENT_TABLE()

FileManagerFrame::FileManagerFrame(wxString const & title) :
    wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    initialize();
}

FileManagerFrame::~FileManagerFrame()
{
    /* EMPTY */
}

void FileManagerFrame::initialize()
{
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);

    h_sizer->Add(v_sizer, 0, wxEXPAND);

    SetMinSize(wxSize(800, 600));
    SetSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
                   wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));

    initializeData();
    initializeOpen(v_sizer);
    initializeLeftPanel(v_sizer);
    initializeRightPanel(h_sizer);

    SetSizer(h_sizer);
}

void FileManagerFrame::initializeData()
{
    _ext_list.emplace_back("jpg");
}

void FileManagerFrame::initializeOpen(wxSizer * sizer)
{
    wxPanel * open_panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer * panel_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton * open_button = new wxButton(open_panel, BUTTON_ID::OPEN, "Open");

    panel_sizer->Add(open_button, 1, wxEXPAND | wxALL, 5);

    open_panel->SetSizer(panel_sizer);

    sizer->Add(open_panel, 0, wxEXPAND);
}

void FileManagerFrame::initializeLeftPanel(wxSizer * sizer)
{
    _control_box = new ManagerControlBox(this, wxID_ANY);

    sizer->Add(_control_box, 1, wxEXPAND);
}

void FileManagerFrame::initializeRightPanel(wxSizer * sizer)
{
    _screen = new ManagerPanel(this, wxID_ANY);

    sizer->Add(_screen, 1, wxEXPAND);
}

void FileManagerFrame::onOpen(wxCommandEvent & event)
{
    const wxString dir_name = wxDirSelector("Choose Folder");

    if (!dir_name.empty()) {
        _file_list.clear();
        _selected_list.clear();
        _deleted_list.clear();
        _deleted_files = std::stack<int>();

        wxDir dir(dir_name);
        wxString file;

        dir.GetFirst(&file);
        wxFileName file_name(dir_name + "/" + file);

        do {
            file_name.Assign(dir_name + "/" + file);
            for (auto const & ext : _ext_list) {
                if (file_name.GetExt() == ext) {
                    _file_list.push_back(file);
                    break;
                }
            }
        } while (dir.GetNext(&file));

        std::sort(_file_list.begin(), _file_list.end());

        _screen->teardown();
    }

}
