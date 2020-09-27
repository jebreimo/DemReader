//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-09-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <string>

namespace DemReader
{
    struct DegMinSec
    {
        int16_t degree;
        int16_t minute;
        float second;
    };

    struct RecordA
    {
        std::string file_name;
        std::string text;
        std::optional<DegMinSec> longitude;
        std::optional<DegMinSec> latitude;
        std::optional<char> process_code;
        std::string sectional_indicator;
        std::string origin_code;
        std::optional<int16_t> dem_level_code;
        std::optional<int16_t> elevation_pattern_code;
        std::optional<int16_t> ref_sys;
        std::optional<int16_t> ref_sys_zone;
        std::optional<double> map_projection_params[15];
        std::optional<int16_t> hor_unit_of_measure;
        std::optional<int16_t> ver_unit_of_measure;
        std::optional<int16_t> polygon_sides;
        std::optional<double> quadrangle_corners[8];
        std::optional<double> min_elevation;
        std::optional<double> max_elevation;
        std::optional<double> rotation_angle;
        std::optional<int16_t> elevation_accuracy;
        std::optional<float> spatial_resolution[3];
        std::optional<int16_t> rows;
        std::optional<int16_t> columns;
        std::optional<int16_t> largest_contour_interval;
        std::optional<int8_t> largest_contour_interval_units;
        std::optional<int16_t> smallest_contour_interval;
        std::optional<int8_t> smallest_contour_interval_units;
        std::optional<int16_t> data_source_year;
        std::optional<int16_t> data_completion_year;
        std::optional<char> inspection_flag;
        std::optional<int8_t> data_validation_flag;
        std::optional<int8_t> suspect_and_void_area_flag;
        std::optional<int8_t> vertical_datum;
        std::optional<int8_t> horizontal_datum;
        std::optional<int16_t> data_edition;
        std::optional<int16_t> percent_void;
        std::optional<int32_t> edge_match_flag;
        std::optional<double> vertical_datum_shift;
    };

    void print(const RecordA& rec, std::ostream& os);

    class FortranReader;

    [[nodiscard]]
    RecordA read_RecordA(FortranReader& reader);
}
