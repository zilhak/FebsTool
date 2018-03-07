project(ImageSearcher)
set(CMAKE_CXX_FLAGS "-std=gnu++14 ${CMAKE_CXX_FLAGS}")
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

include_directories(${CMAKE_SOURCE_DIR})

set(SRCS main.cpp tinyxml2.cpp)
find_package(wxWidgets COMPONENTS core base REQUIRED)
IF(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
ELSE(wxWidgets_FOUND)
    message("wxWidgets not found!!!!!!!!!")
ENDIF(wxWidgets_FOUND)

add_executable(ImageSearcher main.cpp FebsToolApp.hpp FebsToolApp.cpp ui/SelectDialog.hpp ui/SelectDialog.cpp)
target_link_libraries(ImageSearcher ${wxWidgets_LIBRARIES})

