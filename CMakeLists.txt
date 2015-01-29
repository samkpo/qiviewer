cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)
project(qiviewer)
set(PROJECT_LONGNAME "QIviewer")
set(PROJECT_VERSION "0.7.0")
#prefixes
set(BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
set(DATADIR ${CMAKE_INSTALL_PREFIX}/share)
set(PKGDATADIR ${DATADIR}/qiviewer)
set(XDG_APPS_INSTALL_DIR /usr/share/applications)
message("PKGDATADIR: " ${PKGDATADIR})

# Global CMake options
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)

#let's give support for c++11
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")

#Lets find Qt
FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_MIN_VERSION 4.8.0)
INCLUDE(FindQt4)
INCLUDE(${QT_USE_FILE})

option(USE_DEBUGGING "Use debbuging messages" OFF)
IF(USE_DEBUGGING)
    MESSAGE("Pelotudo")
    ADD_DEFINITIONS(-DUSE_DEBUGGING)
ENDIF()

option(USE_DEVELOPING "So, you are programming? good luck" OFF)
IF(USE_DEVELOPING)
    MESSAGE("Pelotudo")
    ADD_DEFINITIONS(-DUSE_DEVELOPING)
ENDIF()

SET(ICON_THEMES_DEFAULT_FOLDER "/usr/share/icons")

#busco y agrego cstringutils
#pkg_check_modules(FILEINFO REQUIRED fileinfo>=0.0.1)
#link_directories(${FILEINFO_LIBRARY_DIRS})
#include_directories(${FILEINFO_INCLUDE_DIRS})
#set(LIBS2 ${FILEINFO_LIBRARIES})

# Add subdirectories
add_subdirectory(src)
#add_subdirectory(src/tests)

# Testing configuration
#enable_testing()
#set(TEST_LINK_LIBRARIES Qt5::Test)

