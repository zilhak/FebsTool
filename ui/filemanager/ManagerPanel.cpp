

#include <ui/filemanager/ManagerPanel.hpp>

BEGIN_EVENT_TABLE(ManagerPanel, wxPanel)
    EVT_PAINT(ManagerPanel::onPaint)
END_EVENT_TABLE()

ManagerPanel::ManagerPanel(wxWindow * parent, wxWindowID id) : wxPanel(parent, id)
{
    initialize();
}

ManagerPanel::~ManagerPanel()
{
    /* EMPTY */
}

void ManagerPanel::initialize()
{
    SetBackgroundColour(*wxWHITE);
}

void ManagerPanel::onMouse(wxMouseEvent & event)
{


    event.ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event.Skip();
}

void ManagerPanel::onPaint(wxPaintEvent & event)
{
    if (!_load) {
        return;
    }
    wxPaintDC dc(this);

    dc.SetPen(*wxRED);
    dc.DrawRectangle(100, 100, 100, 100);
}

bool ManagerPanel::teardown()
{
    _iter = 0;
    _load = true;

    Refresh();
}