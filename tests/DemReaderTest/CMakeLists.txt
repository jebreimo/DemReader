# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-11-01.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.17)

include(FetchContent)
FetchContent_Declare(catch
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG "v2.13.3"
    )
FetchContent_MakeAvailable(catch)

add_executable(DemReaderTest
    DemReaderTest.cpp
    )

target_link_libraries(DemReaderTest
    Dem::DemReader
    Catch2::Catch2
    )
