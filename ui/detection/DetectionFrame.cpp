
#include <ui/detection/DetectionFrame.hpp>
#include <data/ThemeData.hpp>
#include <iostream>

wxBEGIN_EVENT_TABLE(DetectionFrame, wxDialog)
    EVT_BUTTON(ButtonID::OPEN, DetectionFrame::onOpenButton)
    EVT_BUTTON(ButtonID::SETTING, DetectionFrame::onSettingButton)
    EVT_CHAR_HOOK(DetectionFrame::onKeyboardEvent)
    EVT_MOUSE_EVENTS(DetectionFrame::onMouseEvent)
    EVT_COMBOBOX(ID::COMBO_SIZE, DetectionFrame::onZoomBox)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, DetectionFrame::onSelectFile)
wxEND_EVENT_TABLE()

DetectionFrame::DetectionFrame(const wxString & title) : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
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

    _folder_name_list.emplace_back(wxT("folder1"));
    _folder_name_list.emplace_back(wxT("folder2"));
    _folder_name_list.emplace_back(wxT("folder3"));
    _folder_name_list.emplace_back(wxT("folder4"));
}

void DetectionFrame::initializeToolBar(wxBoxSizer * v_sizer)
{
    wxPanel * toolbar_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100,40));
    wxBoxSizer * comp_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel * open_panel = new wxPanel(toolbar_panel, wxID_ANY);
    _toolbar = new DetectionToolBar(toolbar_panel, wxID_ANY);

    open_panel->SetBackgroundColour(COLOUR_TOOLBAR);
    wxButton * open = new wxButton(open_panel, ButtonID::OPEN, wxT("Open"));
    wxButton * setting = new wxButton(open_panel, ButtonID::SETTING, wxT("Setting"));

    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    h_sizer->Add(open, 1, wxALL | wxALIGN_CENTER, 5);
    h_sizer->Add(setting, 1, wxALL | wxALIGN_CENTER, 5);

    open_panel->SetSizer(h_sizer);

    comp_sizer->Add(open_panel, 1, wxALIGN_CENTER_VERTICAL);
    comp_sizer->Add(_toolbar, 3, wxLEFT | wxALIGN_CENTER_VERTICAL, 15);

    toolbar_panel->SetBackgroundColour(COLOUR_TOOLBAR);
    toolbar_panel->SetSizer(comp_sizer);

    v_sizer->Add(toolbar_panel, 0, wxEXPAND);
}

void DetectionFrame::initializeLeftMenu(wxBoxSizer *h_sizer)
{
    wxBoxSizer * comp_sizer = new wxBoxSizer(wxVERTICAL);
    _infobox = new DetectionInfoBox(this, wxID_ANY);
    _file_list = new FileExplorer(this);

    comp_sizer->Add(_infobox, 0, wxEXPAND);
    comp_sizer->Add(_file_list, 1, wxEXPAND);

    h_sizer->Add(comp_sizer, 0, wxEXPAND);
}

void DetectionFrame::initializeImageViewer(wxBoxSizer * sizer)
{
    _image_panel = new ImagePanel(this, ID::IMAGE_VIEWER);
    _image_panel->SetBackgroundColour(wxColour(0xFFFFFF));

    sizer->Add(_image_panel, 1, wxEXPAND);
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

    wxFileName file_name(find->GetPath());
    wxString name = file_name.GetFullName();
    wxString file_ext = file_name.GetExt();
    _dir = file_name.GetPath();

    _file_list->openDir(_dir, { file_ext });
    if (_file_list->highlightItem(name) != -1) {
        _infobox->changeZoomBox(_image_panel->setBackgroundImage(file_name.GetFullPath(), _infobox->getZoom()));
        _infobox->setFolderName(file_name.GetDirs().back());
        _infobox->setFolderSize(_file_list->GetItemCount());
        _infobox->setImageIndex(static_cast<int>(_file_list->getHighlightedItemIndex()) + 1);
        _infobox->setImageName(name);
        _infobox->setImageSize(wxString::Format("%d x %d", _image_panel->getImageWidth(), _image_panel->getImageHeight()));
    }
}

void DetectionFrame::onSettingButton(wxCommandEvent & event)
{
    std::shared_ptr<SettingFrame> setting(new SettingFrame(this, wxID_ANY));

    setting->ShowModal();
}

void DetectionFrame::setImagePanel()
{
    _infobox->changeZoomBox(_image_panel->setBackgroundImage(_dir + "/" + _file_list->getHighlightedItem()));
    _image_panel->setSize(_infobox->getZoom());
    _infobox->setImageIndex(static_cast<int>(_file_list->getHighlightedItemIndex()) + 1);
    _infobox->setImageName(_file_list->getHighlightedItem());
    _infobox->setImageSize(wxString::Format("%d x %d", _image_panel->getImageWidth(), _image_panel->getImageHeight()));
}

void DetectionFrame::onSelectFile(wxListEvent & event)
{
    setImagePanel();
}

void DetectionFrame::onKeyboardEvent(wxKeyEvent & event)
{
    std::cout << "Inserted keycode : "<< event.GetKeyCode() << std::endl;
    if (_image_panel->isLoaded()) {
        if (event.GetKeyCode() == 69) { // 'e'
            _image_panel->setType(_toolbar->getType());
            _image_panel->setDiff(_toolbar->getDifficult());
            _image_panel->setDepth(_infobox->getImageScale());
            if (_image_panel->saveTempDetection()) {
                _file_list->xmlCheck();
            }
        } else if (event.GetKeyCode() == 81) { //'q'
            prev();
        } else if (event.GetKeyCode() == 87) { //'w'

        } else if (event.GetKeyCode() == 83) { //'s'

        } else if (event.GetKeyCode() == 65) { //'a'

        } else if (event.GetKeyCode() == 68) { //'d'

        } else if (event.GetKeyCode() == 82) { //'r'
            next();
        } else if (event.GetKeyCode() == 84) { //'t
            _image_panel->deleteObject();
        } else if (event.GetKeyCode() == 13) { //'enter'
            _image_panel->saveCropImage();
        } else if (event.GetKeyCode() == 49) { //'1'
            _toolbar->setType(wxT("car"));
        } else if (event.GetKeyCode() == 50) { //'2'
            _toolbar->setType(wxT("bus"));
        } else if (event.GetKeyCode() == 51) { //'3'
            _toolbar->setType(wxT("truck"));
        } else if (event.GetKeyCode() == 66) { //'b'
            _image_panel->showObjects();
        } else if (event.GetKeyCode() == 78) { //'n'
            _image_panel->showObjectName();
        } else if (event.GetKeyCode() == 72) { //'h'
            _image_panel->showCrossHair();
        } else if (event.GetKeyCode() == 90) { //'z'
            if (_folder_name_list.size() > 0) { moveFile(_folder_name_list[0]); }
        } else if (event.GetKeyCode() == 88) { //'x'
            if (_folder_name_list.size() > 1) { moveFile(_folder_name_list[1]); }
        } else if (event.GetKeyCode() == 67) { //'c'
            if (_folder_name_list.size() > 2) { moveFile(_folder_name_list[2]); }
        } else if (event.GetKeyCode() == 86) { //'v'
            if (_folder_name_list.size() > 3) { moveFile(_folder_name_list[3]); }
        } else if (event.GetKeyCode() == WXK_TAB) {
            _image_panel->nextObject();
        } else if (event.GetKeyCode() == 96) { // '`'
            _image_panel->previousObject();
        }
    }
    SetTitle(_file_list->getHighlightedItem());
}

void DetectionFrame::onMouseEvent(wxMouseEvent & event)
{
    int mouse_x = event.GetX();
    int mouse_y = event.GetY();

    if (event.m_shiftDown) {
        if (event.LeftDown() || event.LeftDClick()) {
            _image_panel->selectDetectionByClick();
        } else if (event.RightDown() || event.RightDClick()) {
            _image_panel->deleteDetectionByClick();
        }
        _image_panel->Refresh();
        return;
    }

    if (event.GetWheelRotation() > 0) {
        _infobox->zoomIn();
        _image_panel->setSize(_infobox->getZoom());
    } else if (event.GetWheelRotation() < 0) {
        _infobox->zoomOut();
        _image_panel->setSize(_infobox->getZoom());
    }

    _infobox->setMouseInfo(_image_panel->getActualMouse());

    if (event.LeftDown()) {
        Object new_object;
        new_object.type = ObjectType::DETECTION;
        new_object.difficult = _toolbar->getDifficult();

        _image_panel->startAddTempDetection(new_object);
    } else if (event.LeftUp()) {
        _image_panel->endAddTempDetection();
    } else if (event.RightDown()) {
        _previous_mouse_x = mouse_x;
        _previous_mouse_y = mouse_y;
    } else if (event.RightIsDown()) {
        _image_panel->moveView(_previous_mouse_x - mouse_x,
                               _previous_mouse_y - mouse_y);
        _previous_mouse_x = mouse_x;
        _previous_mouse_y = mouse_y;
    }

    _image_panel->Refresh();
}

void DetectionFrame::onZoomBox(wxCommandEvent &event)
{
    if (_image_panel->isLoaded()) {
        _image_panel->setSize(_infobox->getZoom());
    }
}

void DetectionFrame::prev()
{
    if (_file_list->prev()) {
        setImagePanel();
    }
}

void DetectionFrame::next()
{
    if (_file_list->next()) {
        setImagePanel();
    }
}

void DetectionFrame::moveFile(wxString const &folder_name)
{
    wxFileName file(_dir + "/" + _file_list->getHighlightedItem());
    wxString path = _dir + "/" + folder_name;

    if (!wxFileExists(file.GetFullPath())) {
        wxMessageBox(wxT("File not exists."));
        return;
    }

    if (!wxDirExists(path)) {
        wxMkdir(path);
    }
    wxString duplicate_number = "";
    int number = 0;
    while (!wxRenameFile(file.GetFullPath(),
                         path + "/" + file.GetName() + duplicate_number + "." + file.GetExt(),
                         false)) {
        number++;
        duplicate_number = wxString::Format("(%d)", number);
    }
    if (wxFileExists(file.GetPath(wxPATH_GET_SEPARATOR) + file.GetName() + ".xml")) {
        wxRenameFile(file.GetPath(wxPATH_GET_SEPARATOR) + file.GetName() + ".xml",
                path + "/" + file.GetName() + duplicate_number + ".xml", true);
    }

    _file_list->reopenDir();
    setImagePanel();
}
