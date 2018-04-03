project(FepsTool)
set(CMAKE_CXX_FLAGS "-std=gnu++14 ${CMAKE_CXX_FLAGS}")
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

include_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR})

find_package(wxWidgets COMPONENTS core base REQUIRED)
IF(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
ELSE(wxWidgets_FOUND)
    message("wxWidgets not found!!!!!!!!!")
ENDIF(wxWidgets_FOUND)

set(SRC_BASE main.cpp FebsToolApp.hpp FebsToolApp.cpp ui/SelectDialog.hpp ui/SelectDialog.cpp)
set(SRC_Detection ui/detection/DetectionFrame.hpp ui/detection/DetectionFrame.cpp ui/detection/DetectionInfoBox.cpp ui/detection/DetectionInfoBox.hpp)
set(SRC_Components ui/component/FileExplorer.hpp ui/component/FileExplorer.cpp ui/component/ImagePanel.hpp ui/component/ImagePanel.cpp)
set(SRC_DATA data/tinyxml2.h data/tinyxml2.cpp data/XmlConfig.hpp data/XmlConfig.cpp ui/segmentation/SegmentationFrame.cpp ui/segmentation/SegmentationFrame.hpp ui/detection/DetectionToolBar.cpp ui/detection/DetectionToolBar.hpp)

add_executable(FepsTool ${SRC_BASE} ${SRC_Components} ${SRC_DATA} ${SRC_Detection})
target_link_libraries(FepsTool ${wxWidgets_LIBRARIES})

