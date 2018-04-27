#include <string>
#include <wx/wx.h>
#include <wx/filename.h>
#include <data/tinyxml2.h>
#include <vector>
#include <map>

#ifndef IMAGESEARCHER_XMLCONFIG_H
#define IMAGESEARCHER_XMLCONFIG_H


using Document = tinyxml2::XMLDocument;
using Node = tinyxml2::XMLNode;
using Element = tinyxml2::XMLElement;

struct Object
{
    wxString type;
    int difficult = -1;

    wxString pose = "NULL";
    wxString truncated = "NULL";

    std::vector<wxPoint> point_list;
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

struct Name {
    wxString name;
    wxString colour;
};

enum class Action {

};

struct ConfigData {
    bool init = false;

    wxString default_class = "NULL";
    std::vector<Name> class_list;

    std::map<int, Action> key_map;

    int zoom_interval = 10;
    int zoom_min = 20;
    int zoom_max = 500;

    wxSize minimum_size = wxSize(-1, -1);
    wxSize maximum_size = wxSize(-1, -1);
};

namespace config {
bool saveConfig(ConfigData data);
ConfigData loadConfig();

void saveNameList(std::vector<Name> list);
std::vector<Name> loadNameList();
}// namespace config

// ex) file "/usr/home/bogonets/example.jpg" -> loadFromXml("/usr/home/bogonets/example");
std::vector<Object> loadFromXml(wxString file_path_and_name_not_ext);
Object loadObject(Element * root);
ImageInfo loadXmlInfo (wxString image_file_not_ext);

Element * insertObject(Document * doc, Object check);
bool saveToXml (std::vector<Object> check_list,
                wxFileName file,
                ImageInfo info);

#endif //IMAGESEARCHER_XMLCONFIG_H

