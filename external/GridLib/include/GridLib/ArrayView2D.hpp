//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-01-04.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <stdexcept>
#include <vector>
#include "RowIterator.hpp"

namespace GridLib
{
    template <typename T>
    class ArrayView2D
    {
    public:
        constexpr ArrayView2D() = default;

        constexpr ArrayView2D(size_t rows, size_t columns, const T* data) noexcept
            : m_Data(data),
              m_Size(rows, columns)
        {}

        constexpr operator ArrayView<T>() const noexcept
        {
            return ArrayView<T>(m_Data, size());
        }

        const T& operator()(size_t row, size_t column) const
        {
            return m_Data[row * columnCount() + column];
        }

        const T* data() const
        {
            return m_Data;
        }

        [[nodiscard]]
        bool empty() const
        {
            return m_Size == std::pair(size_t(0), size_t(0));
        }

        ArrayView<T> array() const
        {
            return ArrayView<T>(m_Data, size());
        }
        [[nodiscard]]
        ArrayView<T> row(size_t r) const
        {
            return {m_Data + r * m_Size.second, m_Size.second};
        }

        [[nodiscard]]
        size_t rowCount() const
        {
            return m_Size.first;
        }

        [[nodiscard]]
        size_t columnCount() const
        {
            return m_Size.second;
        }

        [[nodiscard]]
        size_t size() const
        {
            return m_Size.first * m_Size.second;
        }

        [[nodiscard]]
        RowIterator<T> begin() const
        {
            return RowIterator<T>({m_Data, m_Size});
        }

        [[nodiscard]]
        RowIterator<T> end() const
        {
            return RowIterator<T>({m_Data + size(), m_Size.second});
        }
    private:
        const T* m_Data = nullptr;
        std::pair<size_t, size_t> m_Size;
    };

    template <typename T>
    bool operator==(const ArrayView2D<T>& a, const ArrayView2D<T>& b)
    {
        return a.rowCount() == b.rowCount()
               && a.columnCount() == b.columnCount()
               && std::equal(a.begin(), a.end(), b.begin());
    }

    template <typename T>
    bool operator!=(const ArrayView2D<T>& a, const ArrayView2D<T>& b)
    {
        return !(a == b);
    }
}
