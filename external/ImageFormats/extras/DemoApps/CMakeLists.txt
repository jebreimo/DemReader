# ===========================================================================
# Copyright © 2021 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2021-01-14.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.17)

add_executable(CheckerBoardPng
    CheckerBoardPng.cpp
    )

target_link_libraries(CheckerBoardPng
    ImageFormats::PngWriter
    )
