#ifndef IMAGESEARCHER_FEBSTOOLAPP_HPP
#define IMAGESEARCHER_FEBSTOOLAPP_HPP

#include <wx/wx.h>

class FebsToolApp : public wxApp
{
public:
    FebsToolApp();
    ~FebsToolApp();

public:
    virtual bool OnInit();
    virtual int OnRun();
};


#endif //IMAGESEARCHER_FEBSTOOLAPP_HPP
