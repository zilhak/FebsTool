#include <string>
#include "data/tinyxml2.h"

#ifndef IMAGESEARCHER_XMLCONFIG_H
#define IMAGESEARCHER_XMLCONFIG_H

bool saveToXml (int x1,
                int y1,
                int x2,
                int y2,
                std::string filepath,
                std::string file_name,
                int image_height,
                int image_width,
                int diff);

#endif //IMAGESEARCHER_XMLCONFIG_H
