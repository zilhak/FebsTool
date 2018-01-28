
#include <iostream>
#include "XmlConfig.hpp"

cyRect loadFromXml (wxString image_file_not_ext)
{
    cyRect result;
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError err = document.LoadFile((image_file_not_ext + ".xml").c_str());
    if (err != tinyxml2::XML_NO_ERROR) {
        return result;
    }
    tinyxml2::XMLNode * root = document.FirstChildElement("annotation");
    tinyxml2::XMLElement * object = root->FirstChildElement("object");
    tinyxml2::XMLElement * bndbox = object->FirstChildElement("bndbox");

    tinyxml2::XMLElement * xmin = bndbox->FirstChildElement("xmin");
    tinyxml2::XMLElement * xmax = bndbox->FirstChildElement("xmax");
    tinyxml2::XMLElement * ymin = bndbox->FirstChildElement("ymin");
    tinyxml2::XMLElement * ymax = bndbox->FirstChildElement("ymax");

    result.x1 = std::stoi(xmin->GetText());
    result.x2 = std::stoi(xmax->GetText());
    result.y1 = std::stoi(ymin->GetText());
    result.y2 = std::stoi(ymax->GetText());

    return result;
}

ImageInfo loadXmlInfo (wxString image_file_not_ext)
{
    ImageInfo result(0, 0, wxT(""));

    tinyxml2::XMLDocument document;
    tinyxml2::XMLError err = document.LoadFile((image_file_not_ext + ".xml").c_str());
    if (err != tinyxml2::XML_NO_ERROR) {
        return result;
    }

    tinyxml2::XMLNode * root = document.FirstChildElement("annotation");

    tinyxml2::XMLElement * object = root->FirstChildElement("object");
    tinyxml2::XMLElement * name = object->FirstChildElement("name");
    tinyxml2::XMLElement * diff = object->FirstChildElement("difficult");

    tinyxml2::XMLElement * size = root->FirstChildElement("size");
    tinyxml2::XMLElement * depth = size->FirstChildElement("depth");
    tinyxml2::XMLElement * height = size->FirstChildElement("height");
    tinyxml2::XMLElement * width = size->FirstChildElement("height");

    result.type = name->GetText();
    result.image_width = std::stoi(width->GetText());
    result.image_height = std::stoi(height->GetText());
    result.depth = std::stoi(depth->GetText());
    result.diff = std::stoi(diff->GetText());

    return result;
}

bool saveToXml (cyRect check, wxFileName file, ImageInfo info)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLNode * root = document.NewElement("annotation");
    tinyxml2::XMLElement * filename = document.NewElement("filename");
    tinyxml2::XMLElement * folder = document.NewElement("folder");
    tinyxml2::XMLElement * object = document.NewElement("object");
    tinyxml2::XMLElement * name = document.NewElement("name");
    tinyxml2::XMLElement * bndbox = document.NewElement("bndbox");
    tinyxml2::XMLElement * xmax = document.NewElement("xmax");
    tinyxml2::XMLElement * xmin = document.NewElement("xmin");
    tinyxml2::XMLElement * ymax = document.NewElement("ymax");
    tinyxml2::XMLElement * ymin = document.NewElement("ymin");
    tinyxml2::XMLElement * difficult = document.NewElement("difficult");
    tinyxml2::XMLElement * size = document.NewElement("size");
    tinyxml2::XMLElement * depth = document.NewElement("depth");
    tinyxml2::XMLElement * height = document.NewElement("height");
    tinyxml2::XMLElement * width = document.NewElement("width");
    tinyxml2::XMLElement * source = document.NewElement("source");
    tinyxml2::XMLElement * annotation = document.NewElement("annotation");
    tinyxml2::XMLElement * database = document.NewElement("database");
    tinyxml2::XMLElement * image = document.NewElement("image");

    document.InsertFirstChild(root);
    root->LinkEndChild(filename);
    root->LinkEndChild(folder);
    root->LinkEndChild(object);
    root->LinkEndChild(size);
    root->LinkEndChild(source);

    object->LinkEndChild(name);
    object->LinkEndChild(bndbox);
    object->LinkEndChild(difficult);

    size->LinkEndChild(depth);
    size->LinkEndChild(height);
    size->LinkEndChild(width);

    source->LinkEndChild(annotation);
    source->LinkEndChild(database);
    source->LinkEndChild(image);

    bndbox->LinkEndChild(xmin);
    bndbox->LinkEndChild(ymin);
    bndbox->LinkEndChild(xmax);
    bndbox->LinkEndChild(ymax);

    filename->LinkEndChild(document.NewText(file.GetFullName().c_str()));
    folder->LinkEndChild(document.NewText("${FOLDER}"));
    name->LinkEndChild(document.NewText(info.type.c_str()));
    xmax->SetText(check.x2);
    xmin->SetText(check.x1);
    ymax->SetText(check.y2);
    ymin->SetText(check.y1);
    difficult->SetText(info.diff);
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