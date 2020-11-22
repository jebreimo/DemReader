//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-26.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <vector>
#include "BitArrayView2D.hpp"

namespace GridLib
{
    class BitArray2D
    {
    public:
        BitArray2D() = default;

        BitArray2D(unsigned rows, unsigned columns)
            : m_Bits((rows * columns + 31) / 32),
              m_Size(rows, columns)
        {}

        [[nodiscard]]
        operator BitArrayView2D() const
        {
            return BitArrayView2D(m_Size.first, m_Size.second, m_Bits.data());
        }

        [[nodiscard]]
        bool empty() const
        {
            return m_Size == std::pair(size_t(0), size_t(0));
        }

        [[nodiscard]]
        size_t size() const
        {
            return m_Size.first * m_Size.second;
        }

        [[nodiscard]]
        size_t rows() const
        {
            return m_Size.first;
        }

        [[nodiscard]]
        size_t columns() const
        {
            return m_Size.second;
        }

        void resize(size_t rows, size_t columns)
        {
            m_Bits.resize((rows * columns + 31) / 32);
            m_Size = {rows, columns};
        }

        [[nodiscard]]
        bool get(size_t row, size_t column) const
        {
            auto i = row * columns() + column;
            auto iWord = i / 32;
            auto iBit = i % 32;
            return (m_Bits[iWord] & (1u << iBit)) != 0;
        }

        void set(size_t row, size_t column, bool value)
        {
            auto i = row * columns() + column;
            auto iWord = i / 32;
            auto iBit = i % 32;
            if (value)
                m_Bits[iWord] |= (1u << iBit);
            else
                m_Bits[iWord] &= ~(1u << iBit);
        }

        void fill(bool value)
        {
            auto v = value ? ~uint32_t(0) : uint32_t(0);
            std::fill(m_Bits.begin(), m_Bits.end(), v);
        }

        [[nodiscard]]
        const uint32_t* data() const
        {
            return m_Bits.data();
        }

        [[nodiscard]]
        uint32_t* data()
        {
            return m_Bits.data();
        }
    private:
        std::vector<uint32_t> m_Bits;
        std::pair <size_t, size_t> m_Size;
    };
}
