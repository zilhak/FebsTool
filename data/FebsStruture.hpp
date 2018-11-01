//
// Created by bogonets on 2018. 6. 21..
//

#ifndef FEPSTOOL_FEBSSTRUTURE_HPP
#define FEPSTOOL_FEBSSTRUTURE_HPP

#include <string>
#include <wx/wx.h>
#include <wx/filename.h>
#include <data/tinyxml2.h>
#include <vector>
#include <map>


using Document = tinyxml2::XMLDocument;
using Node = tinyxml2::XMLNode;
using Element = tinyxml2::XMLElement;

enum class ObjectType {
    CLASSIFY,
    DETECTION,
    SEGMENTATION
};

struct Object
{
    ObjectType type;
    wxString name;
    int difficult = -1;
    wxString comment;

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
    int line_width = 2;
};

enum class Action {

};

struct ConfigData {
    bool init = false;

    ObjectType type = ObjectType::SEGMENTATION;

    wxString default_class = "NULL";
    std::vector<Name> class_list;

    std::map<int, int> key_map;

    int zoom_interval = 10;
    int zoom_min = 20;
    int zoom_max = 500;

    wxSize minimum_size = wxSize(-1, -1);
    wxSize maximum_size = wxSize(-1, -1);

    ConfigData() {};
    ConfigData(bool init) : init(init) {};
};

#endif //FEPSTOOL_FEBSSTRUTURE_HPP
