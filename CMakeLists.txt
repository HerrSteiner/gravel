cmake_minimum_required(VERSION 3.5)

project(gravel VERSION 0.34 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#QT dependencies
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia Network)

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
        syncsetup.h syncsetup.cpp syncsetup.ui
        samplelistmodel.h samplelistmodel.cpp
        samplemapper.h samplemapper.cpp samplemapper.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gravel MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    if(ANDROID)
        add_library(gravel SHARED ${PROJECT_SOURCES})
    else()
        add_executable(gravel ${PROJECT_SOURCES})
    endif()
endif()

# Cross-platform Csound detection
if(APPLE)
    # macOS specific paths
    find_path(CSOUND_INCLUDE_DIR csound.h
        HINTS
            /Library/Frameworks/CsoundLib64.framework/Headers
            "$ENV{HOME}/Library/Frameworks/CsoundLib64.framework/Headers"
    )
    find_library(CSOUND_LIBRARY
        NAMES CsoundLib64
        HINTS
            /Library/Frameworks/CsoundLib64.framework
            "$ENV{HOME}/Library/Frameworks/CsoundLib64.framework"
    )
    find_library(LIB_CSOUND NAMES csnd6.6.0 csnd6)
elseif(WIN32)
    # Windows specific paths
    find_path(CSOUND_INCLUDE_DIR csound.h
        HINTS
            "C:/Program Files/Csound6/include"
            "$ENV{CSOUND_HOME}/include"
        PATH_SUFFIXES csound
    )
    find_library(CSOUND_LIBRARY
        NAMES csound64 csound
        HINTS
            "C:/Program Files/Csound6/lib"
            "$ENV{CSOUND_HOME}/lib"
    )
    find_library(LIB_CSOUND NAMES csnd6)
else()
    # Linux/Unix specific paths
    find_path(CSOUND_INCLUDE_DIR csound.h
        HINTS
            /usr/include
            /usr/local/include
            "$ENV{CSOUND_HOME}/include"
        PATH_SUFFIXES csound
    )
    find_library(CSOUND_LIBRARY
        NAMES csound64 csound
        HINTS
            /usr/lib
            /usr/local/lib
            "$ENV{CSOUND_HOME}/lib"
    )
    find_library(LIB_CSOUND NAMES csnd6)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CSOUND
    REQUIRED_VARS CSOUND_LIBRARY CSOUND_INCLUDE_DIR
)

if(NOT CSOUND_FOUND)
    message(WARNING "Csound not found - audio features will be disabled")
else()
    target_include_directories(gravel PRIVATE ${CSOUND_INCLUDE_DIR})
    target_link_libraries(gravel PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        ${CSOUND_LIBRARY}
        ${LIB_CSOUND}
        Qt6::Multimedia
    )

    message(STATUS "Found Csound libraries: ${CSOUND_LIBRARY}, ${LIB_CSOUND}")
    message(STATUS "Csound include path: ${CSOUND_INCLUDE_DIR}")
endif()

# macOS specific bundle settings
if(APPLE)
    set_target_properties(gravel PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER www.block4.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
    )

    set(CSOUND_FRAMEWORK_PATH "/Library/Frameworks/CsoundLib64.framework")
    add_custom_command(TARGET gravel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:gravel>/../Frameworks"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CSOUND_FRAMEWORK_PATH}"
            "$<TARGET_FILE_DIR:gravel>/../Frameworks/CsoundLib64.framework"
    )
else()
    install(TARGETS gravel
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gravel)
endif()
