# Minimum required CMake version
cmake_minimum_required(VERSION 3.18)

# Project name
project(Bachelor)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Set CMake variables for Qt handling
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "$ENV{QT_DIR}")
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Gui REQUIRED)
# set(CMAKE_AUTORCC ON)

# Find the necessary packages: CGAL and Qt6
set(CMAKE_PREFIX_PATH "$ENV{CGAL_DIR}")
find_package(CGAL REQUIRED)

set(CMAKE_PREFIX_PATH "$ENV{BOOST_DIR}")
find_package(Boost REQUIRED)

find_package(GMP REQUIRED)

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
    Header/logredirect.h
)

# Add the collected source files to the executable or library
# Add executable and include directories
add_executable(Bachelor ${SOURCES} ${HEADERS}
    Header/logredirect.h)

# Add Header directory to the include path
target_include_directories(Bachelor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header)

# Link the required libraries
target_link_libraries(Bachelor PUBLIC
  CGAL::CGAL
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)




