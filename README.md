# Shortest Inspection-Path Queries in Simple Polygons

This application was developed as part of my Bachelor's thesis in Computer Science. It implements multiple algorithms to answer shortest inspection-path queries in simple polygons, as presented in the research paper: [EuroCG08 Abstracts](https://inria.hal.science/inria-00595116/PDF/EuroCG08Abstracts.pdf#page=164).

---

## Installation

### Installation on Linux

#### Required Downloads
The following dependencies must be installed via the command line:
```sh
sudo apt-get install build-essential
sudo apt install libgmp-dev
sudo apt install libmpfr-dev
```
Additionally, download and install the following from their official sources:
- [CGAL v6.0.1](https://github.com/CGAL/cgal/releases/tag/v6.0.1)
- [Qt Open Source](https://www.qt.io/download-open-source)
- [Boost v1.79.0](https://www.boost.org/doc/libs/1_79_0/more/getting_started/unix-variants.html)
- [CMake](https://cmake.org/)

#### Setup
After cloning this repository, configure environment variables so that CMake can locate the required libraries:
```sh
export QT_DIR=/home/"user"/Qt/6.8.2/gcc_64
export CGAL_DIR=/home/"user"/cgal/CGAL-6.0.1
export BOOST_DIR=/home/"user"/cgal/boost_1_87_0
```
*(Replace `"user"` with your actual username and modify the paths according to where you saved the libraries.)*

Next, generate the executable:
```sh
cd Shortest-Inspection-Path-Queries-In-Simple-Polygons
mkdir build && cd build
cmake-gui ..
```
In the **CMake GUI**, click `Configure`, then `Generate`.

---

## Usage
Once the setup is complete, you can run the application using:
```sh
./Bachelor
```

