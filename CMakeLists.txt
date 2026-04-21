cmake_minimum_required(VERSION 3.12)

################################################################################

set(Easy3D_MAJOR_VERSION 2)
set(Easy3D_MINOR_VERSION 5)
set(Easy3D_PATCH_VERSION 2)
set(Easy3D_VERSION ${Easy3D_MAJOR_VERSION}.${Easy3D_MINOR_VERSION}.${Easy3D_PATCH_VERSION})

project(Easy3D VERSION ${Easy3D_VERSION})

################################################################################

# Default to Release build
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("-- Setting C++14")

################################################################################

# Offer the user the choice of overriding the installation directories
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if (WIN32 OR MSVC)  # windows
        set(CMAKE_INSTALL_PREFIX "C:/Program Files/Easy3D/${Easy3D_VERSION}" CACHE PATH "Installation directory" FORCE)
    elseif (APPLE)      # macOS
        set(CMAKE_INSTALL_PREFIX "/usr/local/Cellar/easy3d/${Easy3D_VERSION}" CACHE PATH "Installation directory" FORCE)
    else ()             # Linux
        set(CMAKE_INSTALL_PREFIX "/usr/local/easy3d-${Easy3D_VERSION}" CACHE PATH "Installation directory" FORCE)
    endif ()
endif ()

if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS support" FORCE)
endif ()

set(Easy3D_INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "Installation directory for libraries")
set(Easy3D_INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin CACHE PATH "Installation directory for executables")
set(Easy3D_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation directory for header files")
set(Easy3D_INSTALL_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/lib/CMake/Easy3D CACHE PATH "Installation directory for CMake files")
set(Easy3D_INSTALL_RESOURCE_DIR ${CMAKE_INSTALL_PREFIX}/resources CACHE PATH "Installation directory for resources")

# The include directory of the exported header files
set(Easy3D_EXPORT_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")

# Setting RPATH
set(CMAKE_INSTALL_RPATH ${Easy3D_INSTALL_LIB_DIR} CACHE PATH "Installation runtime directory" FORCE)

################################################################################

### Configuration
set(Easy3D_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(Easy3D_THIRD_PARTY ${Easy3D_ROOT}/3rd_party)
set(Easy3D_INCLUDE_DIR ${Easy3D_ROOT})
set(Easy3D_SOURCE_DIR ${Easy3D_ROOT})
set(Easy3D_BINARY_DIR ${CMAKE_BINARY_DIR})

### conditionally compile certain modules depending on libraries found on the system
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

### Where to put the libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Easy3D_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Easy3D_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Easy3D_BINARY_DIR}/lib)

################################################################################

# Build shared libraries
option(Easy3D_BUILD_SHARED_LIBS "Build shared libaries"         ON)
# Build tutorials
option(Easy3D_BUILD_TUTORIALS "Build Easy3D tutorials"          ON)
# Build documentation
option(Easy3D_BUILD_DOCUMENTATION "Build Easy3D documentation"  ON)
# Build tests
option(Easy3D_BUILD_TESTS "Build Easy3D tests programs"         ON)
# Build advanced features that require CGAL (>= v5.1)
option(Easy3D_ENABLE_CGAL "Build advanced features that require CGAL (>= v5.1)"              ON)
# Build advanced examples/applications that require Qt5 (>= v5.6)
option(Easy3D_ENABLE_QT "Build advanced examples/applications that require Qt5 (>= v5.6)"    OFF)
# Build the video encoding module that requires ffmpeg
option(Easy3D_ENABLE_FFMPEG "Build the video encoding module that require ffmpeg (>= v3.4)" ON)

################################################################################

# Discourage users to build Easy3D directly in its root directory
if (${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    message(FATAL_ERROR "Easy3D does not allow building in its source directory. "
            "Please create a separate build directory and run 'cmake ..' from there.")
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################

include(GenerateExportHeader)
if (Easy3D_BUILD_SHARED_LIBS)
    set(Easy3D_LIB_TYPE SHARED)
    if (WIN32 OR MSVC)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    endif ()
else ()
    set(Easy3D_LIB_TYPE STATIC)
endif ()

################################################################################

if (Easy3D_ENABLE_CGAL)
    find_package(CGAL QUIET)
    if (CGAL_FOUND)
        if (CGAL_VERSION VERSION_GREATER_EQUAL "5.1")
            set(Easy3D_HAS_CGAL TRUE)
            message(STATUS "Found CGAL v${CGAL_VERSION}. Include dirs: ${CGAL_INCLUDE_DIRS}")
        else ()
            set(Easy3D_HAS_CGAL FALSE)
            message(WARNING "CGAL version < 5.1 found. Advanced features disabled.")
        endif ()
    else ()
        set(Easy3D_HAS_CGAL FALSE)
        message(WARNING "CGAL not found. Advanced features disabled.")
    endif ()
endif ()

if (Easy3D_ENABLE_FFMPEG)
    include(FindFFMPEG)
    if (FFMPEG_FOUND)
        set(Easy3D_HAS_FFMPEG TRUE)
    else ()
        set(Easy3D_HAS_FFMPEG FALSE)
        message(WARNING "ffmpeg not found. Video encoding disabled.")
    endif ()
endif ()

################################################################################

# Make relative paths absolute
foreach (path LIB BIN INCLUDE CMAKE)
    set(var Easy3D_INSTALL_${path}_DIR)
    if (NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${Easy3D_INSTALL_DIR}/${${var}}")
    endif ()
endforeach ()

function(add_module module headers sources private_dependencies public_dependencies)
    add_library(easy3d_${module} ${Easy3D_LIB_TYPE} ${headers} ${sources})
    generate_export_header(easy3d_${module} EXPORT_FILE_NAME "${Easy3D_EXPORT_INCLUDE_DIR}/easy3d/${module}/export.h")
    target_include_directories(easy3d_${module} PUBLIC
            "$<BUILD_INTERFACE:${Easy3D_INCLUDE_DIR}>"
            "$<BUILD_INTERFACE:${Easy3D_EXPORT_INCLUDE_DIR}>"
            "$<INSTALL_INTERFACE:${Easy3D_INSTALL_INCLUDE_DIR}>"
            )
    target_link_libraries(easy3d_${module}
            PRIVATE ${private_dependencies}
            PUBLIC ${public_dependencies}
            )
    set_target_properties(easy3d_${module} PROPERTIES
            FOLDER "easy3d"
            SOVERSION ${Easy3D_MAJOR_VERSION}
            VERSION ${Easy3D_VERSION}
            )
    if (WIN32 OR MSVC)
        target_compile_definitions(easy3d_${module} PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN VC_EXTRALEAN _CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_DEPRECATE)
    endif ()

    message(STATUS "Adding target: easy3d::${module} (easy3d_${module})")
    add_library(easy3d::${module} ALIAS easy3d_${module})
    set_property(TARGET easy3d_${module} PROPERTY EXPORT_NAME easy3d::${module})
endfunction()

function(install_module module)
    set_target_properties(easy3d_${module} PROPERTIES PUBLIC_HEADER "${${module}_headers}")
    install(TARGETS easy3d_${module}
            EXPORT Easy3DTargets
            RUNTIME DESTINATION "${Easy3D_INSTALL_BIN_DIR}" COMPONENT bin
            LIBRARY DESTINATION "${Easy3D_INSTALL_LIB_DIR}" COMPONENT lib
            ARCHIVE DESTINATION "${Easy3D_INSTALL_LIB_DIR}" COMPONENT lib
            PUBLIC_HEADER DESTINATION "${Easy3D_INSTALL_INCLUDE_DIR}/easy3d/${module}"
            COMPONENT dev)
endfunction()

################################################################################

add_subdirectory(3rd_party)
add_subdirectory(easy3d)

if (Easy3D_BUILD_TUTORIALS)
    add_subdirectory(tutorials)
endif ()

if (Easy3D_BUILD_TESTS)
    add_subdirectory(tests)
endif ()

add_subdirectory(apps)

################################################################################

if (Easy3D_BUILD_DOCUMENTATION)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        configure_file(${doxyfile_in} ${doxyfile} @ONLY)
        add_custom_target(Documentation
                COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM)
    endif ()
endif ()

################################################################################

message(STATUS "****************************************************************************")
message(STATUS "    Summary of Easy3D configuration")
message(STATUS "****************************************************************************")
message(STATUS "    Easy3D version         :  ${Easy3D_VERSION}")
message(STATUS "    Build type             :  ${CMAKE_BUILD_TYPE}")
message(STATUS "    Build shared libraries :  ${Easy3D_BUILD_SHARED_LIBS}")
message(STATUS "    Build tutorials        :  ${Easy3D_BUILD_TUTORIALS}")
message(STATUS "    Build documentation    :  ${Easy3D_BUILD_DOCUMENTATION}")
message(STATUS "    Build tests            :  ${Easy3D_BUILD_TESTS}")
message(STATUS "----------------------------------------------------------------------------")
message(STATUS "    With CGAL (>= v5.1)    :  ${Easy3D_ENABLE_CGAL}")
message(STATUS "    With Qt5 (>= v5.6)     :  ${Easy3D_ENABLE_QT}")
message(STATUS "    With ffmpeg (>= v3.4)  :  ${Easy3D_ENABLE_FFMPEG}")
message(STATUS "----------------------------------------------------------------------------")
message(STATUS "    Build directory        :  ${Easy3D_BINARY_DIR}")
message(STATUS "    Installation directory :  ${CMAKE_INSTALL_PREFIX}")
message(STATUS "****************************************************************************")
