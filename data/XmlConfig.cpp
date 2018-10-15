
#include <iostream>
#include <vector>
#include <data/XmlConfig.hpp>

constexpr static char const * const CONFIG_ROOT            = "config";
constexpr static char const * const CONFIG_OPTION             = "option";
constexpr static char const * const CONFIG_TYPES                  = "types";
constexpr static char const * const CONFIG_DEFAULT_TYPE               = "default_type";
constexpr static char const * const CONFIG_TYPE                      = "type";
constexpr static char const * const CONFIG_VIEW_LIMIT      = "limit";
constexpr static char const * const CONFIG_MAX_SIZE        = "max_size";
constexpr static char const * const CONFIG_MIN_SIZE        = "min_size";
constexpr static char const * const CONFIG_MAX_VIEW_WIDTH  = "max_view_width";
constexpr static char const * const CONFIG_MAX_VIEW_HEIGHT = "max_view_height";
constexpr static char const * const CONFIG_MIN_VIEW_WIDTH  = "min_view_width";
constexpr static char const * const CONFIG_MIN_VIEW_HEIGHT = "min_view_height";

namespace config {

bool SaveConfig(ConfigData data)
{
    Document document;
    Node * root = document.NewElement(CONFIG_ROOT);
    Element * option = document.NewElement(CONFIG_OPTION);
    Element * types = document.NewElement(CONFIG_TYPES);
    Element * default_type = document.NewElement(CONFIG_DEFAULT_TYPE);
    Element * type = document.NewElement(CONFIG_TYPE);
    Element * view_limit = document.NewElement(CONFIG_VIEW_LIMIT);
    Element * max_view_width = document.NewElement(CONFIG_MAX_VIEW_WIDTH);
    Element * max_view_height = document.NewElement(CONFIG_MAX_VIEW_HEIGHT);
    Element * min_view_width = document.NewElement(CONFIG_MIN_VIEW_WIDTH);
    Element * min_view_height = document.NewElement(CONFIG_MIN_VIEW_HEIGHT);

    document.InsertFirstChild(root);

    root->LinkEndChild(option);

    option->LinkEndChild(types);
    option->LinkEndChild(view_limit);

    types->LinkEndChild(default_type);
    for (auto const & name : data.class_list) {
        Element * type = document.NewElement(CONFIG_TYPE);
        type->SetText(name.ToStdString().c_str());
        types->LinkEndChild(type);
    }

    view_limit->LinkEndChild(min_view_width);
    view_limit->LinkEndChild(min_view_height);
    view_limit->LinkEndChild(max_view_width);
    view_limit->LinkEndChild(max_view_height);

    max_view_width->SetText(data.maximum_size.GetWidth());
    max_view_height->SetText(data.maximum_size.GetHeight());
    min_view_width->SetText(data.minimum_size.GetWidth());
    min_view_height->SetText(data.minimum_size.GetHeight());

    document.SaveFile("feps_config.xml");
    document.Clear();
    return true;
}

ConfigData LoadConfig()
{
    return ConfigData();
}

} // namespace config

std::vector<BoundingBox> loadFromXml(wxString image_file_not_ext)
{
    std::vector<BoundingBox> result;
    Document document;
    tinyxml2::XMLError err = document.LoadFile((image_file_not_ext + ".xml").c_str());
    if (err != tinyxml2::XML_NO_ERROR) {
        return result;
    }
    Node * root = document.FirstChildElement("annotation");
    for(Element * object = root->FirstChildElement("object"); object != NULL; object = object->NextSiblingElement("object")){
        result.push_back(loadObject(object));
    }

    return result;
}

BoundingBox loadObject(Element * object)
{
    Element * name = object->FirstChildElement("name");
    Element * comment = object->FirstChildElement("comment");
    Element * bndbox = object->FirstChildElement("bndbox");
    Element * difficult = object->FirstChildElement("difficult");

    Element * xmin = bndbox->FirstChildElement("xmin");
    Element * xmax = bndbox->FirstChildElement("xmax");
    Element * ymin = bndbox->FirstChildElement("ymin");
    Element * ymax = bndbox->FirstChildElement("ymax");

    wxString comment_str = "";

    if (comment != nullptr) {
        comment_str = comment->GetText();
    }

    return BoundingBox(name->GetText(),
                  std::stoi(xmin->GetText()),
                  std::stoi(ymin->GetText()),
                  std::stoi(xmax->GetText()),
                  std::stoi(ymax->GetText()),
                  std::stoi(difficult->GetText()),
                  comment_str);
}

ImageInfo loadXmlInfo(wxString image_file_not_ext)
{
    ImageInfo result(0, 0);

    tinyxml2::XMLDocument document;
    tinyxml2::XMLError err = document.LoadFile((image_file_not_ext + ".xml").c_str());
    if (err != tinyxml2::XML_NO_ERROR) {
        return result;
    }
    char * c;
    tinyxml2::StrPair * pair;
    std::cout << document.ParseDeep(c, pair) << std::endl;

    tinyxml2::XMLNode * root = document.FirstChildElement("annotation");

    tinyxml2::XMLElement * object = root->FirstChildElement("object");
    tinyxml2::XMLElement * name = object->FirstChildElement("name");
    tinyxml2::XMLElement * diff = object->FirstChildElement("difficult");

    tinyxml2::XMLElement * size = root->FirstChildElement("size");
    tinyxml2::XMLElement * depth = size->FirstChildElement("depth");
    tinyxml2::XMLElement * height = size->FirstChildElement("height");
    tinyxml2::XMLElement * width = size->FirstChildElement("height");

    result.image_width = std::stoi(width->GetText());
    result.image_height = std::stoi(height->GetText());
    result.depth = std::stoi(depth->GetText());

    return result;
}

Element * insertObject(Document * doc, BoundingBox rect)
{
    Element * object = doc->NewElement("object");
    Element * name = doc->NewElement("name");
    Element * bndbox = doc->NewElement("bndbox");
    Element * xmax = doc->NewElement("xmax");
    Element * xmin = doc->NewElement("xmin");
    Element * ymax = doc->NewElement("ymax");
    Element * ymin = doc->NewElement("ymin");
    Element * difficult = doc->NewElement("difficult");

    object->LinkEndChild(name);
    object->LinkEndChild(bndbox);
    object->LinkEndChild(difficult);

    bndbox->LinkEndChild(xmin);
    bndbox->LinkEndChild(ymin);
    bndbox->LinkEndChild(xmax);
    bndbox->LinkEndChild(ymax);

    name->LinkEndChild(doc->NewText(rect.type.c_str()));
    xmax->SetText(rect.x2);
    xmin->SetText(rect.x1);
    ymax->SetText(rect.y2);
    ymin->SetText(rect.y1);
    difficult->SetText(rect.difficult);

    return object;
}

bool saveToXml (std::vector<BoundingBox> check_list, wxFileName file, ImageInfo info)
{
    Document document;
    Node * root = document.NewElement("annotation");
    Element * filename = document.NewElement("filename");
    Element * folder = document.NewElement("folder");
    Element * size = document.NewElement("size");
    Element * depth = document.NewElement("depth");
    Element * height = document.NewElement("height");
    Element * width = document.NewElement("width");
    Element * source = document.NewElement("source");
    Element * annotation = document.NewElement("annotation");
    Element * database = document.NewElement("database");
    Element * image = document.NewElement("image");

    document.InsertFirstChild(root);

    root->LinkEndChild(filename);
    root->LinkEndChild(folder);
    for (int i = 0; i < check_list.size(); i++) {
        root->LinkEndChild(insertObject(&document, check_list.at(i)));
    }
    root->LinkEndChild(size);
    root->LinkEndChild(source);

    size->LinkEndChild(depth);
    size->LinkEndChild(height);
    size->LinkEndChild(width);

    source->LinkEndChild(annotation);
    source->LinkEndChild(database);
    source->LinkEndChild(image);

    filename->LinkEndChild(document.NewText(file.GetFullName().c_str()));
    folder->LinkEndChild(document.NewText("${FOLDER}"));
    depth->SetText(info.depth);
    height->SetText(info.image_height);
    width->SetText(info.image_width);
    annotation->LinkEndChild(document.NewText("Bogonet"));
    database->LinkEndChild(document.NewText("The bogonet image database"));
    image->LinkEndChild(document.NewText("SaveZone"));

    document.SaveFile((file.GetPath(true) + file.GetName() + ".xml").c_str());
    document.Clear();
    return true;
}