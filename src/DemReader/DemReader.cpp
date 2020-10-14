//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-10-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "DemReader/DemReader.hpp"
#include "RecordA.hpp"
#include "RecordB.hpp"
#include "RecordC.hpp"
#include "DemRecordIterator.hpp"
#include "DemReader/DemException.hpp"

namespace DemReader
{
    struct DemReader::Data
    {
        explicit Data(std::istream& stream)
            : iterator(stream)
        {}

        DemRecordIterator iterator;
        RecordA recordA;
        RecordC recordC;
        double zFactor = 0.0;
    };

    DemReader::DemReader(std::istream& stream)
        : m_Data(std::make_unique<Data>(stream))
    {
        if (auto a = m_Data->iterator.read_record_a())
            m_Data->recordA = *a;
        else
            DEM_THROW("The stream doesn't contain a valid record type A.");

        if (m_Data->recordA.data_validation_flag.value_or(0) != 0)
        {
            if (auto c = m_Data->iterator.read_record_c())
                m_Data->recordC = *c;
            m_Data->iterator.move_to_first_record_b();
        }
        m_Data->zFactor = m_Data->recordA.spatial_resolution[2].value_or(1.0);
        if (m_Data->recordA.vertical_unit.value_or(2) == 1)
            m_Data->zFactor *= 0.3048;
    }

    DemReader::DemReader(DemReader&& rhs) noexcept = default;

    DemReader::~DemReader() = default;

    DemReader& DemReader::operator=(DemReader&& rhs) noexcept = default;

    std::optional<ElevationData> DemReader::read_next()
    {
        auto b = m_Data->iterator.next_record_b();
        if (!b)
            return {};

        std::vector<double> elevations;
        auto offset = double(b->elevation_base);
        elevations.reserve(b->elevations.size());
        std::transform(b->elevations.begin(), b->elevations.end(),
                       back_inserter(elevations),
                       [&](auto& e){return offset + e * m_Data->zFactor;});
        return {{{b->row, b->column, b->rows, b->columns}, move(elevations)}};
    }
}
