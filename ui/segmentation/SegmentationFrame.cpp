#include <ui/segmentation/SegmentationFrame.hpp>

BEGIN_EVENT_TABLE(SegmentationFrame, wxDialog)
    EVT_COMMAND(wxID_ANY, FILE_OPEN_EVENT, SegmentationFrame::onFileOpen)
END_EVENT_TABLE();

SegmentationFrame::SegmentationFrame(wxString const & title) : wxDialog(NULL, wxID_ANY, title)
{
    initialize();
}

SegmentationFrame::~SegmentationFrame()
{
    /* EMPTY */
}

void SegmentationFrame::initialize()
{
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

}

void SegmentationFrame::initializeToolBar(wxBoxSizer * v_sizer)
{
    wxPanel * toolbar_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100,40));
    wxBoxSizer * comp_sizer = new wxBoxSizer(wxHORIZONTAL);

    _open = new FileOpenPanel(toolbar_panel, wxID_ANY);
    _toolbar = new SegmentationToolBar(toolbar_panel, wxID_ANY);

    comp_sizer->Add(_open, 1, wxALIGN_CENTER);
    comp_sizer->Add(_toolbar, 3, wxEXPAND | wxLEFT | wxALIGN_CENTER, 15);

    toolbar_panel->SetBackgroundColour(COLOUR_TOOLBAR);
    toolbar_panel->SetSizer(comp_sizer);

    v_sizer->Add(toolbar_panel, 0, wxEXPAND);
}

void SegmentationFrame::initializeLeftMenu(wxBoxSizer * h_sizer)
{
    wxBoxSizer * comp_sizer = new wxBoxSizer(wxVERTICAL);
    _infobox = new SegmentationInfoBox(this, wxID_ANY);
    _file_list = new FileExplorer(this);

    comp_sizer->Add(_infobox, 0);
    comp_sizer->Add(_file_list, 1, wxEXPAND);

    h_sizer->Add(comp_sizer, 1, wxEXPAND);
}

void SegmentationFrame::initializeImageViewer(wxBoxSizer * h_sizer)
{
    _image_panel = new ImagePanel(this, wxID_ANY);

    h_sizer->Add(_image_panel, 3, wxEXPAND);
}

void SegmentationFrame::onFileOpen(wxCommandEvent & event)
{
    std::cout << "okokokk" << std::endl;
    std::cout << "okokokk" << std::endl;
    std::cout << "okokokk" << std::endl;
    std::cout << "okokokk" << std::endl;
}