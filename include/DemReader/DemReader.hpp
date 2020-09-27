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
#include "RecordA.hpp"

namespace DemReader
{
    class DemReader
    {
    public:
        DemReader();

        explicit DemReader(std::istream& stream);

        DemReader(DemReader&& rhs) noexcept;

        ~DemReader();

        DemReader& operator=(DemReader&& rhs) noexcept;

        [[nodiscard]]
        const RecordA& recordA() const;
    private:
        struct Data;
        std::unique_ptr<Data> m_Data;
    };
}
