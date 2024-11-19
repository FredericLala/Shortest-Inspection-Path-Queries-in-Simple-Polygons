# Install script for directory: C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/pkgs/cgal_x64-windows")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "CGAL_Qt6" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CGAL/" TYPE DIRECTORY FILES
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Arrangement_on_surface_2/include/CGAL/Qt"
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Basic_viewer/include/CGAL/Qt"
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/GraphicsView/include/CGAL/Qt"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "CGAL_Qt6" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cgal/demo/resources" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/GraphicsView/demo/resources/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "CGAL_Qt6" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cgal/demo/icons" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/GraphicsView/demo/icons/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/CGAL" TYPE FILE FILES
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/AUTHORS"
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/CHANGES.md"
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/LICENSE"
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/LICENSE.GPL"
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/LICENSE.LGPL"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/AABB_tree/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Advancing_front_surface_reconstruction/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Algebraic_foundations/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Algebraic_kernel_d/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Algebraic_kernel_for_circles/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Algebraic_kernel_for_spheres/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Alpha_shapes_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Alpha_shapes_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Alpha_wrap_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Apollonius_graph_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Arithmetic_kernel/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Arrangement_on_surface_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/BGL/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Barycentric_coordinates_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Basic_viewer/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Boolean_set_operations_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Bounding_volumes/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Box_intersection_d/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/CGAL_Core/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/CGAL_ImageIO/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/CGAL_ipelets/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Cartesian_kernel/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Circular_kernel_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Circular_kernel_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Circulator/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Classification/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Combinatorial_map/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Cone_spanners_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Convex_decomposition_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Convex_hull_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Convex_hull_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Convex_hull_d/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Distance_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Distance_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Envelope_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Envelope_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Filtered_kernel/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Generalized_map/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Generator/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/GraphicsView/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/HalfedgeDS/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Hash_map/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Heat_method_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Homogeneous_kernel/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Hyperbolic_triangulation_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Inscribed_areas/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Interpolation/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Intersections_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Intersections_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Interval_skip_list/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Interval_support/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Jet_fitting_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Kernel_23/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Kernel_d/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Kinetic_space_partition/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Kinetic_surface_reconstruction/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/LEDA/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Linear_cell_complex/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Matrix_search/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Mesh_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Mesh_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Mesher_level/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Minkowski_sum_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Minkowski_sum_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Modifier/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Modular_arithmetic/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Nef_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Nef_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Nef_S2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/NewKernel_d/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Number_types/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Optimal_bounding_box/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Optimal_transportation_reconstruction_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Optimisation_basic/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Orthtree/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Partition_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Periodic_2_triangulation_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Periodic_3_mesh_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Periodic_3_triangulation_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Periodic_4_hyperbolic_triangulation_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Point_set_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Point_set_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Point_set_processing_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Poisson_surface_reconstruction_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Polygon/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Polygon_mesh_processing/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Polygon_repair/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Polygonal_surface_reconstruction/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Polyhedron/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Polyline_simplification_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Polynomial/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Polytope_distance_d/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Principal_component_analysis/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Principal_component_analysis_LGPL/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Profiling_tools/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Property_map/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/QP_solver/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Random_numbers/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Ridges_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/SMDS_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/STL_Extension/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Scale_space_reconstruction_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/SearchStructures/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Segment_Delaunay_graph_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Segment_Delaunay_graph_Linf_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Set_movable_separability_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Shape_detection/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Shape_regularization/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Skin_surface_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Snap_rounding_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Solver_interface/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Spatial_searching/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Spatial_sorting/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Straight_skeleton_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Straight_skeleton_extrusion_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Stream_lines_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Stream_support/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Subdivision_method_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesh/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesh_approximation/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesh_deformation/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesh_parameterization/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesh_segmentation/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesh_shortest_path/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesh_simplification/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesh_skeletonization/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesh_topology/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_mesher/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Surface_sweep_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/TDS_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/TDS_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Testsuite/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Tetrahedral_remeshing/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Three/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Triangulation/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Triangulation_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Triangulation_3/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Triangulation_on_sphere_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Union_find/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Visibility_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Voronoi_diagram_2/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Weights/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/x64-windows-rel/include/CGAL" REGEX "/\\.svn$" EXCLUDE REGEX "/qt$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cgal" TYPE DIRECTORY FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/cmake/modules/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cgal" TYPE FILE FILES
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/lib/cmake/CGAL/CGALConfig.cmake"
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/lib/cmake/CGAL/CGALConfigBuildVersion.cmake"
    "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/lib/cmake/CGAL/CGALConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cgal" TYPE FILE FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/x64-windows-rel/config/CGALConfig-installation-dirs.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/src/v6.0-c1a79220fa.clean/Installation/auxiliary/cgal_create_cmake_script.1")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/x64-windows-rel/Installation/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
