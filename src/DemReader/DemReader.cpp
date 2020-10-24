//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-10-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "DemReader/DemReader.hpp"
#include "RecordA.hpp"
#include "RecordB.hpp"
#include "RecordC.hpp"
#include "DemRecordIterator.hpp"
#include "DemReader/DemException.hpp"

namespace DemReader
{
    namespace
    {
        double dms_to_degrees(const DegMinSec& dms)
        {
            return dms.degree + dms.minute / 60.0 + dms.second / 3600.0;
        }

        void assign(DemFileInfo& info, const RecordA& a)
        {
            info.file_name = a.file_name;
            info.text = a.text;
            if (a.longitude.has_value() && a.latitude.has_value())
            {
                info.geo_location = {dms_to_degrees(*a.latitude),
                                     dms_to_degrees(*a.longitude)};
            }
            info.process_code = a.process_code;
            info.sectional_indicator = a.sectional_indicator;
            info.origin_code = a.origin_code;
            info.dem_level_code = a.dem_level_code;
            info.elevation_pattern_code = a.elevation_pattern_code;
            info.ref_sys = a.ref_sys;
            info.ref_sys_zone = a.ref_sys_zone;
            copy(std::begin(a.map_projection_params),
                 std::end(a.map_projection_params),
                 info.map_projection_params);
            info.horizontal_unit = a.horizontal_unit;
            info.vertical_unit = a.vertical_unit;
            info.polygon_sides = a.polygon_sides;
            for (int i = 0; i < 4; ++i)
            {
                if (a.quadrangle_corners[2 * i]
                    && a.quadrangle_corners[2 * i + 1])
                {
                    info.corners[i] = {*a.quadrangle_corners[2 * i],
                                       *a.quadrangle_corners[2 * i + 1]};
                }
            }
            info.min_elevation = a.min_elevation;
            info.max_elevation = a.max_elevation;
            info.rotation_angle = a.rotation_angle;
            info.elevation_accuracy = a.elevation_accuracy;
            info.x_resolution = a.x_resolution.value_or(1.0);
            info.y_resolution = a.y_resolution.value_or(1.0);
            info.z_resolution = a.z_resolution.value_or(1.0);
            if (a.rows && a.columns)
                info.size = {*a.rows, *a.columns};
            if (a.largest_contour_interval && a.largest_contour_interval_units)
            {
                info.largest_contour_interval = {*a.largest_contour_interval,
                                                 *a.largest_contour_interval_units};
            }
            if (a.smallest_contour_interval && a.smallest_contour_interval_units)
            {
                info.smallest_contour_interval = {*a.smallest_contour_interval,
                                                 *a.smallest_contour_interval_units};
            }
            info.data_source_year = a.data_source_year;
            info.data_completion_year = a.data_completion_year;
            info.inspection_flag = a.inspection_flag;
            info.data_validation_flag = a.data_validation_flag;
            info.suspect_and_void_area_flag = a.suspect_and_void_area_flag;
            info.vertical_datum = a.vertical_datum;
            info.horizontal_datum = a.horizontal_datum;
            info.data_edition = a.data_edition;
            info.percent_void = a.percent_void;
            info.edge_match_flag = a.edge_match_flag;
            info.vertical_datum_shift = a.vertical_datum_shift;
        }
    }

    struct DemReader::Data
    {
        explicit Data(std::istream& stream)
            : iterator(stream)
        {}

        DemRecordIterator iterator;
        DemFileInfo info;
        RecordA recordA;
        RecordC recordC;
        double zFactor = 0.0;
    };

    DemReader::DemReader(std::istream& stream)
        : m_Data(std::make_unique<Data>(stream))
    {
        if (auto a = m_Data->iterator.read_record_a())
            assign(m_Data->info, *a);
        else
            DEM_THROW("The stream doesn't contain a valid record type A.");

        if (m_Data->info.data_validation_flag.value_or(0) != 0)
        {
            if (auto c = m_Data->iterator.read_record_c())
                m_Data->recordC = *c;
            m_Data->iterator.move_to_first_record_b();
        }
        m_Data->zFactor = m_Data->info.z_resolution;
        if (m_Data->recordA.vertical_unit.value_or(2) == 1)
            m_Data->zFactor *= 0.3048;
    }

    DemReader::DemReader(DemReader&& rhs) noexcept = default;

    DemReader::~DemReader() = default;

    DemReader& DemReader::operator=(DemReader&& rhs) noexcept = default;

    std::optional<ElevationData> DemReader::read_next()
    {
        auto b = m_Data->iterator.next_record_b();
        if (!b)
            return {};

        std::vector<double> elevations;
        auto offset = double(b->elevation_base);
        elevations.reserve(b->elevations.size());
        std::transform(b->elevations.begin(), b->elevations.end(),
                       back_inserter(elevations),
                       [&](auto& e){return offset + e * m_Data->zFactor;});
        return {{
                    {b->row, b->column, b->rows, b->columns},
                    b->elevation_base,
                    0.0,
                    move(b->elevations)
                }};
    }
}
