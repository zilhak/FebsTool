project(ImageSearcher)

cmake_minimum_required(VERSION 2.8)

find_package(wxWidgets COMPONENTS core base REQUIRED)
find_package(OpenCV REQUIRED)

include( "${wxWidgets_USE_FILE}" )

add_executable(ImageSearcher main.cpp frame.cpp tinyxml2.cpp)
target_link_libraries(ImageSearcher ${wxWidgets_LIBRARIES})
