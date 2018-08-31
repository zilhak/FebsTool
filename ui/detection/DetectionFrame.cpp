
#include <ui/detection/DetectionFrame.hpp>
#include <iostream>

wxBEGIN_EVENT_TABLE(DetectionFrame, wxDialog)
    EVT_BUTTON(ID::BUTTON_OPEN, DetectionFrame::onOpenButton)
    EVT_CHAR_HOOK(DetectionFrame::onKeyboardEvent)
    EVT_MOUSE_EVENTS(DetectionFrame::onMouseEvent)
    EVT_COMBOBOX(ID::COMBO_SIZE, DetectionFrame::onSizeComboBox)
    EVT_COMBOBOX(ID::COMBO_TYPE, DetectionFrame::onTypeComboBox)
    EVT_COMBOBOX(ID::COMBO_SCALE, DetectionFrame::onScaleComboBox)
    EVT_COMBOBOX(ID::COMBO_DIFFICULT, DetectionFrame::onDifficultComboBox)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, DetectionFrame::onListDoubleClick)
//    EVT_SASH_DRAGGED(ID::SASH, DetectionFrame::onDragSash)
wxEND_EVENT_TABLE()

constexpr static char const * const BEST_NAME   = "BEST";
constexpr static char const * const MIDDLE_NAME = "MIDDLE";
constexpr static char const * const BAD_NAME    = "BAD";
constexpr static char const * const IR_NAME     = "IR_MODE";
constexpr static char const * const TRASH_NAME  = "TRASH";

DetectionFrame::DetectionFrame(const wxString & title) : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
                                                         _left_min_size(300, 600), _right_min_size(500, 600)
{
    _type_list.Clear();
    _type_list.push_back(wxT("car"));
    _type_list.push_back(wxT("truck"));
    _type_list.push_back(wxT("bus"));
    initialize();
}

DetectionFrame::~DetectionFrame()
{
    /*EMPTY*/
}

void DetectionFrame::initialize()
{
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    initializeStyle();
    initializeSetting();
    initializeToolBar(v_sizer);

    initializeLeftMenu(h_sizer);
    initializeImageViewer(h_sizer);

    v_sizer->Add(h_sizer, 1, wxEXPAND);
    SetSizer(v_sizer);

    Refresh();
}

void DetectionFrame::initializeStyle()
{
    SetMinSize(wxSize(800, 600));
    SetSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
                   wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));
}

void DetectionFrame::initializeSetting()
{
    _image_extension.insert(wxT("jpg"));
    _image_extension.insert(wxT("jpeg"));
    _image_extension.insert(wxT("png"));
}

void DetectionFrame::initializeToolBar(wxBoxSizer * sizer)
{
    _tool_bar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, 40));
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);
    _open_button = new wxButton(_tool_bar, ID::BUTTON_OPEN, wxT("Open"));
    _close_button = new wxButton(_tool_bar, ID::BUTTON_XML, wxT("XML view"));

    _size_combobox = new wxComboBox(_tool_bar, ID::COMBO_SIZE, wxT("100"));
    _size_combobox->SetEditable(false);
    for (int i = 10; i <= 600; i += 10) {
        _size_combobox->Append(wxString::Format("%d", i));
    }
    _size_combobox->Select(9);

    _type_combobox = new wxComboBox(_tool_bar, ID::COMBO_TYPE, wxT("car"));
    _type_combobox->SetEditable(false);

    for (auto & type_list : _type_list) {
        _type_combobox->Append(type_list);
    }
    _type_combobox->Select(0);
//    _type_combobox->Append(wxT("car"));
//    _type_combobox->Append(wxT("truck"));
//    _type_combobox->Append(wxT("bus"));
//    _type_combobox->Select(0);


    _scale_combobox = new wxComboBox(_tool_bar, ID::COMBO_SCALE, wxT("3"));
    _scale_combobox->SetEditable(false);
    _scale_combobox->Append(wxT("1"));
    _scale_combobox->Append(wxT("2"));
    _scale_combobox->Append(wxT("3"));

    _difficult_combobox = new wxComboBox(_tool_bar, ID::COMBO_DIFFICULT, wxT("0"));
    _difficult_combobox->Append(wxT("0"));
    _difficult_combobox->Append(wxT("1"));
    _difficult_combobox->Append(wxT("2"));

    _make_type_ctrl = new wxTextCtrl(_tool_bar, ID::MAKE_TYPE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    h_sizer->Add(_open_button, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    h_sizer->Add(_close_button, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 15);
    h_sizer->Add(new wxStaticText(_tool_bar, wxID_ANY, "Size:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    h_sizer->Add(_size_combobox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    h_sizer->Add(new wxStaticText(_tool_bar, wxID_ANY, "Type:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    h_sizer->Add(_type_combobox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    h_sizer->Add(new wxStaticText(_tool_bar, wxID_ANY, "Scale:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    h_sizer->Add(_scale_combobox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    h_sizer->Add(new wxStaticText(_tool_bar, wxID_ANY, "Difficult:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    h_sizer->Add(_difficult_combobox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    h_sizer->Add(new wxStaticText(_tool_bar, wxID_ANY, "Make Type:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    h_sizer->Add(_make_type_ctrl, 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 15);

    _tool_bar->SetSizer(h_sizer);

    sizer->Add(_tool_bar, 0, wxEXPAND);
}

void DetectionFrame::initializeLeftMenu(wxBoxSizer *h_sizer)
{
//    _sash_panel = new wxSashLayoutWindow(this, ID::SASH, wxDefaultPosition, wxDefaultSize, wxSW_3D | wxCLIP_CHILDREN);
//    _sash_panel->SetDefaultBorderSize(2);
//    _sash_panel->SetOrientation(wxLAYOUT_VERTICAL);
//    _sash_panel->SetAlignment(wxLAYOUT_LEFT);
//    _sash_panel->SetSashVisible(wxSASH_RIGHT, true);
    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);

    _file_list_viewer = new FileExplorer(this);
    _file_list_viewer->SetBackgroundColour(wxColour(0xCCCCCC));

    _image_info_box = new wxPanel(this, ID::FILE_EXPLORER);
    _image_info_box->SetBackgroundColour(0xBBBBBB);
    wxGridSizer * panel_sizer = new wxGridSizer(2, 3, 5);
    _image_info_box->SetSizer(panel_sizer);

    _info_image_name = new wxStaticText(_image_info_box, wxID_ANY, "");
    _info_image_size = new wxStaticText(_image_info_box, wxID_ANY, "");
    _info_mouse_x = new wxStaticText(_image_info_box, wxID_ANY, "");
    _info_mouse_y = new wxStaticText(_image_info_box, wxID_ANY, "");

    panel_sizer->Add(new wxStaticText(_image_info_box, wxID_ANY, "ImageName :"));
    panel_sizer->Add(_info_image_name);
    panel_sizer->Add(new wxStaticText(_image_info_box, wxID_ANY, "ImageSize :"));
    panel_sizer->Add(_info_image_size);
    panel_sizer->Add(new wxStaticText(_image_info_box, wxID_ANY, "MouseX :"));
    panel_sizer->Add(_info_mouse_x);
    panel_sizer->Add(new wxStaticText(_image_info_box, wxID_ANY, "MouseY :"));
    panel_sizer->Add(_info_mouse_y);

    v_sizer->Add(_image_info_box, 1, wxEXPAND);
    v_sizer->Add(_file_list_viewer, 5, wxEXPAND);

//    _sash_panel->SetMinSize(_left_min_size);
//    _sash_panel->SetSizer(v_sizer);

    h_sizer->Add(v_sizer, 0, wxEXPAND);
//    h_sizer->Add(_sash_panel, 1, wxEXPAND);
}

void DetectionFrame::initializeImageViewer(wxBoxSizer * sizer)
{
    _image_viewer = new ImagePanel(this, ID::IMAGE_VIEWER);
    _image_viewer->SetBackgroundColour(wxColour(0xFFFFFF));

    sizer->Add(_image_viewer, 1, wxEXPAND);
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

bool DetectionFrame::fileExtCheck(wxString const extension)
{
    if (_image_extension.find(extension) != _image_extension.end()) {
        return true;
    } else {
        return false;
    }
}

bool DetectionFrame::moveToDir(wxString const & dir_name)
{
    wxFileName file(_dir->GetName() + "/" + _current_file);
    wxString trashbin_path = file.GetPath(wxPATH_GET_SEPARATOR) + dir_name;

    if (!wxDirExists(trashbin_path)) {
        wxMkdir(trashbin_path);
    }
    wxString duplicate_number = "";
    int number = 0;
    while (!wxRenameFile(file.GetFullPath(),
                         trashbin_path + "/" + file.GetName() + duplicate_number + "." + file.GetExt(),
                         false)) {
        number++;
        duplicate_number = wxString::Format("(%d)", number);
    }
    if (wxFileExists(file.GetPath(wxPATH_GET_SEPARATOR) + file.GetName() + ".xml")) {
        wxRenameFile(file.GetPath(wxPATH_GET_SEPARATOR) + file.GetName() + ".xml",
                     trashbin_path + "/" + file.GetName() + duplicate_number + ".xml", true);
    }
    makeFileList(file.GetPath());
    refresh();
}

void DetectionFrame::openFindDialog(wxString const & title, std::vector<wxString> const & file_list, wxString const & img_kind)
{
    wxSharedPtr<FindDialog> find_dialog(new FindDialog(this, wxID_ANY, wxT("Find Text : ") + title, file_list, img_kind));
    find_dialog->Center();
    if (find_dialog->ShowModal() == EXIT_SUCCESS) {
        for (auto & file_name : _file_list) {
            if (file_name.Contains(find_dialog->getFindValue())) {
                _image_index = _file_list_viewer->FindItem(0, file_name);
                showImage(_file_list.at(_image_index));
                _file_list_viewer->EnsureVisible(_image_index + 20);
            }
        }
    }
}

void DetectionFrame::onOpenButton(wxCommandEvent & event)
{
    wxFileDialog * find = new wxFileDialog(this,
                                           _("Open jpg file"),
                                           "",
                                           "",
                                           "JPG files (*.jpg;*.jpeg)|*.jpg;*.jpeg");
    if (find->ShowModal() == wxID_CANCEL) {
        return;
    }

    wxString temp = wxString::FromUTF8(find->GetFilename());
    std::cout << temp << std::endl;

    makeFileList(find->GetDirectory());
    _dir_name = find->GetDirectory();
    for (auto & file : _file_list) {
        if (file == find->GetFilename()) {
            //loadXmlInfo(find->GetDirectory() + file);
            showImage(file);
            return;
        }
    }
    std::cout << "Cannot Found file!!!" << std::endl;
    std::cout << "Tip: if path name have korean, this error occur." << std::endl;
}

void DetectionFrame::makeFileList(wxString const directory)
{
    _file_list_viewer->DeleteAllItems();
    _file_list.clear();

    _dir = new wxDir(directory);

    _dir->GetFirst(&_current_file);
    wxFileName file(directory + "/" + _current_file);

    do {
        file.Assign(directory + "/" + _current_file);
        if (fileExtCheck(file.GetExt())) {
            _file_list.push_back(_current_file);
        }
    } while (_dir->GetNext((&_current_file)));

    std::sort(_file_list.begin(), _file_list.end());

    for (int index = 0; index < _file_list.size(); index++) {
        file.Assign(directory + "/" + _file_list.at(index));
       _file_list_viewer->InsertItem(index, wxString::FromUTF8(_file_list.at(index)));
       if (wxFileExists(directory + "/" + file.GetName() + ".xml")) {
           _file_list_viewer->SetItem(index, 1, ("O"));
       } else {
           _file_list_viewer->SetItem(index, 1, ("X"));
       }
    }
}

void DetectionFrame::onListDoubleClick(wxListEvent & event)
{
    showImage(event.GetLabel());
}

void DetectionFrame::onKeyboardEvent(wxKeyEvent & event)
{
    std::cout << event.GetKeyCode() << std::endl;
    if (_image_viewer->isReady()) {
        if (event.GetKeyCode() == WXK_SPACE) { // 'space'
            _image_viewer->setType(_type_combobox->GetValue());
            _image_viewer->setDiff(wxAtoi(_difficult_combobox->GetValue()));
            if (_image_viewer->save()) {
                _file_list_viewer->xmlCheck(_current_file);
            }
        } else if (event.GetKeyCode() == 396) { //'ctrl'
            prevFile();
        } else if (event.GetKeyCode() == 87) { //'w'

        } else if (event.GetKeyCode() == 83) { //'s'

        } else if (event.GetKeyCode() == 65) { //'a'

        } else if (event.GetKeyCode() == 68) { //'d'

        } else if (event.GetKeyCode() == WXK_ALT) { //'alt'
            nextFile();
        } else if (event.GetKeyCode() == WXK_DELETE) { //'delete'
            _image_viewer->deleteObject();
        } else if (event.GetKeyCode() == WXK_INSERT) { //'insert'
            _image_viewer->saveCropImage();
        } else if (event.GetKeyCode() >= 49 && event.GetKeyCode() <= 57) { //'1~9'
            _type_combobox->SetValue(*(_type_list.begin() + event.GetKeyCode() - 49));
            _difficult_combobox->SetValue(wxT("0"));
        } else if (event.GetKeyCode() == 390) { // 'numberpad-'
            _difficult_combobox->SetValue(wxT("0"));
        } else if (event.GetKeyCode() == 388) { // 'numberpad+'
            _difficult_combobox->SetValue(wxT("1"));
        } else if (event.GetKeyCode() == WXK_F1) { //'f1'
            moveToDir(BEST_NAME);
        } else if (event.GetKeyCode() == WXK_F2) { //'f2'
            moveToDir(MIDDLE_NAME);
        } else if (event.GetKeyCode() == WXK_F3) { //'f3'
            moveToDir(BAD_NAME);
        } else if (event.GetKeyCode() == WXK_F4) { //'f4'
            moveToDir(IR_NAME);
        } else if (event.GetKeyCode() == WXK_F5) { //'f5'
            moveToDir(TRASH_NAME);
        } else if (event.GetKeyCode() == WXK_TAB) { // `tab`
            _image_viewer->nextObject();
        } else if (event.GetKeyCode() == WXK_HOME) { // 'home'
            openFindDialog(wxT("key"), _file_list, wxT("key"));
        } else if (event.GetKeyCode() == WXK_END) {
            openFindDialog(wxT("r"), _file_list, wxT("r"));
        }
    }
    SetTitle(_current_file);
    event.Skip();
    if (event.GetKeyCode() == WXK_RETURN) {
        _type_list.push_back(_make_type_ctrl->GetValue());
        _type_combobox->Append(_make_type_ctrl->GetValue());
        Layout();
        wxMessageBox(wxT("Add Type : ") + _make_type_ctrl->GetValue(), wxT("Information"), wxOK | wxICON_INFORMATION);
    }
}

//void DetectionFrame::onDragSash(wxSashEvent & event)
//{
//    int left_previous_width = _sash_panel->GetSize().x;
//    int left_width = event.GetDragRect().width;
//    int changed_width = left_width - left_previous_width;
//    int right_width = _image_viewer->GetSize().x - changed_width;
//    int right_x = _image_viewer->GetPosition().x + changed_width;
//
//    if (left_width < _left_min_size.x) {
//        left_width = _left_min_size.x;
//        right_width = GetSize().x - left_width;
//        right_x = left_width;
//    }
//
//    if (right_width < _right_min_size.x) {
//        right_width = _right_min_size.x;
//        left_width = GetSize().x - right_width;
//        right_x = left_width;
//    }
//
//    _sash_panel->SetSize(wxSize(left_width, event.GetDragRect().height));
//    _image_viewer->SetPosition(wxPoint(right_x, _image_viewer->GetPosition().y));
//    _image_viewer->SetSize(wxSize(right_width, _right_min_size.y));
//
//    wxSizer * sizer = GetSizer();
//
//    wxSizerItem * left_item = sizer->GetItem(_sash_panel, true);
//    wxSizerItem * right_item = sizer->GetItem(_image_viewer, true);
//    left_item->SetInitSize(left_width, _left_min_size.y);
//    right_item->SetInitSize(right_width, _right_min_size.y);
//    Layout();
//}

void DetectionFrame::onMouseEvent(wxMouseEvent & event)
{
    int x = static_cast<int>(static_cast<double>(event.GetX() - 15) * 100.0 / wxAtof(_size_combobox->GetValue()));
    int y = static_cast<int>(static_cast<double>(event.GetY() - 15) * 100.0 / wxAtof(_size_combobox->GetValue()));

    _info_mouse_x->SetLabel(wxString::Format("%d", x));
    _info_mouse_y->SetLabel(wxString::Format("%d", y));

    if (event.GetId() == ID::IMAGE_VIEWER) {
        if (event.GetWheelRotation() < 0) {
            if (_size_combobox->GetSelection() > 1) {
                _size_combobox->Select(_size_combobox->GetSelection() - 1);
                wxCommandEvent evt;
                onSizeComboBox(evt);
            }
        } else if (event.GetWheelRotation() > 0) {
            if (_size_combobox->GetSelection() < _size_combobox->GetCount() - 1) {
                _size_combobox->Select(_size_combobox->GetSelection() + 1);
                wxCommandEvent evt;
                onSizeComboBox(evt);
            }
        }
    }
}

void DetectionFrame::onSizeComboBox(wxCommandEvent &event)
{
    _image_viewer->setSize(static_cast<double>(wxAtoi(_size_combobox->GetValue())) / (double) 100);
    if (_image_viewer->isReady()) {
        _image_viewer->setBackgroundImage(_dir->GetName(), _current_file);
    }
}

void DetectionFrame::onTypeComboBox(wxCommandEvent &event)
{
    _image_viewer->setType(_type_combobox->GetValue());
}

void DetectionFrame::onScaleComboBox(wxCommandEvent &event)
{
    _image_viewer->setDepth(wxAtoi(_scale_combobox->GetValue()));
}

void DetectionFrame::onDifficultComboBox(wxCommandEvent &event)
{
    _image_viewer->setDiff(wxAtoi(_difficult_combobox->GetValue()));
}

void DetectionFrame::prevFile()
{
    if (_image_index > 0) {
        _image_index--;
        showImage(_file_list.at(_image_index));
        if (_file_list_viewer->getHighlightedItemIndex() < _file_list_viewer->GetItemCount() - 5) {
            _file_list_viewer->ScrollLines(-1);
        }
    }
}

void DetectionFrame::nextFile()
{
    if (_image_index < _file_list.size() - 1) {
        _image_index++;
        showImage(_file_list.at(_image_index));
        if (_file_list_viewer->getHighlightedItemIndex() > 5) {
            _file_list_viewer->ScrollLines(1);
        }
    }
}

void DetectionFrame::refresh()
{
    if (_image_index < _file_list.size()) {
        showImage(_file_list.at(_image_index));
    } else if (--_image_index < _file_list.size()){
        showImage(_file_list.at(_image_index));
    }
}

void DetectionFrame::showImage(wxString const & file_name)
{
    SetTitle(file_name);

    _image_index = _file_list_viewer->highlightItem(file_name);
    _current_file = file_name;
    _image_viewer->setBackgroundImage(_dir->GetName(), _current_file);

    _info_image_name->SetLabel(wxString::Format("%s", _current_file));

    _info_image_size->SetLabel(wxString::Format("%d x %d", _image_viewer->getImageWidth(), _image_viewer->getImageHeight()));
}