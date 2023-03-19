cmake_minimum_required(VERSION 3.5)

project(gravel VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

#find_package(csnd6.6.0 REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        highlighter.h
        highlighter.cpp
        soundengine.h
        soundengine.cpp
        Parser.h
        Parser.cpp
        csoundparser.h csoundparser.cpp
        coder.h
        coder.cpp
        track.h
        track.cpp
        sequence.h sequence.cpp
        patternEvent.h patternEvent.cpp
        instrumentdefinition.h instrumentdefinition.cpp
        parameter.h parameter.cpp
        dialogaudiosetup.h dialogaudiosetup.cpp dialogaudiosetup.ui

)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gravel
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET gravel APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(gravel SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(gravel
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Try to find the Csound library.
# Once done this will define:
#  CSOUND_FOUND - System has the Csound library
#  CSOUND_INCLUDE_DIRS - The Csound include directories.
#  CSOUND_LIBRARIES - The libraries needed to use the Csound library.
#"$ENV{HOME}/Library/Frameworks/CsoundLib64.framework/Headers"
if(APPLE)
find_path(CSOUND_INCLUDE_DIR csound.h HINTS /Library/Frameworks/CsoundLib64.framework/Headers
"/Library/Frameworks/CsoundLib64.framework/Headers")
else()
find_path(CSOUND_INCLUDE_DIR csound.h PATH_SUFFIXES csound)
endif()


if(APPLE)
find_library(CSOUND_LIBRARY NAMES CsoundLib64 HINTS /Library/Frameworks/CsoundLib64.framework/Versions/Current
"/Library/Frameworks/CsoundLib64.framework/Versions/Current")
else()
find_library(CSOUND_LIBRARY NAMES csound64 csound)
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CSOUND_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(CSOUND
                                  CSOUND_LIBRARY CSOUND_INCLUDE_DIR)
mark_as_advanced(CSOUND_INCLUDE_DIR CSOUND_LIBRARY)

set(CSOUND_INCLUDE_DIRS ${CSOUND_INCLUDE_DIR})
set(CSOUND_LIBRARIES ${CSOUND_LIBRARY} )

#/usr/local/lib/libcsnd6.6.0.dylib
#target_link_libraries(gravel PRIVATE)
##find_path(CSOUND_INCLUDE csound.hpp)

target_include_directories(gravel PRIVATE "${CSOUND_INCLUDE_DIR}")

###find_library(CSOUND_LIB CsoundLib64 REQUIRED PATHS "${CSOUND_LIBRARY}")
###find_library(CSOUND_LIB "${CSOUND_LIBRARY_NAMES}" REQUIRED PATHS "${CSOUND_LIBRARY}")
if(APPLE)
find_library(LIB_CSOUND csnd6.6.0 REQUIRED)
else()
find_library(LIB_CSOUND csnd6 REQUIRED)
endif()
#add_library(CSOUND "${CSOUND_LIB}")
#add_library(gravel )
#target_link_libraries(gravel PRIVATE Qt${QT_VERSION_MAJOR}::Widgets "${CSOUND_LIB}" "${LIB_CSOUND}")
target_link_libraries(gravel PRIVATE Qt${QT_VERSION_MAJOR}::Widgets "${CSOUND_LIBRARIES}" "${LIB_CSOUND}" Qt6::Multimedia)
message("LIB_CSOUND : ${LIB_CSOUND}")
message("CSOUND_LIBRARIES : ${CSOUND_LIBRARIES}")

set_target_properties(gravel PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS gravel
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gravel)
endif()

message("CSOUND_LIB :${CSOUND_LIB}")

