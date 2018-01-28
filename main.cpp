#include "main.h"
#include "ui/frame.hpp"
#include <wx/wx.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    Frame *frame = new Frame(wxT("Program"));
    frame->Show(true);

    return true;
}