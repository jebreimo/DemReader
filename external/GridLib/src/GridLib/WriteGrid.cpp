//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "GridLib/WriteGrid.hpp"
#include <Yson/JsonWriter.hpp>

namespace GridLib
{
    namespace
    {
        void writeAxis(Yson::Writer& writer, const Axis& axis)
        {
            writer.beginObject();
            writer.key("unit").value(toString(axis.unit));
            writer.key("resolution").value(axis.resolution);
            writer.endObject();
        }

        void writeMetadata(Yson::Writer& writer, const Grid& grid)
        {
            writer.beginObject();
            writer.key("rowCount").value(uint64_t(grid.rowCount()));
            writer.key("columnCount").value(uint64_t(grid.columnCount()));
            writer.key("row_axis");
            writeAxis(writer, grid.rowAxis());
            writer.key("column_axis");
            writeAxis(writer, grid.columnAxis());
            writer.key("vertical_axis");
            writeAxis(writer, grid.verticalAxis());
            writer.key("rotation_angle").value(grid.rotationAngle());

            if (grid.axisOrientation() != RotationDir::COUNTERCLOCKWISE)
                writer.key("axis_orientation").value("CLOCKWISE");

            if (const auto& coords = grid.planarCoords())
            {
                writer.key("planar_location")
                    .beginObject()
                    .key("northing").value(coords->northing)
                    .key("easting").value(coords->easting)
                    .key("zone").value(coords->zone)
                    .endObject();
            }

            if (const auto& coords = grid.sphericalCoords())
            {
                writer.key("spherical_location")
                    .beginObject()
                    .key("latitude").value(coords->latitude)
                    .key("longitude").value(coords->longitude)
                    .endObject();
            }

            if (const auto& refSys = grid.referenceSystem())
            {
                writer.key("reference_system")
                    .beginObject()
                    .key("horizontal_system").value(refSys->horizontal);
                if (refSys->vertical)
                    writer.key("vertical_system").value(refSys->vertical);
                writer.endObject();
            }

            writer.endObject();
        }

        void writeElevations(Yson::Writer& writer,
                             const Chorasmia::ArrayView2D<double>& values,
                             std::optional<double> unknownElevation)
        {
            writer.beginArray();
            for (const auto& row : values)
            {
                writer.beginArray(Yson::JsonParameters(10));
                for (const auto& value : row)
                {
                    if (!unknownElevation || value != *unknownElevation)
                        writer.value(value);
                    else
                        writer.null();
                }
                writer.endArray();
            }
            writer.endArray();
        }
    }

    void writeAsJson(std::ostream& stream, const Grid& grid)
    {
        Yson::JsonWriter writer(stream, Yson::JsonFormatting::FORMAT);
        writer.beginObject();
        writer.key("metadata");
        writeMetadata(writer, grid);
        writeElevations(writer, grid.elevations(), grid.unknownElevation());
        writer.endObject();
    }
}
