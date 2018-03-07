#include <wx/wx.h>
#include "FebsToolApp.hpp"

#define __GXX_ABI_VERSION 1002

wxIMPLEMENT_WX_THEME_SUPPORT;
IMPLEMENT_APP_NO_MAIN(FebsToolApp)

int main(int argc, char ** argv)
{
    wxDISABLE_DEBUG_SUPPORT();
    return wxEntry(argc, argv);
}