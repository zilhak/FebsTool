
#include <iostream>
#include <vector>
#include <data/XmlConfig.hpp>

bool SaveConfig(ConfigData data)
{
    return true;
}

ConfigData LoadConfig()
{
    return ConfigData();
}

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
    Element * bndbox = object->FirstChildElement("bndbox");
    Element * difficult = object->FirstChildElement("difficult");

    Element * xmin = bndbox->FirstChildElement("xmin");
    Element * xmax = bndbox->FirstChildElement("xmax");
    Element * ymin = bndbox->FirstChildElement("ymin");
    Element * ymax = bndbox->FirstChildElement("ymax");

    return BoundingBox(name->GetText(),
                  std::stoi(xmin->GetText()),
                  std::stoi(ymin->GetText()),
                  std::stoi(xmax->GetText()),
                  std::stoi(ymax->GetText()),
                  std::stoi(difficult->GetText()));
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