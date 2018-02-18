#include <string>
#include <wx/filename.h>
#include "data/tinyxml2.h"
#include <vector>

#ifndef IMAGESEARCHER_XMLCONFIG_H
#define IMAGESEARCHER_XMLCONFIG_H

struct cyRect {
    wxString type;
    int difficult = -1;

    int x1 = -1;
    int y1 = -1;
    int x2 = -1;
    int y2 = -1;

    cyRect() { /* EMPTY */ }
    cyRect(wxString type, int x1, int y1, int x2, int y2, int difficult) :
            type(type), x1(x1), y1(y1), x2(x2), y2(y2), difficult(difficult)
    { /* EMPTY */ }
};

struct ImageInfo {
    int image_height;
    int image_width;
    int depth = 3;

    ImageInfo(int image_height,
              int image_width,
              int depth = 3) : image_height(image_height),
                              image_width(image_width),
                              depth(depth)
    { /* EMPTY */ }
};

struct ConfigData {

};

using Document = tinyxml2::XMLDocument;
using Node = tinyxml2::XMLNode;
using Element = tinyxml2::XMLElement;

bool SaveConfig(ConfigData data);
ConfigData LoadConfig();

// ex) file "/usr/home/bogonets/example.jpg" -> loadFromXml("/usr/home/bogonets/example");
std::vector<cyRect> loadFromXml(wxString file_path_and_name_not_ext);
cyRect loadObject(Element * root);
ImageInfo loadXmlInfo (wxString image_file_not_ext);

Element * insertObject(Document * doc, cyRect check);
bool saveToXml (std::vector<cyRect> check_list,
                wxFileName file,
                ImageInfo info);

#endif //IMAGESEARCHER_XMLCONFIG_H
