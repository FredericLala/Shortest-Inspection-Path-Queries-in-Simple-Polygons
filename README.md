# Shortest Inspection-Path Queries in Simple Polygons
This application was written as part of my Bachelor's thesis in Computer Science. It implements multiple algorithms to answer shortest inspection-path queries in simple polygons presented in https://inria.hal.science/inria-00595116/PDF/EuroCG08Abstracts.pdf#page=164
## Installation
### Installation on Linux
#### Required Downloads
The following needs to be installed through the command line:
- sudo apt-get install build-essential
- sudo appt install libgmp-dev
- sudo apt install libmpfr-dev

The following needs to be installed from the web:
- https://github.com/CGAL/cgal/releases/tag/v6.0.1
- https://www.qt.io/download-open-source
- https://www.boost.org/doc/libs/1_79_0/more/getting_started/unix-variants.html
- https://cmake.org/

#### Setup
After this repo is cloned the following commands need to be run, so that CMAKE can find the necessary libraries:
- export QT_DIR=/home/"user"/Qt/6.8.2/gcc_64
- export CGAL_DIR=/home/"user"/cgal/CGAL-6.0.1
- export BOOST_DIR=/home/"user"/cgal/boost_1_87_0

The paths need to be modified depending on the location the libraries were saved to.

To generate an executable file, the CMAKE-GUI needs to be opened:
- cd Shortest-Inspection-Path-Queries-In-Simple-Polygons
- mkdir build; cd build
- cmake-gui ..

In the CMAKE-GUI press Configure and then Generate.
The application can now be opened with:
- ./Bachelor
