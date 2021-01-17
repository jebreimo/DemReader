# ===========================================================================
# Copyright © 2021 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2021-01-13.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.17)
project(ImageFormats)

set(CMAKE_CXX_STANDARD 17)

find_package(PNG REQUIRED)

add_library(ImageFormats_PngWriter
    include/ImageFormats/ImageFormatsException.hpp
    include/ImageFormats/PngInfo.hpp
    include/ImageFormats/PngTransform.hpp
    include/ImageFormats/PngWriter.hpp
    src/ImageFormats/PngInfo.cpp
    src/ImageFormats/PngTransform.cpp
    src/ImageFormats/PngWriter.cpp
    )

target_include_directories(ImageFormats_PngWriter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

target_link_libraries(ImageFormats_PngWriter
    PUBLIC
        PNG::PNG
    )

add_library(ImageFormats::PngWriter ALIAS ImageFormats_PngWriter)

add_subdirectory(extras/DemoApps)

set_target_properties(ImageFormats_PngWriter
    PROPERTIES
        EXPORT_NAME PngWriter
    )

export(TARGETS ImageFormats_PngWriter
    NAMESPACE ImageFormats::
    FILE ImageFormatsConfig.cmake)
