
#include <ui/detection/DetectionFrame.hpp>
#include <data/ThemeData.hpp>
#include <iostream>

wxBEGIN_EVENT_TABLE(DetectionFrame, wxDialog)
    EVT_BUTTON(ButtonID::OPEN, DetectionFrame::onOpenButton)
    EVT_CHAR_HOOK(DetectionFrame::onKeyboardEvent)
    EVT_MOUSE_EVENTS(DetectionFrame::onMouseEvent)
    EVT_COMBOBOX(ID::COMBO_SIZE, DetectionFrame::onZoomBox)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, DetectionFrame::onSelectFile)

    EVT_COMBOBOX(ID::COMBO_TYPE, DetectionFrame::onTypeComboBox)
    EVT_COMBOBOX(ID::COMBO_SCALE, DetectionFrame::onScaleComboBox)
    EVT_COMBOBOX(ID::COMBO_DIFFICULT, DetectionFrame::onDifficultComboBox)
wxEND_EVENT_TABLE()

constexpr static char const * const TRASHBIN_NAME = "TrashCan";

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
        _infobox->setImageName(name);
        _infobox->setImageSize(wxString::Format("%d x %d", _image_panel->getImageWidth(), _image_panel->getImageHeight()));
    }
}

void DetectionFrame::onSelectFile(wxListEvent & event)
{

}

void DetectionFrame::onKeyboardEvent(wxKeyEvent & event)
{
    std::cout << "Inserted keycode : "<< event.GetKeyCode() << std::endl;
    if (_image_panel->isReady()) {
        if (event.GetKeyCode() == 69) { // 'e'
            _image_panel->setType(_type_combobox->GetValue());
            if (_image_panel->save()) {
                _file_list->xmlCheck(_current_file);
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
            _type_combobox->SetValue(wxT("car"));
        } else if (event.GetKeyCode() == 50) { //'2'
            _type_combobox->SetValue(wxT("bus"));
        } else if (event.GetKeyCode() == 51) { //'3'
            _type_combobox->SetValue(wxT("truck"));
        } else if (event.GetKeyCode() == WXK_DELETE) { //'delete'

        } else if (event.GetKeyCode() == WXK_PAGEUP) { //'pageup'

        } else if (event.GetKeyCode() == WXK_PAGEDOWN) {

        } else if (event.GetKeyCode() == WXK_TAB) {
            _image_panel->nextObject();
        }
    }
    SetTitle(_current_file);
}

void DetectionFrame::onMouseEvent(wxMouseEvent & event)
{
    if (event.GetWheelRotation() > 0) {
        _infobox->zoomIn();
        _image_panel->setSize(_infobox->getZoom());
    } else if (event.GetWheelRotation() < 0) {
        _infobox->zoomOut();
        _image_panel->setSize(_infobox->getZoom());
    }

    if (event.LeftDown()) {
        Object new_object;
        new_object.type = ObjectType::DETECTION;
        new_object.difficult = _toolbar->getDifficult();

        _image_panel->addObject(new_object);
    }
}

void DetectionFrame::onZoomBox(wxCommandEvent &event)
{
    _image_panel->setSize(static_cast<double>(wxAtoi(_size_combobox->GetValue())));
    if (_image_panel->isReady()) {
        _image_panel->setSize(_infobox->getZoom());
    }
}

void DetectionFrame::prev()
{

}

void DetectionFrame::next()
{

}

void DetectionFrame::moveFile(wxString const &folder_name)
{
    wxFileName file(_dir + "/" + _current_file);
    wxString path = _dir + "/" + folder_name;

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
}

void DetectionFrame::onTypeComboBox(wxCommandEvent &event)
{
    _image_panel->setType(_type_combobox->GetValue());
}

void DetectionFrame::onScaleComboBox(wxCommandEvent &event)
{
    _image_panel->setDepth(wxAtoi(_scale_combobox->GetValue()));
}

void DetectionFrame::onDifficultComboBox(wxCommandEvent &event)
{
    _image_panel->setDiff(wxAtoi(_difficult_combobox->GetValue()));
}
