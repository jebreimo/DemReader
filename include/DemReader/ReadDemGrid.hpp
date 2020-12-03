//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-11-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include <GridLib/Grid.hpp>
#include <GridLib/GridRect.hpp>
#include "GridLib/Unit.hpp"

namespace Dem
{
    GridLib::Grid read_dem_grid(std::istream& stream,
                                GridLib::Unit vertical_unit);

    GridLib::Grid read_dem_grid(std::istream& stream,
                                GridLib::Unit desired_unit,
                                GridLib::GridRect rectangle);
}
