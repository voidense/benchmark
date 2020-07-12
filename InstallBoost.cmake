# Ref: https://gist.github.com/thiagowfx/970e3931387ed7db9a39709a8a130ee9

# BOOST_VERSION
# BOOST_COMPONENTS_TO_BUILD, a list of boost components to be installed
# BOOST_USE_STATIC_LIBS, whether or not to build boost as static library
# All of above shall be defined before including this file

# BOOST_INSTALL_ROOT, the location where boost will be installed

if(NOT BOOST_COMPONENTS)
  message(FATAL_ERROR "No COMPONENTS specified for Boost")
endif()

# Create a list(string) for the build command (e.g. --with-program_options;--with-system)
# and assigns it to BOOST_COMPONENTS_FOR_BUILD
foreach(component ${BOOST_COMPONENTS})
  list(APPEND BOOST_COMPONENTS_FOR_BUILD --with-${component})
endforeach()

if(NOT BOOST_INSTALL_ROOT)
  message(STATUS "BOOST_INSTALL_ROOT is not defined, using project_binary_dir")
  set(BOOST_INSTALL_ROOT ${PROJECT_BINARY_DIR} CACHE PATH "")
endif()

if(BOOST_USE_STATIC_LIBS)
  set(BOOST_STATIC_LINK "link=static")
endif()

# Set the library prefix and library suffix properly.
if(BOOST_USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_STATIC_LIBRARY_PREFIX})
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(LIBRARY_PREFIX ${CMAKE_STATIC_LIBRARY_PREFIX})
  set(LIBRARY_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
  set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_SHARED_LIBRARY_PREFIX})
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(LIBRARY_PREFIX ${CMAKE_SHARED_LIBRARY_PREFIX})
  set(LIBRARY_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()

string(REPLACE "." "_" BOOST_VERSION_REFORMATTED ${BOOST_VERSION})
include(ExternalProject)
ExternalProject_Add(Boost
  URL http://sourceforge.net/projects/boost/files/boost/${BOOST_VERSION}/boost_${BOOST_VERSION_REFORMATTED}.tar.gz
  CONFIGURE_COMMAND ./bootstrap.sh --prefix=${BOOST_INSTALL_ROOT}
  BUILD_COMMAND CXX=${CMAKE_CXX_COMPILER} 
                ./b2 -j16
                cxxflags=-fPIC
                ${BOOST_STATIC_LINK}
                threading=multi
                --prefix=${BOOST_INSTALL_ROOT}
                ${BOOST_COMPONENTS_FOR_BUILD}
                install
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE true
  INSTALL_DIR ${BOOST_INSTALL_ROOT})
