//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-11-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "DemReader/ReadDemGrid.hpp"
#include "DemReader/DemReader.hpp"

namespace Dem
{
    constexpr float METERS_PER_FOOT = 0.3048;
    constexpr int16_t UNKNOWN = -32767;

    GridLib::Unit fromDemUnit(int16_t unit)
    {
        switch (unit)
        {
        case 1:
            return GridLib::Unit::FEET;
        case 2:
            return GridLib::Unit::METERS;
        case 3:
            return GridLib::Unit::ARC_SECONDS;
        default:
            return GridLib::Unit::UNDEFINED;
        }
    }

    GridLib::Grid read_dem_grid(std::istream& stream, GridLib::Unit vertical_unit)
    {
        return read_dem_grid(stream, vertical_unit,
                             {{0, 0}, {UINT_MAX, UINT_MAX}});
    }

    GridLib::Grid read_dem_grid(std::istream& stream,
                                GridLib::Unit desired_unit,
                                GridLib::GridRect rectangle)
    {
        GridLib::Grid grid;
        Dem::DemReader reader(stream);
        auto& a = reader.record_a();
        auto hUnit = fromDemUnit(a.horizontal_unit.value_or(0));
        auto cRes = a.x_resolution.value_or(1.0);
        auto rRes = a.y_resolution.value_or(1.0);

        auto vUnit = fromDemUnit(a.vertical_unit.value_or(0));
        auto vRes = a.z_resolution.value_or(1.0);

        double factor = 1.0;
        if (desired_unit == GridLib::Unit::METERS)
        {
            if (vUnit == GridLib::Unit::FEET)
            {
                factor = vRes * METERS_PER_FOOT;
                vRes = 1.0;
                vUnit = GridLib::Unit::METERS;
            }
            else
            {
                factor = vRes;
                vRes = 1.0;
            }

            if (hUnit == GridLib::Unit::FEET)
            {
                rRes *= METERS_PER_FOOT;
                cRes *= METERS_PER_FOOT;
                hUnit = GridLib::Unit::METERS;
            }
        }
        else if (desired_unit == GridLib::Unit::FEET)
        {
            if (vUnit == GridLib::Unit::METERS)
            {
                factor = vRes / METERS_PER_FOOT;
                vRes = 1.0;
                vUnit = GridLib::Unit::FEET;
            }
            else
            {
                factor = vRes;
                vRes = 1.0;
            }

            if (hUnit == GridLib::Unit::METERS)
            {
                rRes *= 1.0 / METERS_PER_FOOT;
                cRes *= 1.0 / METERS_PER_FOOT;
                hUnit = GridLib::Unit::FEET;
            }
        }

        grid.setVerticalAxis({vRes, vUnit});
        grid.setRowAxis({rRes, hUnit});
        grid.setColumnAxis({cRes, hUnit});
        grid.setRotationAngle(a.rotation_angle.value_or(0));

        GridLib::MutableArrayView2D<double> values;
        GridLib::MutableBitArrayView2D unknown;
        auto rows = a.rows.value_or(1);
        auto cols = a.columns.value_or(1);
        while (auto b = reader.next_record_b())
        {
            if (values.empty())
            {
                if (rows == 1)
                    rows = b->rows;
                // DEM files uses columns as the major axis.
                grid.resize(cols, rows);
                values = grid.elevations();
                unknown = grid.unknownElevations();
            }
            for (int i = 0; i < b->columns; ++i)
            {
                for (int j = 0; j < b->rows; ++j)
                {
                    auto elev = b->elevations[i];
                    if (elev != UNKNOWN)
                        values(i + b->column, j + b->row) = elev * factor;
                    else
                        unknown.set(i + b->column, j + b->row, true);
                }
            }
        }

        return grid;
    }
}
