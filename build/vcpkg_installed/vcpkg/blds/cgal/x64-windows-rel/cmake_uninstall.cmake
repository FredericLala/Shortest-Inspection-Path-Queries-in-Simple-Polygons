if(NOT EXISTS "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/x64-windows-rel/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/x64-windows-rel/install_manifest.txt")
endif(NOT EXISTS "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/x64-windows-rel/install_manifest.txt")

if (NOT DEFINED CMAKE_INSTALL_PREFIX)
  set (CMAKE_INSTALL_PREFIX "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/pkgs/cgal_x64-windows")
endif ()
 message(${CMAKE_INSTALL_PREFIX})

file(READ "C:/Users/frede/Bachelor/build/vcpkg_installed/vcpkg/blds/cgal/x64-windows-rel/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
  message(STATUS "Uninstalling $ENV{DESTDIR}${file}")
  if(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
    execute_process(
      COMMAND "C:/Program Files/CMake/bin/cmake.exe" -E remove "$ENV{DESTDIR}${file}"
      OUTPUT_STRIP_TRAILING_WHITESPACE
      OUTPUT_VARIABLE rm_out
      RESULT_VARIABLE rm_retval
      )
    if(NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing $ENV{DESTDIR}${file}")
    endif(NOT "${rm_retval}" STREQUAL 0)
  else(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
    message(STATUS "File $ENV{DESTDIR}${file} does not exist.")
  endif(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
endforeach(file)
