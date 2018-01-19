cmake_minimum_required(VERSION 3.5)
project(mcdlib)

option(ENABLE_OMETIFF_SUPPORT "Enable OME-TIFF support" ON)
option(BUILD_PYTHON_MODULE "Build python module" ON)
option(BUILD_UNIT_TESTS "Build test cases" OFF)

include(GNUInstallDirs)

set(MCDLIB_VERSION_MAJOR 0)
set(MCDLIB_VERSION_MINOR 1)
set(MCDLIB_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_ROOT
        src/MCDFile.cpp
        src/MCDFileMetadata.cpp
        src/MCDFileAcquisitionData.cpp)

set(SOURCE_FILES_DATA
        src/data/MetadataBase.cpp
        src/data/Slide.cpp
        src/data/Panorama.cpp
        src/data/Region.cpp
        src/data/Acquisition.cpp
        src/data/Channel.cpp
        src/data/ChannelData.cpp)

set(SOURCE_FILES_PUGIXML
        src/pugixml/pugixml.cpp)

set(SOURCE_FILES ${SOURCE_FILES_ROOT} ${SOURCE_FILES_DATA} ${SOURCE_FILES_PUGIXML})

set(HEADER_FILES_ROOT
        src/MCDFile.h
        src/MCDFileMetadata.h
        src/MCDFileAcquisitionData.h
        src/MCDFileIOException.h
        src/MCDFileMalformedException.h)

set(HEADER_FILES_DATA
        src/data/MetadataBase.h
        src/data/Slide.h
        src/data/Panorama.h
        src/data/Region.h
        src/data/Acquisition.h
        src/data/Channel.h
        src/data/ChannelData.h)

set(HEADER_FILES_PUGIXML
        src/pugixml/pugixml.hpp
        src/pugixml/pugiconfig.hpp)

set(HEADER_FILES ${HEADER_FILES_ROOT} ${HEADER_FILES_DATA} ${HEADER_FILES_PUGIXML})

add_library(mcd SHARED ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(mcd PROPERTIES VERSION ${MCDLIB_VERSION_MAJOR}.${MCDLIB_VERSION_MINOR}.${MCDLIB_VERSION_PATCH})

install(TARGETS mcd LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADER_FILES_ROOT} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${HEADER_FILES_DATA} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/data)
install(FILES ${HEADER_FILES_PUGIXML} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/pugixml)

if (ENABLE_OMETIFF_SUPPORT)
    find_package(OMEFiles REQUIRED)
    target_link_libraries(mcd PUBLIC OME::Files)
    target_compile_definitions(mcd PUBLIC -DOMETIFF_SUPPORT_ENABLED)
endif ()

if (BUILD_PYTHON_MODULE)
    add_subdirectory(python)
endif ()

if (BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif ()
