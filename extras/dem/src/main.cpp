//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-09-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <iostream>
#include <cassert>
#include <fstream>
#include <Argos/Argos.hpp>
#include <DemReader/FortranReader.hpp>

struct DegMinSec
{
    int16_t degree;
    int16_t minute;
    float second;
};

std::ostream& operator<<(std::ostream& os, const DegMinSec& dms)
{
    os << dms.degree << ' ' << dms.minute << ' ' << dms.second;
    return os;
}

struct DemRecordA
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
    std::optional<int16_t> ground_planimetric_ref_sys;
    std::optional<int16_t> ground_planimetric_ref_sys_zone;
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
    std::optional<int16_t> largest_primary_contour_interval;
    std::optional<int8_t> largest_primary_contour_interval_source_units;
    std::optional<int16_t> smallest_primary_contour_interval;
    std::optional<int8_t> smallest_primary_contour_interval_source_units;
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

void write(const DemRecordA& rec, std::ostream& os)
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
    WRITE_OPTIONAL(ground_planimetric_ref_sys);
    WRITE_OPTIONAL(ground_planimetric_ref_sys_zone);
    for (int i = 0; i < 15; ++i)
    {
        if (rec.map_projection_params[i])
            os << "map_projection_params[" << i << "]: "
               << *rec.map_projection_params[i] << '\n';
    }
    WRITE_OPTIONAL(hor_unit_of_measure);
    WRITE_OPTIONAL(ver_unit_of_measure);
    WRITE_OPTIONAL(polygon_sides);
    for (int i = 0; i < 8; ++i)
    {
        if (rec.quadrangle_corners[i])
            os << "quadrangle_corners[" << i << "]: "
               << *rec.quadrangle_corners[i] << '\n';
    }
    WRITE_OPTIONAL(min_elevation);
    WRITE_OPTIONAL(max_elevation);
    WRITE_OPTIONAL(rotation_angle);
    WRITE_OPTIONAL(elevation_accuracy);
    for (int i = 0; i < 3; ++i)
    {
        if (rec.spatial_resolution[i])
            os << "spatial_resolution[" << i << "]: "
               << *rec.spatial_resolution[i] << '\n';
    }
    WRITE_OPTIONAL(rows);
    WRITE_OPTIONAL(columns);
    WRITE_OPTIONAL(largest_primary_contour_interval);
    WRITE_OPTIONAL(largest_primary_contour_interval_source_units);
    WRITE_OPTIONAL(smallest_primary_contour_interval);
    WRITE_OPTIONAL(smallest_primary_contour_interval_source_units);
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

std::optional<DegMinSec> read_DegMinSec(DemReader::FortranReader& reader)
{
    auto d = reader.read_int16(4);
    auto m = reader.read_int16(2);
    auto s = reader.read_float32(7);
    if (!d || !m || !s)
        return {};
    return DegMinSec{*d, *m, *s};
}

DemRecordA read_record_a(DemReader::FortranReader& reader)
{
    DemRecordA result;
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
    result.ground_planimetric_ref_sys = reader.read_int16(6);
    result.ground_planimetric_ref_sys_zone = reader.read_int16(6);
    for (auto& param : result.map_projection_params)
        param = reader.read_float64(24);
    result.hor_unit_of_measure = reader.read_int16(6);
    result.ver_unit_of_measure = reader.read_int16(6);
    result.polygon_sides = reader.read_int16(6);
    for (auto& corner : result.quadrangle_corners)
        corner = reader.read_float64(24);
    result.min_elevation = reader.read_float64(24);
    result.max_elevation = reader.read_float64(24);
    result.rotation_angle = reader.read_float64(24);
    result.elevation_accuracy = reader.read_int16(6);
    for (auto& res : result.spatial_resolution)
        res = reader.read_float32(12);
    result.rows = reader.read_int16(6);
    result.columns = reader.read_int16(6);
    result.largest_primary_contour_interval = reader.read_int16(5);
    result.largest_primary_contour_interval_source_units = reader.read_int8(1);
    result.smallest_primary_contour_interval = reader.read_int16(5);
    result.smallest_primary_contour_interval_source_units = reader.read_int8(1);
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
    reader.skip(108);
    return result;
}

int main(int argc, char* argv[])
{
    using namespace Argos;
    auto args = ArgumentParser(argv[0], true)
        .add(Argument("FILE"))
        .parse(argc, argv);

    std::ifstream file(args.value("FILE").asString());
    if (!file)
        args.value("FILE").error("no such file!");
    DemReader::FortranReader reader(file);
    try
    {
        auto recordA = read_record_a(reader);
        write(recordA, std::cout);
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << "\n";
    }
    return 0;
}
