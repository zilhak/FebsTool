#include <wx/wx.h>
#include <wx/dir.h>
#include "tinyxml2.h"

class Frame : public wxFrame
{
public:
    wxDECLARE_EVENT_TABLE();

public:
    Frame(const wxString & title);
    ~Frame();

private:
    void initOpen();
};

bool saveToXml (int x1, int y1, int x2, int y2, std::string filepath, std::string file_name, int height, int width, int diff);