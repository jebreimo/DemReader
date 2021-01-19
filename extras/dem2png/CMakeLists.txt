# ===========================================================================
# Copyright © 2021 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2021-01-17.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.17)
project(dem2png)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(argos
    GIT_REPOSITORY "https://github.com/jebreimo/Argos.git"
    GIT_TAG v0.99.19)
FetchContent_Declare(fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
    GIT_TAG 7.1.3)
FetchContent_MakeAvailable(argos fmt)

add_executable(dem2png
    src/dem2png.cpp
    )

target_link_libraries(dem2png
    PRIVATE
        Argos::Argos
        Dem::DemReader
        fmt::fmt
        ImageFormats::PngWriter
    )
