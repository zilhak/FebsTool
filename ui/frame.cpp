
#include "frame.hpp"
#include <iostream>

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_BUTTON(ID::BUTTON_OPEN, Frame::onOpenButton)
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
    initializeToolBar(v_sizer);
    initializeImageViewer(v_sizer);

    SetSizer(v_sizer);

    Refresh();
}

void Frame::initializeStyle()
{
    SetMinSize(wxSize(400, 300));
}

void Frame::initializeToolBar(wxBoxSizer * sizer)
{
    _tool_bar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, 40));
    wxBoxSizer * h_sizer = new wxBoxSizer(wxHORIZONTAL);
    _open_button = new wxButton(_tool_bar, BUTTON_OPEN, wxT("Open"));
    _close_button = new wxButton(_tool_bar, BUTTON_CLOSE, wxT("Close"));

    h_sizer->Add(_open_button, 0, wxALIGN_CENTER_VERTICAL);
    h_sizer->Add(_close_button, 0, wxALIGN_CENTER_VERTICAL);

    _tool_bar->SetSizer(h_sizer);

    sizer->Add(_tool_bar, 0, wxEXPAND);
}

void Frame::initializeImageViewer(wxBoxSizer * sizer)
{
    _Image_viewer = new wxPanel(this, wxID_ANY);
    _Image_viewer->SetBackgroundColour(wxColour(0xFFFFFF));

    sizer->Add(_Image_viewer, 1, wxEXPAND);
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

void Frame::onOpenButton(wxCommandEvent & event)
{
    wxFileDialog * find = new wxFileDialog(this,
                                           _("Open jpg file"),
                                           "",
                                           "",
                                           "JPG files (*.jpg;*.jpeg)| *.jpg;*.jpeg");
    if (find->ShowModal() == wxID_CANCEL) {
        return;
    }

    _dir = new wxDir(find->GetDirectory());
    _dir->GetFirst(&_current_file, "*.jpg");
    while (_dir->GetNext(&_current_file)) {
        std::cout << _current_file << std::endl;
    }

    wxString _current_path = _dir->GetName()+"/"+_current_file;

}

bool Frame::saveToXml (int x1, int y1, int x2, int y2, std::string filepath, std::string file_name, int image_height, int image_width, int diff)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLDeclaration * dec = document.NewDeclaration();
    tinyxml2::XMLNode * root = document.NewElement("annotation");
    tinyxml2::XMLElement * filename = document.NewElement("filename");
    tinyxml2::XMLElement * folder = document.NewElement("folder");
    tinyxml2::XMLElement * object = document.NewElement("object");
    tinyxml2::XMLElement * name = document.NewElement("name");
    tinyxml2::XMLElement * bndbox = document.NewElement("bndbox");
    tinyxml2::XMLElement * xmax = document.NewElement("xmax");
    tinyxml2::XMLElement * xmin = document.NewElement("xmin");
    tinyxml2::XMLElement * ymax = document.NewElement("ymax");
    tinyxml2::XMLElement * ymin = document.NewElement("ymin");
    tinyxml2::XMLElement * difficult = document.NewElement("difficult");
    tinyxml2::XMLElement * size = document.NewElement("size");
    tinyxml2::XMLElement * depth = document.NewElement("depth");
    tinyxml2::XMLElement * height = document.NewElement("height");
    tinyxml2::XMLElement * width = document.NewElement("width");
    tinyxml2::XMLElement * source = document.NewElement("source");
    tinyxml2::XMLElement * annotation = document.NewElement("annotation");
    tinyxml2::XMLElement * database = document.NewElement("database");
    tinyxml2::XMLElement * image = document.NewElement("image");

    document.InsertFirstChild(root);
    root->LinkEndChild(filename);
    root->LinkEndChild(folder);
    root->LinkEndChild(object);
    root->LinkEndChild(size);
    root->LinkEndChild(source);

    object->LinkEndChild(name);
    object->LinkEndChild(bndbox);
    object->LinkEndChild(difficult);

    size->LinkEndChild(depth);
    size->LinkEndChild(height);
    size->LinkEndChild(width);

    source->LinkEndChild(annotation);
    source->LinkEndChild(database);
    source->LinkEndChild(image);

    bndbox->LinkEndChild(xmin);
    bndbox->LinkEndChild(ymin);
    bndbox->LinkEndChild(xmax);
    bndbox->LinkEndChild(ymax);

    filename->LinkEndChild(document.NewText(file_name.c_str()));
    folder->LinkEndChild(document.NewText("${FOLDER}"));
    name->LinkEndChild(document.NewText("car"));
    xmax->SetText(x2);
    xmin->SetText(x1);
    ymax->SetText(y2);
    ymin->SetText(y1);
    difficult->SetText(diff);
    depth->LinkEndChild(document.NewText("3"));
    height->SetText(image_height);
    width->SetText(image_width);
    annotation->LinkEndChild(document.NewText("Bogonet"));
    database->LinkEndChild(document.NewText("The bogonet image database"));
    image->LinkEndChild(document.NewText("SaveZone"));

    document.SaveFile((filepath + "/" + file_name.substr(0, file_name.length() - 4) + ".xml").c_str());
    document.Clear();
    return true;
}

