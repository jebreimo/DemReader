//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-10-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <iosfwd>
#include <vector>
#include <optional>

namespace DemReader
{
    struct DemPosition
    {
        int16_t row;
        int16_t column;
        int16_t rows;
        int16_t columns;
    };

    struct ElevationData
    {
        DemPosition position;
        std::vector<double> elevations;
    };

    class DemReader
    {
    public:
        DemReader(std::istream& stream);

        DemReader(DemReader&& rhs) noexcept;

        ~DemReader();

        DemReader& operator=(DemReader&& rhs) noexcept;

        std::optional<ElevationData> read_next();

    private:
        struct Data;
        std::unique_ptr<Data> m_Data;
    };
}
