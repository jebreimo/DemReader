//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-10-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <iostream>
#include <fstream>
#include <Argos/Argos.hpp>
#include <Yson/JsonWriter.hpp>
#include <DemReader/DemReader.hpp>
#include "GridLib/GridRect.hpp"

const Yson::JsonParameters FLAT(Yson::JsonFormatting::FLAT);

const char* UNITS[] = {
    "radians",
    "feet",
    "meters",
    "arc-seconds"
};

constexpr float METERS_PER_FOOT = 0.3048;

std::string get_unit_name(int unit)
{
    if (0 <= unit && unit < 4)
        return UNITS[unit];
    return std::to_string(unit);
}

void write_metadata(Yson::JsonWriter& writer, const Dem::RecordA& a, char unit)
{
    auto hor_unit = a.horizontal_unit.value_or(2);
    auto ver_unit = a.vertical_unit.value_or(2);
    float hor_res = 1.0;
    if (hor_unit == 1 && unit == 'm')
    {
        hor_unit = ver_unit = 2;
        hor_res = METERS_PER_FOOT;
    }
    else if (hor_unit == 2 && unit == 'f')
    {
        hor_unit = ver_unit = 1;
        hor_res = 1 / METERS_PER_FOOT;
    }

    writer.key("x_resolution").value(a.x_resolution.value_or(1.0) * hor_res);
    writer.key("y_resolution").value(a.y_resolution.value_or(1.0) * hor_res);

    if (unit == 'r')
    {
        writer.key("z_resolution").value(a.z_resolution.value_or(1.0));
    }

    writer.key("horizontal_unit").value(get_unit_name(hor_unit));
    writer.key("vertical_unit").value(get_unit_name(ver_unit));
}

double get_vertical_resolution(const Dem::RecordA& a, char unit)
{
    if (unit == 'r')
        return 1.0;

    double z_res = a.z_resolution.value_or(1.0);
    auto ver_unit = a.vertical_unit.value_or(unit == 'm' ? 2 : 1);

    if (unit == 'm' && ver_unit == 1)
        z_res *= METERS_PER_FOOT;
    else if (unit == 'f' && ver_unit == 2)
        z_res *= 1 / METERS_PER_FOOT;

    return z_res;
}

void write_elevations(Yson::JsonWriter& writer,
                      const Dem::RecordA& a,
                      const Dem::RecordB& b,
                      const GridLib::GridRect& rect,
                      char unit)
{
    GridLib::GridRect record_rect{{unsigned(b.row - 1), unsigned(b.column - 1)},
                                  {unsigned(b.rows), unsigned(b.columns)}};
    auto out_rect = get_intersection(rect, record_rect);
    if (is_empty(out_rect))
        return;

    auto z_off = b.elevation_base;

    auto z_res = get_vertical_resolution(a, unit);

    writer.beginObject();
    if (unit == 'r')
        writer.key("z_offset").value(z_off);
    writer.key("z_res").value(z_res);
    writer.key("grid_pos").beginObject(FLAT)
        .key("row").value(out_rect.pos.row - rect.pos.row)
        .key("column").value(out_rect.pos.column - rect.pos.column)
        .endObject();
    writer.key("grid_size").beginObject(FLAT)
        .key("rows").value(out_rect.size.rows)
        .key("columns").value(out_rect.size.columns)
        .endObject();
    writer.key("grid").beginArray(Yson::JsonParameters(10));

    GridLib::GridRect value_rect = {{out_rect.pos.row - record_rect.pos.row,
                                     out_rect.pos.column - record_rect.pos.column},
                                    out_rect.size};
    if (unit == 'r')
    {
        for (auto z : b.elevations)
            writer.value(z);
    }
    else
    {
        for (unsigned i = 0; i < value_rect.size.columns; ++i)
        {
            for (unsigned j = 0; j < value_rect.size.rows; ++j)
            {
                unsigned n = (i + value_rect.pos.column) * record_rect.size.columns
                    + value_rect.pos.row + j;
                auto elevation = b.elevations[n];
                if (elevation > -32767)
                    writer.value(z_off + elevation * z_res);
                else
                    writer.value(-32767);
            }
        }
        //for (auto z : b.elevations)
        //    writer.value(z_off + z * z_res);
    }
    writer.endArray();
    writer.endObject();
}

int main(int argc, char* argv[])
{
    using namespace Argos;
    auto args = ArgumentParser(argv[0], true)
        .allowAbbreviatedOptions(true)
        .add(Argument("FILE").text("The name of the DEM file."))
        .add(Argument("OUTPUT").optional(true)
                 .text("The name of the output file. Output is written to"
                       " stdout if this is not given."))
        .add(Option{"-p", "--position"}.argument("ROW,COL")
                 .text("Set the bottom left row and column in the input file"
                       " that will be processed. Defaults to 0, 0."))
        .add(Option{"-s", "--size"}.argument("ROWS,COLS")
                 .text("The number of rows and columns in the input file that"
                       " will be processed. Values that are too large are"
                       " adjusted automatically. Defaults to everything."))
        .add(Option{"-u", "--unit"}.argument("UNIT")
                 .text("Set the units used in the output: 'm' for meters,"
                       " 'f' for feet, and 'r' for 'raw', i.e. same as input."
                       " Default is meters."))
        .parse(argc, argv);

    auto size = args.value("--size").split(',', 2, 2).asUInts({UINT_MAX, UINT_MAX});
    auto position = args.value("--position").split(',', 2, 2).asUInts({0, 0});
    auto unitStr = args.value("--unit").asString("m");
    if (unitStr != "m" && unitStr != "f" && unitStr != "r")
        args.value("--unit").error();

    auto unit = unitStr[0];

    GridLib::GridRect rect = {{position[0], position[1]}, {size[0], size[1]}};
    std::ifstream file(args.value("FILE").asString());
    if (!file)
        args.value("FILE").error("no such file!");

    try
    {
        Dem::DemReader reader(file);
        Yson::JsonWriter writer(std::cout, Yson::JsonFormatting::FORMAT);
        writer.beginObject();
        write_metadata(writer, reader.record_a(), unit);
        writer.key("elevations").beginArray();

        while (auto b = reader.next_record_b())
        {
            write_elevations(writer, reader.record_a(), *b, rect, unit);
        }

        writer.endArray();
        writer.endObject();
        writer.flush();
        std::cout << std::endl;
        //const auto& a = reader.record_a();
        //double zScale = a.z_resolution.value_or(1.0)
        //    * (a.vertical_unit.value_or(2) != 1 ? 1.0 : 0.3048);
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << "\n";
    }
    return 0;
}
