
#include <iostream>
#include <vector>
#include <data/XmlConfig.hpp>

constexpr static char const * const CONFIG_FILE = "OC_Config.xml";

constexpr static char const * const CONFIG_ROOT            = "config";
constexpr static char const * const CONFIG_OPTION             = "option";
constexpr static char const * const CONFIG_TYPES                  = "types";
constexpr static char const * const CONFIG_DEFAULT_TYPE               = "default_type";
constexpr static char const * const CONFIG_TYPE                       = "name";
constexpr static char const * const CONFIG_TYPE_NAME                      = "name";
constexpr static char const * const CONFIG_TYPE_COLOUR                    = "colour";
constexpr static char const * const CONFIG_VIEW_LIMIT      = "limit";
constexpr static char const * const CONFIG_MAX_SIZE        = "max_size";
constexpr static char const * const CONFIG_MIN_SIZE        = "min_size";
constexpr static char const * const CONFIG_MAX_VIEW_WIDTH  = "max_view_width";
constexpr static char const * const CONFIG_MAX_VIEW_HEIGHT = "max_view_height";
constexpr static char const * const CONFIG_MIN_VIEW_WIDTH  = "min_view_width";
constexpr static char const * const CONFIG_MIN_VIEW_HEIGHT = "min_view_height";

constexpr static char const * const CONFIG_ZOOM            = "zoom";
constexpr static char const * const CONFIG_ZOOM_MAX        = "zoom_max";
constexpr static char const * const CONFIG_ZOOM_MIN        = "zoom_min";
constexpr static char const * const CONFIG_ZOOM_INTERVAL   = "zoom_interval";

namespace config {

bool saveConfig(ConfigData data)
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
    Element * zoom = document.NewElement(CONFIG_ZOOM);
    Element * zoom_min = document.NewElement(CONFIG_ZOOM_MIN);
    Element * zoom_max = document.NewElement(CONFIG_ZOOM_MAX);
    Element * zoom_interval = document.NewElement(CONFIG_ZOOM_INTERVAL);

    document.InsertFirstChild(root);

    root->LinkEndChild(option);

    option->LinkEndChild(types);
    option->LinkEndChild(view_limit);
    option->LinkEndChild(zoom);

    types->LinkEndChild(default_type);
    for (auto const & name : data.class_list) {
        Element * type = document.NewElement(CONFIG_TYPE);
        Element * type_name = document.NewElement(CONFIG_TYPE_NAME);
        Element * type_colour = document.NewElement(CONFIG_TYPE_COLOUR);

        type->LinkEndChild(type_name);
        type->LinkEndChild(type_colour);

        type_name->SetText(name.name.ToStdString().c_str());
        type_colour->SetText(name.colour.ToStdString().c_str());

        types->LinkEndChild(type);
    }

    view_limit->LinkEndChild(min_view_width);
    view_limit->LinkEndChild(min_view_height);
    view_limit->LinkEndChild(max_view_width);
    view_limit->LinkEndChild(max_view_height);

    zoom->LinkEndChild(zoom_min);
    zoom->LinkEndChild(zoom_max);
    zoom->LinkEndChild(zoom_interval);

    if (data.default_class == "NULL" && data.class_list.size() > 0) {
        data.default_class = data.class_list[0].name;
    }

    default_type->SetText(data.default_class.ToStdString().c_str());
    max_view_width->SetText(data.maximum_size.GetWidth());
    max_view_height->SetText(data.maximum_size.GetHeight());
    min_view_width->SetText(data.minimum_size.GetWidth());
    min_view_height->SetText(data.minimum_size.GetHeight());

    zoom_min->SetText(data.zoom_min);
    zoom_max->SetText(data.zoom_max);
    zoom_interval->SetText(data.zoom_interval);

    document.SaveFile(CONFIG_FILE);
    document.Clear();
    return true;
}

ConfigData loadConfig()
{
    ConfigData result;

    Document document;
    tinyxml2::XMLError err = document.LoadFile(CONFIG_FILE);
    if (err != tinyxml2::XML_NO_ERROR) {
        result.init = true;
        return result;
    }

    Node * root = document.FirstChildElement(CONFIG_ROOT);

    Element * option = root->FirstChildElement(CONFIG_OPTION);
    Element * types = option->FirstChildElement(CONFIG_TYPES);
    Element * limit = option->FirstChildElement(CONFIG_VIEW_LIMIT);
    Element * zoom = option->FirstChildElement(CONFIG_ZOOM);

    Element * default_type = types->FirstChildElement(CONFIG_DEFAULT_TYPE);

    for (Element * type = types->FirstChildElement(CONFIG_TYPE); type != NULL; type = type->NextSiblingElement(CONFIG_TYPE)) {
        Element * type_name = type->FirstChildElement(CONFIG_TYPE_NAME);
        Element * type_colour = type->FirstChildElement(CONFIG_TYPE_COLOUR);

        Name name;
        name.name = type_name->GetText();
        name.colour = type_colour->GetText();

        result.class_list.push_back(name);
    }

    Element * min_width = limit->FirstChildElement(CONFIG_MIN_VIEW_WIDTH);
    Element * min_height = limit->FirstChildElement(CONFIG_MIN_VIEW_HEIGHT);
    Element * max_width = limit->FirstChildElement(CONFIG_MAX_VIEW_WIDTH);
    Element * max_height = limit->FirstChildElement(CONFIG_MAX_VIEW_HEIGHT);

    result.default_class = default_type->GetText();
    result.minimum_size = wxSize(std::stoi(min_width->GetText()), std::stoi(min_height->GetText()));
    result.maximum_size = wxSize(std::stoi(max_width->GetText()), std::stoi(max_height->GetText()));

    Element * zoom_min = zoom->FirstChildElement(CONFIG_ZOOM_MIN);
    Element * zoom_max = zoom->FirstChildElement(CONFIG_ZOOM_MAX);
    Element * zoom_interval = zoom->FirstChildElement(CONFIG_ZOOM_INTERVAL);

    result.zoom_min = std::stoi(zoom_min->GetText());
    result.zoom_max = std::stoi(zoom_max->GetText());
    result.zoom_interval = std::stoi(zoom_interval->GetText());

    return result;
}

void saveNameList(std::vector<Name> list)
{
}

std::vector<Name> loadNameList()
{


    return std::vector<Name>();
}

} // namespace config

std::vector<Object> loadFromXml(wxString image_file_not_ext)
{
    std::vector<Object> result;
    Document document;
    tinyxml2::XMLError err = document.LoadFile((image_file_not_ext + ".xml").c_str());
    if (err == tinyxml2::XML_NO_ERROR) {
        Node * root = document.FirstChildElement("annotation");
        for(Element * object = root->FirstChildElement("object"); object != NULL; object = object->NextSiblingElement("object")){
            result.push_back(loadObject(object));
        }
        return result;
    }

    return result;
}

Object loadObject(Element * object)
{
    Object result;

    Element * bndbox = object->FirstChildElement("bndbox");
    if (bndbox == nullptr) {
        result.type = ObjectType::SEGMENTATION;

        Element * pose = object->FirstChildElement("pose");
        Element * truncated = object->FirstChildElement("truncated");
        Element * polygon = object->FirstChildElement("polygon");

        for (int i = 1; ; ++i) {
            Element * x = polygon->FirstChildElement(wxString::Format("x%d", i).ToStdString().c_str());
            Element * y = polygon->FirstChildElement(wxString::Format("y%d", i).ToStdString().c_str());

            if (x == nullptr) {
                break;
            }

            result.point_list.push_back(wxPoint(std::stoi(x->GetText()), std::stoi(y->GetText())));
        }
    } else {
        result.type = ObjectType::DETECTION;

        Element * xmin = bndbox->FirstChildElement("xmin");
        Element * xmax = bndbox->FirstChildElement("xmax");
        Element * ymin = bndbox->FirstChildElement("ymin");
        Element * ymax = bndbox->FirstChildElement("ymax");

        result.point_list.push_back(wxPoint(std::stoi(xmin->GetText()), std::stoi(ymin->GetText())));
        result.point_list.push_back(wxPoint(std::stoi(xmax->GetText()), std::stoi(ymax->GetText())));
    }

    Element * name = object->FirstChildElement("name");
    Element * difficult = object->FirstChildElement("difficult");

    result.name = name->GetText();
    result.difficult = std::stoi(difficult->GetText());

    return result;
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

Element * insertObject(Document * doc, Object obj)
{
    Element * object = doc->NewElement("object");
    if (obj.type == ObjectType::DETECTION) {
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

        name->LinkEndChild(doc->NewText(obj.name.c_str()));
        xmax->SetText(obj.point_list[1].x);
        xmin->SetText(obj.point_list[0].x);
        ymax->SetText(obj.point_list[1].y);
        ymin->SetText(obj.point_list[0].y);
        difficult->SetText(obj.difficult);
    } else if (obj.type == ObjectType::SEGMENTATION) {
        Element * name = doc->NewElement("name");
        Element * pose = doc->NewElement("pose");
        Element * truncated = doc->NewElement("truncated");
        Element * difficult = doc->NewElement("difficult");
        Element * polygon = doc->NewElement("polygon");

        object->LinkEndChild(name);
        object->LinkEndChild(pose);
        object->LinkEndChild(truncated);
        object->LinkEndChild(difficult);
        object->LinkEndChild(polygon);

        name->LinkEndChild(doc->NewText(obj.name.c_str()));
        pose->LinkEndChild(doc->NewText(obj.pose.c_str()));
        truncated->LinkEndChild(doc->NewText(obj.truncated.c_str()));
        difficult->SetText(obj.difficult);
        int i = 0;
        for (auto const & point : obj.point_list) {
            ++i;
            Element * x = doc->NewElement(wxString::Format("x%d",i).ToStdString().c_str());
            Element * y = doc->NewElement(wxString::Format("y%d",i).ToStdString().c_str());

            polygon->LinkEndChild(x);
            polygon->LinkEndChild(y);

            x->SetText(point.x);
            y->SetText(point.y);
        }
    }

    return object;
}

bool saveToXml (std::vector<Object> check_list, wxFileName file, ImageInfo info)
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

    tinyxml2::XMLError a = document.SaveFile((file.GetPath() + "/" + file.GetName() + ".xml").c_str());

    document.Clear();
    return true;
}
