#include <ui/segmentation/SegmentationFrame.hpp>

SegmentationFrame::SegmentationFrame(wxString const & title) : wxDialog(NULL, wxID_ANY, title)
{
    initialize();
}

SegmentationFrame::~SegmentationFrame()
{

}

void SegmentationFrame::initialize()
{
    initializeStyle();
}

void SegmentationFrame::initializeStyle()
{
    SetMinSize(wxSize(400, 300));
    SetSize(wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
                   wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)));
}