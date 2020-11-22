# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-08-28.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.17)
project(GridLib)

set(CMAKE_CXX_STANDARD 17)

function(GridLib_enable_all_warnings target)
    target_compile_options(${target}
        PRIVATE
            $<$<CXX_COMPILER_ID:Clang>:-Wall>
            $<$<CXX_COMPILER_ID:GNU>:-Wall>
            $<$<CXX_COMPILER_ID:AppleClang>:-Wall>
            $<$<CXX_COMPILER_ID:MSVC>:/W4>)
endfunction()

add_library(GridLib
    include/GridLib/GridRect.hpp
    include/GridLib/Array2D.hpp
    include/GridLib/ArrayView2D.hpp
    include/GridLib/BitArray2D.hpp
    include/GridLib/BitArrayView2D.hpp
    src/GridLib/Grid.cpp
    include/GridLib/Grid.hpp
    include/GridLib/MutableArrayView2D.hpp
    include/GridLib/MutableBitArrayView2D.hpp
    include/GridLib/Unit.hpp
    src/GridLib/GridRect.cpp
    )

target_include_directories(GridLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

add_library(GridLib::GridLib ALIAS GridLib)

enable_testing()

add_subdirectory(tests)

export(TARGETS GridLib
    NAMESPACE GridLib::
    FILE GridLibConfig.cmake)
