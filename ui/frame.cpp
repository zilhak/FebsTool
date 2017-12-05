
#include "frame.hpp"
#include <iostream>

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_BUTTON(ID::BUTTON_OPEN, Frame::onOpenButton)
    EVT_CHAR_HOOK(Frame::onKeyboardEvent)
    EVT_MOUSE_EVENTS(Frame::onMouseEvent)
    EVT_COMBOBOX(ID::COMBO_SCALE, Frame::onScaleComboBox)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, Frame::onListDoubleClick)
wxEND_EVENT_TABLE()

Frame::Frame(const wxString & title) : wxFrame(NULL, wxID_ANY, title)
{
    initialize();
}

Frame::~Frame()
{
    /*EMPTY*/
}

void Frame::initialize()
{
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);
    initializeStyle();
    initializeSetting();
    initializeToolBar(v_sizer);
    initializeImageViewer(v_sizer);

    SetSizer(v_sizer);

    Refresh();
}

void Frame::initializeStyle()
{
    SetMinSize(wxSize(400, 300));
    SetSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
                   wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));
}

void Frame::initializeSetting()
{
    _image_extension.insert(wxT("jpg"));
    _image_extension.insert(wxT("jpeg"));
    _image_extension.insert(wxT("png"));
}

void Frame::initializeToolBar(wxBoxSizer * sizer)
{
    _tool_bar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, 40));
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);
    _open_button = new wxButton(_tool_bar, BUTTON_OPEN, wxT("Open"));
    _close_button = new wxButton(_tool_bar, BUTTON_CLOSE, wxT("Close"));

    _scale_combobox = new wxComboBox(_tool_bar, COMBO_SCALE);
    for (int i = 10; i <= 600; i += 10) {
        _scale_combobox->Append(wxString::Format("%d", i));
    }

    h_sizer->Add(_open_button, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    h_sizer->Add(_close_button, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 15);
    h_sizer->Add(_scale_combobox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 20);

    _tool_bar->SetSizer(h_sizer);

    sizer->Add(_tool_bar, 0, wxEXPAND);
}

void Frame::initializeImageViewer(wxBoxSizer * sizer)
{
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    _file_list = new FileExplorer(this);
    _file_list->SetBackgroundColour(wxColour(0xCCCCCC));

    _image_viewer = new ImagePanel(this, wxID_ANY);
    _image_viewer->SetBackgroundColour(wxColour(0xFFFFFF));

    h_sizer->Add(_file_list, 0, wxEXPAND);
    h_sizer->Add(_image_viewer, 1, wxEXPAND);

    sizer->Add(h_sizer, 1, wxEXPAND);
}

bool calculateRate(int image_width, int image_height)
{
//    bool result = false;
//    int screen_x = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
//    int screen_y = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

//    if (image_width > screen_x) {
//        result_x = screen_x - 50;
//        x_rate = static_cast<double>(image_width) / static_cast<double>(screen_x - 50);
//        result = true;
//    } else {
//        result_x = image_width;
//    }
//    if (image_height > screen_y) {
//        result_y = screen_y - 100;
//        y_rate = static_cast<double>(image_height) / static_cast<double>(screen_y-100);
//        result = true;
//    } else {
//        result_y = image_height;
//    }

    return true;
}

bool Frame::fileExtCheck(wxString const extension)
{
    if (_image_extension.find(extension) != _image_extension.end()) {
        return true;
    } else {
        return false;
    }
}

void Frame::onOpenButton(wxCommandEvent & event)
{
    wxFileDialog * find = new wxFileDialog(this,
                                           _("Open jpg file"),
                                           "",
                                           "",
                                           "JPG files (*.jpg;*.jpeg)|*.jpg;*.jpeg");
    if (find->ShowModal() == wxID_CANCEL) {
        return;
    }

    addToListCtrl(find->GetDirectory());
    _dir = new wxDir(find->GetDirectory());
    _dir->GetFirst(&_current_file);

    do {
        if (_current_file == find->GetFilename()){
            break;
        }
    } while (_dir->GetNext(&_current_file));

    if (_current_file != find->GetFilename()){
        std::cout << "Cannot Found file!!!" << std::endl;
        std::cout << "Tip: if path name have korean, this error occur." << std::endl;
        return;
    }

    _file_list->highlightItem(_current_file);
    _image_viewer->setBackgroundImage(_dir->GetName(), _current_file);
}

void Frame::addToListCtrl(wxString const directory)
{
    _file_list->DeleteAllItems();
    _dir = new wxDir(directory);
    _dir->GetFirst(&_current_file);
    wxFileName file(directory + "/" + _current_file);

    long index = 0;
    do {
        file.Assign(directory + "/" + _current_file);
        if (fileExtCheck(file.GetExt())) {
            _file_list->InsertItem(index, _current_file);
            if (wxFileExists(directory + "/" + file.GetName() + ".xml")) {
                _file_list->SetItem(index, 1, ("O"));
            } else {
                _file_list->SetItem(index, 1, ("X"));
            }
            index++;
        }
    } while (_dir->GetNext(&_current_file));
}

void Frame::onListDoubleClick(wxListEvent & event)
{
    std::cout << event.GetIndex() << std::endl;
}

void Frame::onKeyboardEvent(wxKeyEvent &event)
{
    std::cout << event.GetKeyCode() << std::endl;
    if (_image_viewer->isReady()) {
        if (event.GetKeyCode() == 69) { // 'e'
            _file_list->xmlCheck(_current_file);
            _image_viewer->save();
            nextFile();
        } else if (event.GetKeyCode() == 81) { //'q'
            prevFile();
        } else if (event.GetKeyCode() == 87) { //'w'

        } else if (event.GetKeyCode() == 83) { //'s'

        } else if (event.GetKeyCode() == 65) { //'a'

        } else if (event.GetKeyCode() == 68) { //'d'

        } else if (event.GetKeyCode() == 82) { //'r'
            nextFile();
        }
    }
    SetTitle(_current_file);
}

void Frame::onMouseEvent(wxMouseEvent &event)
{

}

void Frame::onScaleComboBox(wxCommandEvent &event)
{
    if (_image_viewer->isReady()) {
        _image_viewer->changeScale(static_cast<double>(wxAtoi(_scale_combobox->GetValue())) / (double) 100);
        _image_viewer->setBackgroundImage(_dir->GetName(), _current_file);
    }
}

void Frame::prevFile()
{
    wxString search;
    wxString prevFile;
    _dir->GetFirst(&search);

    prevFile = _current_file;
    do {
        if (search.substr(search.length()-4, 4) == ".jpg" ||
            search.substr(search.length()-5, 5) == ".jpeg") {
            if (search == _current_file) {
                _temp_file = _current_file;
                _current_file = prevFile;

                _file_list->highlightItem(_current_file);
                _image_viewer->setBackgroundImage(_dir->GetName(), _current_file);
                _use_prev = true;
                return;
            } else {
                prevFile = search;
            }
        }
    } while (_dir->GetNext(&search));
}

void Frame::nextFile()
{
    if (_use_prev) {
        _current_file = _temp_file;
        _file_list->highlightItem(_current_file);
        _image_viewer->setBackgroundImage(_dir->GetName(), _temp_file);
        _use_prev = false;
        return;
    }
    while (_dir->GetNext(&_current_file)) {
        if (_current_file.substr(_current_file.length()-4, 4) == ".jpg" ||
            _current_file.substr(_current_file.length()-5, 5) == ".jpeg") {
            _file_list->highlightItem(_current_file);
            _image_viewer->setBackgroundImage(_dir->GetName(), _current_file);
            return;
        }
    }

    std::cout << "File Search End." << std::endl;
}