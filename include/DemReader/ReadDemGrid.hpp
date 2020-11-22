//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-11-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include <GridLib/GridRect.hpp>
#include "GridLib/Unit.hpp"

namespace Dem
{
    struct DemGrid
    {
        GridLib::GridSize size;
        std::vector<double> values;
        std::vector<uint64_t> missing_values;
        Unit x_unit;
        Unit y_unit;
    };

    DemGrid read_dem_grid(std::istream& stream,
                          Unit vertical_unit,
                          GridLib::GridRect rectangle);
}
