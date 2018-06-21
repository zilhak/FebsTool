#include <string>
#include <wx/wx.h>
#include <wx/filename.h>
#include <data/tinyxml2.h>
#include <vector>
#include <map>

#ifndef IMAGESEARCHER_XMLCONFIG_H
#define IMAGESEARCHER_XMLCONFIG_H

#include <data/FebsStruture.hpp>

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

