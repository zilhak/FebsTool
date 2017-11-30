#include "main.h"
#include "frame.hpp"

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
    Frame *frame = new Frame(wxT("Program"));
    //frame->Show(true);


    return true;
}