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
set(SRC_DETECTION ui/detection/DetectionFrame.hpp ui/detection/DetectionFrame.cpp ui/detection/DetectionInfoBox.cpp ui/detection/DetectionInfoBox.hpp ui/detection/DetectionToolBar.cpp ui/detection/DetectionToolBar.hpp)
set(SRC_SEGMENTATION ui/segmentation/SegmentationFrame.cpp ui/segmentation/SegmentationFrame.hpp ui/segmentation/SegmentationInfoBox.cpp ui/segmentation/SegmentationInfoBox.hpp ui/segmentation/SegmentationToolBar.cpp ui/segmentation/SegmentationToolBar.hpp ui/segmentation/HelpDialog.cpp ui/segmentation/HelpDialog.hpp)
set(SRC_COMPONENTS ui/component/FileExplorer.hpp ui/component/FileExplorer.cpp ui/component/ImagePanel.hpp ui/component/ImagePanel.cpp)
set(SRC_DATA data/tinyxml2.h data/tinyxml2.cpp data/XmlConfig.hpp data/XmlConfig.cpp data/ThemeData.hpp data/EventData.cpp data/EventData.hpp)
set(SRC_SETTING ui/settings/SettingFrame.cpp ui/settings/SettingFrame.hpp ui/settings/SettingPanelManager.cpp ui/settings/SettingPanelManager.hpp ui/settings/GeneralSetting.cpp ui/settings/GeneralSetting.hpp ui/settings/NameBoxSetting.cpp ui/settings/NameBoxSetting.hpp data/FebsID.hpp data/FebsStruture.hpp)

add_executable(FepsTool ${SRC_BASE} ${SRC_COMPONENTS} ${SRC_DATA} ${SRC_DETECTION} ${SRC_SEGMENTATION} ${SRC_SETTING})
target_link_libraries(FepsTool ${wxWidgets_LIBRARIES})

