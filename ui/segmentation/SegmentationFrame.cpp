#include <ui/segmentation/SegmentationFrame.hpp>

BEGIN_EVENT_TABLE(SegmentationFrame, wxDialog)
    EVT_BUTTON(ButtonID::OPEN, SegmentationFrame::onOpenButton)
    EVT_BUTTON(ButtonID::SETTING, SegmentationFrame::onSettingButton)
    EVT_BUTTON(ButtonID::HELP, SegmentationFrame::onHelpButton)
    EVT_MOUSE_EVENTS(SegmentationFrame::onMouse)
    EVT_CHAR_HOOK(SegmentationFrame::onKeyboard)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, SegmentationFrame::onSelectFile)
    EVT_COMBOBOX(SegmentationInfoBox::ID::ZOOM_BOX, SegmentationFrame::onZoom)
END_EVENT_TABLE();

SegmentationFrame::SegmentationFrame(wxString const & title) : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)
{
    initialize();
}

SegmentationFrame::~SegmentationFrame()
{
    /* EMPTY */
}

void SegmentationFrame::initialize()
{
    wxInitAllImageHandlers();

    wxBoxSizer * v_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    initializeToolBar(v_sizer);
    initializeLeftMenu(h_sizer);
    initializeImageViewer(h_sizer);

    initializeStyle();
    initializeSetting();

    v_sizer->Add(h_sizer, 1, wxEXPAND);
    SetSizer(v_sizer);
}

void SegmentationFrame::initializeStyle()
{
    SetMinSize(wxSize(400, 300));
    SetSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
                   wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));
}

void SegmentationFrame::initializeSetting()
{
    applySetting();
}

void SegmentationFrame::initializeToolBar(wxBoxSizer * v_sizer)
{
    wxPanel * toolbar_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100,40));
    wxBoxSizer * comp_sizer = new wxBoxSizer(wxHORIZONTAL);

    _open = new wxPanel(toolbar_panel, wxID_ANY);
    _toolbar = new SegmentationToolBar(toolbar_panel, wxID_ANY);

    _open->SetBackgroundColour(COLOUR_TOOLBAR);
    wxButton * open = new wxButton(_open, ButtonID::OPEN, wxT("Open"));
    wxButton * setting = new wxButton(_open, ButtonID::SETTING, wxT("Setting"));
    wxButton * help = new wxButton(_open, ButtonID::HELP, wxT("Help"));

    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);

    h_sizer->Add(open, 1, wxALL | wxALIGN_CENTER, 5);
    h_sizer->Add(setting, 1, wxALL | wxALIGN_CENTER, 5);
    h_sizer->Add(help, 1, wxALL | wxALIGN_CENTER, 5);

    _open->SetSizer(h_sizer);

    comp_sizer->Add(_open, 1, wxALIGN_CENTER_VERTICAL);
    comp_sizer->Add(_toolbar, 3, wxLEFT | wxALIGN_CENTER_VERTICAL, 15);

    toolbar_panel->SetBackgroundColour(COLOUR_TOOLBAR);
    toolbar_panel->SetSizer(comp_sizer);

    v_sizer->Add(toolbar_panel, 0, wxEXPAND);
}

void SegmentationFrame::initializeLeftMenu(wxBoxSizer * h_sizer)
{
    wxBoxSizer * comp_sizer = new wxBoxSizer(wxVERTICAL);
    _infobox = new SegmentationInfoBox(this, wxID_ANY);
    _file_list = new FileExplorer(this);

    comp_sizer->Add(_infobox, 0, wxEXPAND);
    comp_sizer->Add(_file_list, 1, wxEXPAND);

    h_sizer->Add(comp_sizer, 1, wxEXPAND);
}

void SegmentationFrame::initializeImageViewer(wxBoxSizer * h_sizer)
{
    _image_panel = new ImagePanel(this, wxID_ANY);

    h_sizer->Add(_image_panel, 3, wxEXPAND);
}

Object SegmentationFrame::createNewObject()
{
    Object new_object;
    new_object.pose = _toolbar->getPose();
    new_object.truncated = _toolbar->getTruncated();
    new_object.type = _toolbar->getName();
    new_object.difficult = _toolbar->getDifficult();

    return new_object;
}

void SegmentationFrame::setImagePanel()
{
    _image_panel->cancelAddObject();
    _infobox->changeZoomBox(_image_panel->setBackgroundImage(_dir + "/" + _file_list->getHighlightedItem()));
    _image_panel->setSize(_infobox->getZoom());
    _infobox->setImageName(_file_list->getHighlightedItem());
    _infobox->setImageSize(wxString::Format("%d x %d", _image_panel->getImageWidth(), _image_panel->getImageHeight()));
}

void SegmentationFrame::prev()
{
    if (_file_list->prev()) {
        setImagePanel();
    }
}

void SegmentationFrame::next()
{
    if (_file_list->next()) {
        setImagePanel();
    }
}

void SegmentationFrame::applySetting()
{
    ConfigData data = config::loadConfig();

    if (data.init) {
        Name init_name;
        std::vector<wxString> name_list = {wxT("GS-A"), wxT("GS-B"), wxT("GS-S"), wxT("MB"), wxT("�淮A"), wxT("�淮B"), wxT("��¥"), wxT("���ݼ�A"), wxT("���ݼ�B"), wxT("�߷�A"), wxT("�߷�B")};
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
}

void SegmentationFrame::onOpenButton(wxCommandEvent & event)
{
    wxFileDialog * find = new wxFileDialog(this,
                                           _("Open jpg file"),
                                           "",
                                           "",
                                           "JPG files (*.jpg;*.jpeg)|*.jpg;*.jpeg", wxFD_DEFAULT_STYLE, wxPoint(1,1));
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
        _infobox->setImageName(name);
        _infobox->setImageSize(wxString::Format("%d x %d", _image_panel->getImageWidth(), _image_panel->getImageHeight()));

        _status = STATUS::IDLE;
    }
}

void SegmentationFrame::onSettingButton(wxCommandEvent & event)
{
    SettingFrame * setting = new SettingFrame(this, wxID_ANY);

    if (setting->ShowModal() == wxID_CANCEL) {
        delete setting;
    }

    applySetting();
}

void SegmentationFrame::onHelpButton(wxCommandEvent & event)
{
   std::shared_ptr<HelpDialog> help_dialog = std::shared_ptr<HelpDialog>(new HelpDialog(this));
   
   help_dialog->ShowModal();
}

void SegmentationFrame::onMouse(wxMouseEvent & event)
{
    if (_status == STATUS::NOT_LOADED) {
        return;
    }
    int mouse_x = event.GetX();
    int mouse_y = event.GetY();

    _infobox->setMouseInfo(_image_panel->convertToActualLocation(mouse_x, mouse_y));
    wxRect rect = _image_panel->getView();
    _image_panel->setMousePoint(mouse_x, mouse_y);

    if (event.LeftDown() && _status == STATUS::IDLE)  {
        if (_image_panel->startAddObject(ImagePanel::ObjectKind::SEGMENTATION, createNewObject())) {
            _status = STATUS::DRAWING;
        }
    } else if (event.LeftDown() && _status == STATUS::DRAWING) {
        _image_panel->addPointToNewObject();
    } else if (event.GetWheelRotation() != 0) {
        if (event.GetWheelRotation() > 0) {
            _infobox->zoomIn();
        }
        else {
            _infobox->zoomOut();
        }
        _image_panel->setSize(_infobox->getZoom());
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

void SegmentationFrame::onKeyboard(wxKeyEvent & event)
{
    std::cout << (wxString::Format("%d", event.GetKeyCode())) << std::endl;
    if (_status == STATUS::NOT_LOADED) {
        return;
    } else {
        switch (event.GetKeyCode()) {
        case WXK_ALT:
            _image_panel->showObjectName();
            break;
        case WXK_CONTROL :
            _image_panel->showObjects();
            break;
        case 81: // q
            prev();
            _status = STATUS::IDLE;
            break;
        case 82: // r
            next();
            _status = STATUS::IDLE;
            break;
        case 84:
            if (_image_panel->deleteObject()) {
                _file_list->xmlCheck(false);
            }
            _status = STATUS::IDLE;
            break;
        }
    }
    
    if (_status == STATUS::IDLE) {
    switch (event.GetKeyCode()) {
        case WXK_TAB:
            _image_panel->nextObject();
            break;
        case '`':
            _image_panel->previousObject();
            break;
        case 70: // f
            if(_image_panel->unpackObject()) {
                _status = STATUS::DRAWING;
            }
            break;
        }
    } else if (_status == STATUS::DRAWING) {
        switch (event.GetKeyCode()) {
        case WXK_ESCAPE:
            _status = STATUS::IDLE;
            _image_panel->cancelAddObject();
            break;
        case 87 : //w
            _image_panel->pointUp();
            break;
        case 83 : //s
            _image_panel->pointDown();
            break;
        case 65 : //a
            _image_panel->pointLeft();
            break;
        case 68 : //d
            _image_panel->pointRight();
            break;
        case 69 : //e
            _file_list->xmlCheck();
            _image_panel->endAddObject();
            _status = STATUS::IDLE;
            break;
        case 70: // f
            if (!_image_panel->undo()) {
                _status = STATUS::IDLE;
            }
            break;
        }
        _image_panel->Refresh();
    }
}

void SegmentationFrame::onSelectFile(wxListEvent & event)
{
    setImagePanel();
}

void SegmentationFrame::onZoom(wxCommandEvent & event)
{
    if (_status == STATUS::NOT_LOADED) {
        return;
    }

    _image_panel->setSize(_infobox->getZoom());
}
