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
#include "ArrayView2D.hpp"

namespace GridLib
{
    template <typename T>
    class MutableArrayView2D
    {
    public:
        using MutableIterator = RowIterator<T, true>;
        using ConstIterator = RowIterator<T>;

        MutableArrayView2D() = default;

        MutableArrayView2D(size_t rows, size_t columns, T* values)
            : m_Data(values),
              m_Size(rows, columns)
        {}

        constexpr operator ArrayView<T>() const noexcept
        {
            return {m_Data, size()};
        }

        constexpr operator MutableArrayView<T>() noexcept
        {
            return {m_Data, size()};
        }

        constexpr operator ArrayView2D<T>() const noexcept
        {
            return {m_Size.first, m_Size.second, m_Data};
        }

        constexpr MutableArrayView<T> operator[](size_t row)
        {
            return {m_Data + row * m_Size.second, m_Size.second};
        }

        constexpr ArrayView<T> operator[](size_t row) const
        {
            return {m_Data + row * m_Size.second, m_Size.second};
        }

        const T& operator()(size_t row, size_t column) const
        {
            return m_Data[row * columnCount() + column];
        }

        T& operator()(size_t row, size_t column)
        {
            return m_Data[row * columnCount() + column];
        }

        const T* data() const
        {
            return m_Data;
        }

        T* data()
        {
            return m_Data;
        }

        [[nodiscard]]
        bool empty() const
        {
            return m_Size == std::pair(size_t(0), size_t(0));
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
        MutableIterator begin()
        {
            return {m_Data, m_Size.second};
        }

        [[nodiscard]]
        ConstIterator begin() const
        {
            return {m_Data, m_Size.second};
        }

        [[nodiscard]]
        MutableIterator end()
        {
            return {m_Data + size(), m_Size.second};
        }

        [[nodiscard]]
        ConstIterator end() const
        {
            return {m_Data + size(), m_Size.second};
        }

        friend bool
        operator==(const MutableArrayView2D& a, const MutableArrayView2D& b)
        {
            return a.m_Size == b.m_Size
                   && (a.m_Data == b.m_Data
                       || std::equal(a.data(), a.data() + a.size()), b.data());
        }

        friend bool
        operator!=(const MutableArrayView2D& a, const MutableArrayView2D& b)
        {
            return !(a == b);
        }
    private:
        T* m_Data = nullptr;
        std::pair<size_t, size_t> m_Size;
    };
}
