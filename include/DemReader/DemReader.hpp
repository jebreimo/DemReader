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
#include "DemFileInfo.hpp"

namespace DemReader
{
    struct GridPosition
    {
        int16_t row = 0;
        int16_t column = 0;
    };

    struct GridExtent
    {
        GridPosition position;
        GridSize size;
    };

    struct ElevationData
    {
        GridExtent extent;
        double offset;
        double factor;
        std::vector<int32_t> elevations;
    };

    class DemReader
    {
    public:
        DemReader(std::istream& stream);

        DemReader(DemReader&& rhs) noexcept;

        ~DemReader();

        DemReader& operator=(DemReader&& rhs) noexcept;

        DemFileInfo file_info() const;

        std::optional<ElevationData> read_next();
    private:
        struct Data;
        std::unique_ptr<Data> m_Data;
    };
}
