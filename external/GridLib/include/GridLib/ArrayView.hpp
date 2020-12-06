//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-12-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstddef>

namespace GridLib
{
    template <typename T>
    class ArrayView
    {
    public:
        constexpr ArrayView() = default;

        constexpr ArrayView(const T* data, size_t size) noexcept
            : m_Data(data),
              m_Size(size)
        {}

        [[nodiscard]]
        const T& operator[](size_t i) const
        {
            return m_Data[i];
        }

        [[nodiscard]]
        const T* begin() const
        {
            return m_Data;
        }

        [[nodiscard]]
        const T* end() const
        {
            return m_Data + m_Size;
        }

        [[nodiscard]]
        size_t size() const
        {
            return m_Size;
        }

        [[nodiscard]]
        const T* data() const
        {
            return m_Data;
        }
    private:
        const T* m_Data = nullptr;
        size_t m_Size = 0;
    };
}
