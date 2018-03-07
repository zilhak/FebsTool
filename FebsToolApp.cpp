//
// Created by bogonets on 18. 3. 7.
//

#include "FebsToolApp.hpp"

FebsToolApp::FebsToolApp()
{
    /* EMPTY */
}

FebsToolApp::~FebsToolApp()
{
    /* EMPTY */
}

bool FebsToolApp::OnInit()
{
    wxPNGHandler *png_handler = new wxPNGHandler();
    wxImage::AddHandler(png_handler);
}