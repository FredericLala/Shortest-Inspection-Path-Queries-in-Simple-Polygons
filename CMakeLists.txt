# Minimum required CMake version
cmake_minimum_required(VERSION 3.18)

# Project name
project(Bachelor)

# Set CMake variables for Qt handling
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)

# Find the necessary packages: CGAL and Qt6
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt6)
find_package(Qt6Core REQUIRED)
find_package(Qt6Gui REQUIRED)
find_package(Qt6Widgets REQUIRED)

# Collect all .cpp and .h files from Source and Header directories
set(SOURCES
    Source/main.cpp
    Source/mainwindow.cpp
    Source/onepointquery.cpp
    Source/polygonwidget.cpp
    Source/polygongen.cpp
    Source/shortestpath.cpp
    Source/twopointquery.cpp
    Source/approximatequery.cpp
    Source/generalcase.cpp
)

# List each header file explicitly
set(HEADERS
    Header/common.h
    Header/mainwindow.h
    Header/onepointquery.h
    Header/polygonwidget.h
    Header/polygongen.h
    Header/shortestpath.h
    Header/twopointquery.h
    Header/approximatequery.h
    Header/generalcase.h
)

# Add the collected source files to the executable or library
# Add executable and include directories
add_executable(Bachelor ${SOURCES} ${HEADERS})

# Add Header directory to the include path
target_include_directories(Bachelor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header)

# Link the required libraries
target_link_libraries(Bachelor PUBLIC
  CGAL::CGAL
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)

# If CGAL with Qt6 support is found, link the CGAL Qt viewer library
if(CGAL_Qt6_FOUND)
  target_link_libraries(Bachelor PUBLIC CGAL::CGAL_Basic_viewer)
else()
  message(STATUS "NOTICE: CGAL Qt6 components were not found, some features may not be compiled.")
endif()





