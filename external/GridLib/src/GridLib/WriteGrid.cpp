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
        void writeAxis(Yson::JsonWriter& writer, const Axis& axis)
        {
            writer.beginObject();
            writer.key("unit").value(toString(axis.unit));
            writer.key("resolution").value(axis.resolution);
            writer.endObject();
        }

        void writeMetadata(Yson::JsonWriter& writer, const Grid& grid)
        {
            writer.beginObject();
            writer.key("row axis");
            writeAxis(writer, grid.rowAxis());
            writer.key("column axis");
            writeAxis(writer, grid.columnAxis());
            writer.key("vertical axis");
            writeAxis(writer, grid.verticalAxis());
            writer.key("rotation angle").value(grid.rotationAngle());
            writer.endObject();
        }
    }

    void writeAsJson(std::ostream& stream, const Grid& grid)
    {
        Yson::JsonWriter writer(stream, Yson::JsonFormatting::FORMAT);
        writer.beginObject();
        writeMetadata(writer, grid);
        writer.endObject();
    }
}
