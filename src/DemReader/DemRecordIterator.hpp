//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-09-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <iosfwd>
#include <memory>
#include <variant>
#include "RecordA.hpp"
#include "RecordB.hpp"
#include "RecordC.hpp"

namespace DemReader
{
    using DemRecord = std::variant<std::nullptr_t, RecordA, RecordB, RecordC>;

    class DemRecordIterator
    {
    public:
        DemRecordIterator();

        explicit DemRecordIterator(std::istream& stream);

        DemRecordIterator(DemRecordIterator&& rhs) noexcept;

        ~DemRecordIterator();

        DemRecordIterator& operator=(DemRecordIterator&& rhs) noexcept;

        [[nodiscard]]
        std::optional<RecordA> read_record_a();

        [[nodiscard]]
        std::optional<RecordC> read_record_c();

        void move_to_first_record_b();

        [[nodiscard]]
        std::optional<RecordB> next_record_b();
    private:
        struct Data;
        std::unique_ptr<Data> m_Data;
    };
}
