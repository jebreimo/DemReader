# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-09-27.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.17)
project(dem2grid)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(argos
    GIT_REPOSITORY "https://github.com/jebreimo/Argos.git"
    GIT_TAG v0.99.19)
FetchContent_MakeAvailable(argos)

add_executable(dem2grid
    src/dem2grid.cpp
    )

target_link_libraries(dem2grid
    PRIVATE
        Argos::Argos
        Dem::DemReader
    )
