
#include <ui/detection/DetectionFrame.hpp>
#include <data/ThemeData.hpp>
#include <iostream>
#include <opencv/cv.hpp>

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
    cv::Mat mat;
    std::cout << "test : " << mat.elemSize() << std::endl;
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

    applySetting();
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
    std::shared_ptr<SettingFrame> setting(new SettingFrame(this, wxID_ANY, ObjectType::DETECTION));

    setting->ShowModal();
    setting->save();

    applySetting();
}

void DetectionFrame::setImagePanel()
{
    _infobox->changeZoomBox(_image_panel->setBackgroundImage(_dir + "/" + _file_list->getHighlightedItem()));
    _image_panel->setSize(_infobox->getZoom());
    _infobox->setImageIndex(static_cast<int>(_file_list->getHighlightedItemIndex()) + 1);
    _infobox->setImageName(_file_list->getHighlightedItem());
    _infobox->setImageSize(wxString::Format("%d x %d", _image_panel->getImageWidth(), _image_panel->getImageHeight()));
}

void DetectionFrame::applySetting()
{
    ConfigData data = config::loadConfig();

    if (data.init) {
        Name init_name;
        std::vector<wxString> name_list = {wxT("car"), wxT("bus"), wxT("truck")};
        for (auto const & name : name_list) {
            init_name.name = name;
            init_name.colour = wxString::Format("%ul", 0x00FFFF);
            data.class_list.push_back(init_name);
        }
        data.default_class = name_list[0];
    }
    _image_panel->setNameList(data.class_list);
    _image_panel->setInitSize(data.maximum_size, data.minimum_size);
    _infobox->setZoomBox(data.zoom_min, data.zoom_max, data.zoom_interval);
    _toolbar->setNameBox(data.class_list, data.default_class);

    _image_panel->Refresh();
}

// Code List
//  0 ~ 9 : class change
// 10 ~ 19: move to folder
// 20  : confirm box
// 30  : next object
// 31  : previous object
// 32  : delete object
// 50  : nextfile
// 51  : prevfile
// 100 : deletefile
// 101 : cropfile
// 110 : crosshair on/off
// 111 : object name on/off
// 112 : box on/off
// 200 : undo


int DetectionFrame::getAction(int keycode)
{
    auto code = _key_map.find(keycode);

    if (code != _key_map.end()) {
        return code->second;
    }

    switch (keycode) {
        case 13 : return 101;// 'enter', crop image
        case 69 : return 20; // 'e', confirm box

        case 96 : return 30; // '`', next object;
        case 9  : return 31; //'tab', prev object;
        case 84 : return 32; // 't', delete object

        case 81 : return 51; // 'q', previous image
        case 82 : return 50; // 'r', next image
        //case ?? : return 100// delete file!!

        case 49 : return 0;  // '1', class change
        case 50 : return 1;  // '2', class change
        case 51 : return 2;  // '3', class change
        case 52 : return 3;  // '4', class change
        case 53 : return 4;  // '5', class change
        case 54 : return 5;  // '6', class change
        case 55 : return 6;  // '7', class change
        case 56 : return 7;  // '8', class change
        case 57 : return 8;  // '9', class change
        case 48 : return 9;  // '0', class change

        case 90 : return 10; // 'z', image move to folder
        case 88 : return 11; // 'x', image move to folder
        case 67 : return 12; // 'c', image move to folder
        case 86 : return 13; // 'v', image move to folder

        case 72 : return 110;// 'h', crosshair on/off
        case 78 : return 111;// 'n', class name on/off
        case 66 : return 112;// 'b', box on/off

        case 85 : return 200;// 'u', undo
        default : return -1;
    }
}

void DetectionFrame::onSelectFile(wxListEvent & event)
{
    setImagePanel();
}

void DetectionFrame::onKeyboardEvent(wxKeyEvent & event)
{
    std::cout << "Inserted keycode : "<< event.GetKeyCode() << std::endl;
    if (_image_panel->isLoaded()) {
        int action_code = getAction(event.GetKeyCode());
        switch (action_code) {
            case 20 : // confirm box
                _image_panel->setType(_toolbar->getType());
                _image_panel->setDiff(_toolbar->getDifficult());
                _image_panel->setDepth(_infobox->getImageScale());
                if (_image_panel->saveTempDetection()) {
                    _file_list->xmlCheck();
                }
                break;
            case 30: // select next object
                _image_panel->nextObject(); break;
            case 31: // select previous object
                _image_panel->previousObject(); break;
            case 32: // delete selected object
                _image_panel->deleteObject(); break;
            case 50 : // next image
                next(); break;
            case 51 : // previous image
                prev(); break;
            case 100 : // delete file
                break;
            case 101 : // save croped image
                _image_panel->saveCropImage(); break;
            case 110 : // crosshair on/off
                _image_panel->showCrossHair(); break;
            case 111 : // classname on/off
                _image_panel->showObjectName(); break;
            case 112 : // box on/off
                _image_panel->showObjects(); break;
            case 200 : // undo

            case 0 : case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: // set class name
                _toolbar->setType(action_code);
                break;
            case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: // move file
                if (_folder_name_list.size() > action_code - 10) { moveFile(_folder_name_list[action_code - 10]); }
                break;
            default:
                return;
        }
        SetTitle(_file_list->getHighlightedItem());
        _infobox->setObjectInfo(_image_panel->getSelectedDetection());
    }
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
            _image_panel->save();
            _image_panel->setStatus(ImagePanel::STATUS::IDLE);
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
        _infobox->setObjectInfo(_image_panel->getSelectedDetection());
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
    } else if (event.LeftIsDown()) {
        _infobox->setObjectInfo(_image_panel->getSelectedDetection());
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

    _file_list->deleteFile(file.GetFullName());
    _infobox->decreaseFolderSize();
    setImagePanel();
}
