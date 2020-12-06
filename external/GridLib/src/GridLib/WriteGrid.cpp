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
            writer.key("rows").value(uint64_t(grid.rows()));
            writer.key("columns").value(uint64_t(grid.columns()));
            writer.key("row_axis");
            writeAxis(writer, grid.rowAxis());
            writer.key("column_axis");
            writeAxis(writer, grid.columnAxis());
            writer.key("vertical_axis");
            writeAxis(writer, grid.verticalAxis());
            writer.key("rotation_angle").value(grid.rotationAngle());
            if (grid.axisOrientation() != RotationDir::COUNTER_CLOCKWISE)
                writer.key("axis_orientation").value("CLOCKWISE");
            writer.endObject();
        }

        void writeElevations(Yson::Writer& writer,
                             const ArrayView2D<double>& values,
                             const BitArrayView2D& unknown)
        {
            writer.beginArray();

            writer.endArray();
        }
    }

    void writeAsJson(std::ostream& stream, const Grid& grid)
    {
        Yson::JsonWriter writer(stream, Yson::JsonFormatting::FORMAT);
        writer.beginObject();
        writer.key("metadata");
        writeMetadata(writer, grid);
        writer.endObject();
    }
}
