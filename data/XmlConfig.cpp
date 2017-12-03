
#include "XmlConfig.hpp"


bool saveToXml (int x1, int y1, int x2, int y2, std::string filepath, std::string file_name, int image_height, int image_width, int diff)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLDeclaration * dec = document.NewDeclaration();
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

    filename->LinkEndChild(document.NewText(file_name.c_str()));
    folder->LinkEndChild(document.NewText("${FOLDER}"));
    name->LinkEndChild(document.NewText("car"));
    xmax->SetText(x2);
    xmin->SetText(x1);
    ymax->SetText(y2);
    ymin->SetText(y1);
    difficult->SetText(diff);
    depth->LinkEndChild(document.NewText("3"));
    height->SetText(image_height);
    width->SetText(image_width);
    annotation->LinkEndChild(document.NewText("Bogonet"));
    database->LinkEndChild(document.NewText("The bogonet image database"));
    image->LinkEndChild(document.NewText("SaveZone"));

    document.SaveFile((filepath + "/" + file_name.substr(0, file_name.length() - 4) + ".xml").c_str());
    document.Clear();
    return true;
}