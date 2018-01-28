#include <string>
#include <wx/filename.h>
#include "data/tinyxml2.h"

#ifndef IMAGESEARCHER_XMLCONFIG_H
#define IMAGESEARCHER_XMLCONFIG_H

struct cyRect {
    int x1 = -1;
    int y1 = -1;
    int x2 = -1;
    int y2 = -1;

    cyRect() { /* EMPTY */ }
    cyRect(int x1, int y1, int x2, int y2) :
            x1(x1), y1(y1), x2(x2), y2(y2)
    { /* EMPTY */ }
};

struct ImageInfo {
    int image_height;
    int image_width;
    wxString type;
    int depth = 3;
    int diff = 1;

    ImageInfo(int image_height,
              int image_width,
              wxString type,
              int depth = 3,
              int diff = 1) : image_height(image_height),
                              image_width(image_width),
                              type(type),
                              depth(depth),
                              diff(diff)
    { /* EMPTY */ }
};

// ex) file "/usr/home/bogonets/example.jpg" -> loadFromXml("/usr/home/bogonets/example");
cyRect loadFromXml(wxString file_path_and_name_not_ext);

bool saveToXml (cyRect check,
                wxFileName file,
                ImageInfo info);

#endif //IMAGESEARCHER_XMLCONFIG_H
