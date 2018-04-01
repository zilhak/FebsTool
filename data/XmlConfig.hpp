#include <string>
#include <wx/wx.h>
#include <wx/filename.h>
#include <data/tinyxml2.h>
#include <vector>
#include <map>

#ifndef IMAGESEARCHER_XMLCONFIG_H
#define IMAGESEARCHER_XMLCONFIG_H

struct TargetObject
{

};

struct BoundingBox : TargetObject
{
    wxString type;
    int difficult = -1;

    int x1 = -1;
    int y1 = -1;
    int x2 = -1;
    int y2 = -1;

    BoundingBox() { /* EMPTY */ }
    BoundingBox(wxString type, int x1, int y1, int x2, int y2, int difficult) :
            type(type), x1(x1), y1(y1), x2(x2), y2(y2), difficult(difficult)
    { /* EMPTY */ }
};

struct Segmentation : TargetObject
{

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

enum class Action {

};

struct ConfigData {
    std::vector<wxString> class_list;
    std::map<int, Action> key_map;
    long box_colour_normal = -1;
    long box_colour_selected = -1;

    wxSize minimum_size = wxSize(-1, -1);
    wxSize maximum_size = wxSize(-1, -1);
};

using Document = tinyxml2::XMLDocument;
using Node = tinyxml2::XMLNode;
using Element = tinyxml2::XMLElement;

bool SaveConfig(ConfigData data);
ConfigData LoadConfig();

// ex) file "/usr/home/bogonets/example.jpg" -> loadFromXml("/usr/home/bogonets/example");
std::vector<BoundingBox> loadFromXml(wxString file_path_and_name_not_ext);
BoundingBox loadObject(Element * root);
ImageInfo loadXmlInfo (wxString image_file_not_ext);

Element * insertObject(Document * doc, BoundingBox check);
bool saveToXml (std::vector<BoundingBox> check_list,
                wxFileName file,
                ImageInfo info);

#endif //IMAGESEARCHER_XMLCONFIG_H
