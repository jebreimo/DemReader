//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-09-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "DemRecordIterator.hpp"

#include <istream>
#include <ostream>
#include "DemReader/DemException.hpp"
#include "FortranReader.hpp"

namespace DemReader
{
    struct DemRecordIterator::Data
    {
        explicit Data(std::istream& stream)
            : reader(stream)
        {}

        FortranReader reader;
        bool hasReadRecordA = false;
        bool lookForRecordC = false;
    };

    DemRecordIterator::DemRecordIterator() = default;

    DemRecordIterator::DemRecordIterator(std::istream& stream)
        : m_Data(std::make_unique<Data>(stream))
    {
        //m_Data->recordA = read_RecordA(m_Data->reader);
    }

    DemRecordIterator::DemRecordIterator(DemRecordIterator&& rhs) noexcept
        : m_Data(move(rhs.m_Data))
    {}

    DemRecordIterator::~DemRecordIterator() = default;

    DemRecordIterator& DemRecordIterator::operator=(DemRecordIterator&& rhs) noexcept
    {
        m_Data = move(rhs.m_Data);
        return *this;
    }

    std::optional<RecordA> DemRecordIterator::read_record_a()
    {
        if (!m_Data)
            DEM_THROW("No input stream.");

        if (m_Data->hasReadRecordA)
            m_Data->reader.seek(0, std::ios_base::beg);

        if (!m_Data->reader.fill_buffer(1024))
            return {};

        auto recA = DemReader::read_record_a(m_Data->reader);
        m_Data->hasReadRecordA = true;
        m_Data->lookForRecordC = recA.data_validation_flag.value_or(0) != 0;
        return recA;
    }

    std::optional<RecordC> DemRecordIterator::read_record_c()
    {
        if (!m_Data)
            DEM_THROW("No input stream.");

        if (!m_Data->hasReadRecordA)
            DEM_THROW("Must read record a before b or c.");

        m_Data->reader.seek(-1024, std::ios_base::end);

        if (!m_Data->reader.fill_buffer(1024))
            return {};

        auto recC = DemReader::read_record_c(m_Data->reader);
        return recC;
    }

    void DemRecordIterator::move_to_first_record_b()
    {
        m_Data->reader.seek(1024, std::ios_base::beg);
    }

    std::optional<RecordB> DemRecordIterator::next_record_b()
    {
        if (!m_Data)
            DEM_THROW("No input stream.");

        if (!m_Data->hasReadRecordA)
            DEM_THROW("Must read record a before b or c.");

        if (!m_Data->reader.fill_buffer(1024))
            return {};

        if (m_Data->lookForRecordC
            && m_Data->reader.fill_buffer(2048)
            && m_Data->reader.remaining_buffer_size() == 1024)
        {
            // We've reached the final 1024 bytes of the file, which contains
            // record type c.
            return {};
        }

        return read_record_b(m_Data->reader);
    }
}
