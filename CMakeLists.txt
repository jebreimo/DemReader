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

add_subdirectory(external/GridLib)
add_subdirectory(external/ImageFormats)

if (NOT DEFINED DEMREADER_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(DEMREADER_MASTER_PROJECT ON)
    else ()
        set(DEMREADER_MASTER_PROJECT OFF)
    endif ()
endif ()

option(DEMREADER_BUILD_EXTRAS "Build deminfo and dem2grid." ${DEMREADER_MASTER_PROJECT})
option(DEMREADER_BUILD_TESTS "Build tests." ${DEMREADER_MASTER_PROJECT})

add_library(DemReader
    include/DemReader/DemException.hpp
    include/DemReader/DemReader.hpp
    include/DemReader/ReadDemGrid.hpp
    include/DemReader/RecordA.hpp
    include/DemReader/RecordB.hpp
    include/DemReader/RecordC.hpp
    src/DemReader/DemReader.cpp
    src/DemReader/FortranReader.hpp
    src/DemReader/FortranReader.cpp
    src/DemReader/ParseNumber.cpp
    src/DemReader/ParseNumber.hpp
    src/DemReader/PrintMacros.hpp
    src/DemReader/ReadDemGrid.cpp
    src/DemReader/RecordA.cpp
    src/DemReader/RecordB.cpp
    src/DemReader/RecordC.cpp
    )

target_link_libraries(DemReader
    PUBLIC
        GridLib::GridLib
    )

target_include_directories(DemReader
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

add_library(Dem::DemReader ALIAS DemReader)

export(TARGETS DemReader
    NAMESPACE DemReader::
    FILE DeamReaderConfig.cmake)

if (DEMREADER_BUILD_EXTRAS)
    add_subdirectory(extras/dem2grid)
    add_subdirectory(extras/deminfo)
endif()

if (DEMREADER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/DemReaderTest)
endif ()
