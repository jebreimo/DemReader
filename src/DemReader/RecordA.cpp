//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-09-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "DemReader/RecordA.hpp"

#include <ostream>
#include "FortranReader.hpp"

namespace Dem
{
    std::ostream& operator<<(std::ostream& os, const DegMinSec& dms)
    {
        os << dms.degree << ' ' << dms.minute << ' ' << dms.second;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const CartesianCoordinates& cc)
    {
        return os << cc.northing << ' ' << cc.easting;
    }

    #define WRITE_STRING(name) \
        do { \
            if (!rec.name.empty()) \
                os << #name ": " << rec.name << '\n'; \
        } while (false)

    #define WRITE_OPTIONAL(name) \
        do { \
            if (rec.name) \
                os << #name ": " << *rec.name << '\n'; \
        } while (false)

    #define CAST_AND_WRITE_OPTIONAL(name, type) \
        do { \
            if (rec.name) \
                os << #name ": " << type(*rec.name) << '\n'; \
        } while (false)

    #define WRITE_ARRAY(name) \
        do { \
            for (int i = 0; i < std::size(rec.name); ++i) \
            { \
                if (rec.name[i]) \
                    os << #name "[" << i << "]: " << *rec.name[i] << '\n'; \
            } \
        } while (false)

    void print(const RecordA& rec, std::ostream& os)
    {
        WRITE_STRING(file_name);
        WRITE_STRING(text);
        WRITE_OPTIONAL(longitude);
        WRITE_OPTIONAL(latitude);
        WRITE_OPTIONAL(process_code);
        WRITE_OPTIONAL(process_code);
        WRITE_STRING(sectional_indicator);
        WRITE_STRING(origin_code);
        WRITE_OPTIONAL(dem_level_code);
        WRITE_OPTIONAL(elevation_pattern_code);
        WRITE_OPTIONAL(ref_sys);
        WRITE_OPTIONAL(ref_sys_zone);
        WRITE_ARRAY(map_projection_params);
        WRITE_OPTIONAL(horizontal_unit);
        WRITE_OPTIONAL(vertical_unit);
        WRITE_OPTIONAL(polygon_sides);
        WRITE_ARRAY(quadrangle_corners);
        WRITE_OPTIONAL(min_elevation);
        WRITE_OPTIONAL(max_elevation);
        WRITE_OPTIONAL(rotation_angle);
        WRITE_OPTIONAL(elevation_accuracy);
        WRITE_OPTIONAL(x_resolution);
        WRITE_OPTIONAL(y_resolution);
        WRITE_OPTIONAL(z_resolution);
        WRITE_OPTIONAL(rows);
        WRITE_OPTIONAL(columns);
        WRITE_OPTIONAL(largest_contour_interval);
        WRITE_OPTIONAL(largest_contour_interval_units);
        WRITE_OPTIONAL(smallest_contour_interval);
        WRITE_OPTIONAL(smallest_contour_interval_units);
        WRITE_OPTIONAL(data_source_year);
        WRITE_OPTIONAL(data_completion_year);
        WRITE_OPTIONAL(inspection_flag);
        WRITE_OPTIONAL(data_validation_flag);
        CAST_AND_WRITE_OPTIONAL(suspect_and_void_area_flag, int);
        CAST_AND_WRITE_OPTIONAL(vertical_datum, int);
        CAST_AND_WRITE_OPTIONAL(horizontal_datum, int);
        WRITE_OPTIONAL(data_edition);
        WRITE_OPTIONAL(percent_void);
        WRITE_OPTIONAL(edge_match_flag);
        WRITE_OPTIONAL(vertical_datum_shift);
    }

    std::optional<DegMinSec> read_DegMinSec(FortranReader& reader)
    {
        auto d = reader.read_int16(4);
        auto m = reader.read_int16(2);
        auto s = reader.read_float32(7);
        if (!d || !m || !s)
            return {};
        return DegMinSec{*d, *m, *s};
    }

    RecordA read_record_a(FortranReader& reader)
    {
        RecordA result;
        result.file_name = reader.read_string(40);
        result.text = reader.read_string(40);
        reader.skip(29);
        result.longitude = read_DegMinSec(reader);
        result.latitude = read_DegMinSec(reader);
        result.process_code = reader.read_char();
        reader.skip(1);
        result.sectional_indicator = reader.read_string(3);
        result.origin_code = reader.read_string(4);
        result.dem_level_code = reader.read_int16(6);
        result.elevation_pattern_code = reader.read_int16(6);
        result.ref_sys = reader.read_int16(6);
        result.ref_sys_zone = reader.read_int16(6);
        for (auto& param : result.map_projection_params)
            param = reader.read_float64(24);
        result.horizontal_unit = reader.read_int16(6);
        result.vertical_unit = reader.read_int16(6);
        result.polygon_sides = reader.read_int16(6);
        for (auto& corner : result.quadrangle_corners)
        {
            auto n = reader.read_float64(24);
            auto e = reader.read_float64(24);
            if (n && e)
                corner = {*n, *e};
        }
        result.min_elevation = reader.read_float64(24);
        result.max_elevation = reader.read_float64(24);
        result.rotation_angle = reader.read_float64(24);
        result.elevation_accuracy = reader.read_int16(6);
        result.x_resolution = reader.read_float32(12);
        result.y_resolution = reader.read_float32(12);
        result.z_resolution = reader.read_float32(12);
        result.rows = reader.read_int16(6);
        result.columns = reader.read_int16(6);
        result.largest_contour_interval = reader.read_int16(5);
        result.largest_contour_interval_units = reader.read_int8(1);
        result.smallest_contour_interval = reader.read_int16(5);
        result.smallest_contour_interval_units = reader.read_int8(1);
        result.data_source_year = reader.read_int16(4);
        result.data_completion_year = reader.read_int16(4);
        result.inspection_flag = reader.read_char();
        result.data_validation_flag = reader.read_int8(1);
        result.suspect_and_void_area_flag = reader.read_int8(2);
        result.vertical_datum = reader.read_int8(2);
        result.horizontal_datum = reader.read_int8(2);
        result.data_edition = reader.read_int16(4);
        result.percent_void = reader.read_int16(4);
        result.edge_match_flag = reader.read_int32(8);
        result.vertical_datum_shift = reader.read_float64(7);
        reader.skip(109);
        return result;
    }
}
