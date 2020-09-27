# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-09-18.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.17)
project(DemReader VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)

if (NOT DEFINED DEMREADER_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(DEMREADER_MASTER_PROJECT ON)
    else ()
        set(DEMREADER_MASTER_PROJECT OFF)
    endif ()
endif ()

option(DEMREADER_BUILD_DEM "Build dem command" ${DEMREADER_MASTER_PROJECT})

add_library(DemReader
    include/DemReader/DemException.hpp
    include/DemReader/FortranReader.hpp
    src/DemReader/FortranReader.cpp
    src/DemReader/ParseNumber.cpp
    src/DemReader/ParseNumber.hpp
    src/DemReader/DemReader.cpp src/DemReader/DemReader.hpp)

add_library(DemReader::DemReader ALIAS DemReader)

target_include_directories(DemReader
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

export(TARGETS DemReader
    NAMESPACE DemReader::
    FILE DeamReaderConfig.cmake)

if (DEMREADER_BUILD_DEM)
    add_subdirectory(extras/dem)
endif()
