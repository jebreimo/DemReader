//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-10-22.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>
#include <optional>
#include <string>

namespace DemReader
{
    struct SphericalCoordinates
    {
        double latitude = 0;
        double longitude = 0;
    };

    struct ContourInterval
    {
        int16_t size = 0;
        int8_t unit = 0;
    };

    struct CartesianCoordinates
    {
        double northing = 0;
        double easting = 0;
    };

    struct GridSize
    {
        int16_t rows = 0;
        int16_t columns = 0;
    };

    struct Rmse
    {
        int16_t rmse[3] = {};
        int16_t sample_size = 0;
    };

    struct DemFileInfo
    {
        std::string file_name;
        std::string text;
        std::optional<SphericalCoordinates> geo_location;
        std::optional<char> process_code;
        std::string sectional_indicator;
        std::string origin_code;
        std::optional<int16_t> dem_level_code;
        std::optional<int16_t> elevation_pattern_code;
        std::optional<int16_t> ref_sys;
        std::optional<int16_t> ref_sys_zone;
        std::optional<double> map_projection_params[15];
        std::optional<int16_t> horizontal_unit;
        std::optional<int16_t> vertical_unit;
        std::optional<int16_t> polygon_sides;
        std::optional<CartesianCoordinates> corners[4];
        std::optional<double> min_elevation;
        std::optional<double> max_elevation;
        std::optional<double> rotation_angle;
        std::optional<int16_t> elevation_accuracy;
        float x_resolution = 0;
        float y_resolution = 0;
        float z_resolution = 0;
        GridSize size;
        std::optional<ContourInterval> largest_contour_interval;
        std::optional<ContourInterval> smallest_contour_interval;
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

        std::optional<Rmse> datum_rmse;
        std::optional<Rmse> dem_rmse;
    };
}
