//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-09-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "DemReader/DemReader.hpp"

#include <istream>
#include <ostream>
#include "FortranReader.hpp"
#include "DemReader/RecordA.hpp"

namespace DemReader
{
    struct DemReader::Data
    {
        Data(std::istream& stream)
            : reader(stream)
        {}

        FortranReader reader;
        RecordA recordA;
    };

    DemReader::DemReader() = default;

    DemReader::DemReader(std::istream& stream)
        : m_Data(std::make_unique<Data>(stream))
    {
        m_Data->recordA = read_RecordA(m_Data->reader);
    }

    DemReader::DemReader(DemReader&& rhs) noexcept
        : m_Data(move(rhs.m_Data))
    {}

    DemReader::~DemReader() = default;

    DemReader& DemReader::operator=(DemReader&& rhs) noexcept
    {
        m_Data = move(rhs.m_Data);
        return *this;
    }

    const RecordA& DemReader::recordA() const
    {
        // TODO: check m_Data
        return m_Data->recordA;
    }
}
