#include "FileManagerFrame.hpp"

BEGIN_EVENT_TABLE(FileManagerFrame, wxDialog)
    EVT_BUTTON(BUTTON_ID::OPEN, FileManagerFrame::onOpen)
    EVT_CHAR_HOOK(FileManagerFrame::onKeyBoardEvent)
    EVT_MOUSE_EVENTS(FileManagerFrame::onMouseEvent)
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

        int row = _control_box->getRow();
        int col = _control_box->getCol();

        _iteration = 0;
        _screen->setPath(dir_name);
        _screen->setGrid(row, col);
        _screen->setImages(subvector(_iteration));
        _screen->teardown();
    }
}

void FileManagerFrame::onKeyBoardEvent(wxKeyEvent & event)
{
    std::cout << event.GetKeyCode() << std::endl;
    switch (event.GetKeyCode()) {
        case 65 :
            break;
        case 81 :
            prev();
            break;
        case 82 :
            next();
            break;
        default :
            break;
    }
}

void FileManagerFrame::onMouseEvent(wxMouseEvent & event)
{
    if (event.LeftDown()) {
        int index = _screen->getImageRectIndex(event.GetPosition());
        if (index != -1) {

        }
    }
}

void FileManagerFrame::prev()
{
    int row = _control_box->getRow();
    int col = _control_box->getCol();

    _iteration -= row * col;
    if (_iteration < 0) {
        _iteration = 0;
    }
    _screen->setImages(subvector(_iteration));
}

void FileManagerFrame::next()
{
    int row = _control_box->getRow();
    int col = _control_box->getCol();

    if (_iteration + row * col >= _file_list.size()) {
        return;
    }
    _iteration += row * col;

    _screen->setImages(subvector(_iteration));
}

void FileManagerFrame::toggleImage(int index)
{
    _selected_list[_iteration + index] = !_selected_list[_iteration + index];
}

std::vector<wxString> FileManagerFrame::subvector(int iter)
{
    int row = _control_box->getRow();
    int col = _control_box->getCol();

    if (iter >= _file_list.size()) {
        return std::vector<wxString>();
    } else if (iter + row * col >= _file_list.size()) {
        return std::vector<wxString>(_file_list.begin() + iter, _file_list.end());
    } else {
        return std::vector<wxString>(_file_list.begin() + iter, _file_list.begin() + iter + row * col);
    }
}