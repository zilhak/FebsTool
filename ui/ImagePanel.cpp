#include "ImagePanel.hpp"

wxBEGIN_EVENT_TABLE(ImagePanel, wxPanel)
    EVT_PAINT(ImagePanel::onPaint)
    EVT_MOUSE_EVENTS(ImagePanel::onMouse)
wxEND_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow *parent, wxWindowID id)
{
    /* EMPTY */
}

ImagePanel::~ImagePanel()
{
    /* EMPTY */
}

void ImagePanel::setBackgroundImage()
{

}

void ImagePanel::onPaint(wxPaintEvent & event)
{

}

void ImagePanel::onMouse(wxMouseEvent & event)
{

}