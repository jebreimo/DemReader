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

namespace GridLib
{
    template <typename T>
    class ArrayView2D
    {
    public:
        typedef T* iterator;
        typedef const T* const_iterator;

        constexpr ArrayView2D() = default;

        constexpr ArrayView2D(size_t rows, size_t columns, const T* values)
            : m_Values(values),
              m_Size(rows, columns)
        {}

        const T& operator()(size_t row, size_t column) const
        {
            return m_Values[row * columns() + column];
        }

        const T* data() const
        {
            return m_Values;
        }

        [[nodiscard]]
        bool empty() const
        {
            return m_Size == std::pair(size_t(0), size_t(0));
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

        [[nodiscard]]
        size_t size() const
        {
            return m_Size.first * m_Size.second;
        }

        [[nodiscard]]
        iterator begin()
        {
            return m_Values;
        }

        [[nodiscard]]
        const_iterator begin() const
        {
            return m_Values;
        }

        iterator end()
        {
            return m_Values + size();
        }

        const_iterator end() const
        {
            return m_Values + size();
        }

    private:
        const T* m_Values = nullptr;
        std::pair<size_t, size_t> m_Size;
    };

    template <typename T>
    bool operator==(const ArrayView2D<T>& a, const ArrayView2D<T>& b)
    {
        return a.rows() == b.rows()
               && a.columns() == b.columns()
               && std::equal(a.begin(), a.end(), b.begin());
    }

    template <typename T>
    bool operator!=(const ArrayView2D<T>& a, const ArrayView2D<T>& b)
    {
        return !(a == b);
    }
}
