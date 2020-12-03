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
    class Array2D
    {
    public:
        typedef typename std::vector<T>::iterator iterator;
        typedef typename std::vector<T>::const_iterator const_iterator;

        Array2D() = default;

        Array2D(size_t rows, size_t columns)
            : m_Buffer(rows * columns),
              m_Size(rows, columns)
        {}

        Array2D(size_t rows, size_t columns, const T* values)
            : m_Buffer(values, values + rows * columns),
              m_Size(rows, columns)
        {}

        Array2D(size_t rows, size_t columns, std::vector<T> values)
            : m_Buffer(std::move(values)),
              m_Size(rows, columns)
        {
            if (size() != m_Buffer.size())
                throw std::runtime_error("Array2D has incorrect size.");
        }

        operator ArrayView2D<T>() const noexcept
        {
            return ArrayView2D<T>(rows(), columns(), data());
        }

        const T& operator()(size_t row, size_t column) const noexcept
        {
            return m_Buffer[row * columns() + column];
        }

        T& operator()(size_t row, size_t column) noexcept
        {
            return m_Buffer[row * columns() + column];
        }

        const T* data() const noexcept
        {
            return m_Buffer.data();
        }

        T* data() noexcept
        {
            return m_Buffer.data();
        }

        [[nodiscard]]
        bool empty() const noexcept
        {
            return m_Buffer.empty();
        }

        [[nodiscard]]
        size_t rows() const noexcept
        {
            return m_Size.first;
        }

        [[nodiscard]]
        size_t columns() const noexcept
        {
            return m_Size.second;
        }

        [[nodiscard]]
        size_t size() const noexcept
        {
            return m_Size.first * m_Size.second;
        }

        void resize(size_t rows, size_t columns)
        {
            auto oldSize = size();
            auto oldRows = this->rows();
            auto oldColumns = this->columns();
            auto newSize = rows * columns;
            m_Buffer.resize(newSize);
            m_Size = {rows, columns};

            if (oldRows == 0)
                return;

            if (columns > oldColumns)
            {
                auto src = m_Buffer.data() + oldSize - 1;
                auto dst = m_Buffer.data() + (oldRows - 1) * columns + oldColumns - 1;
                while (src != dst)
                {
                    for (size_t i = 0; i < oldColumns; ++i)
                        *dst-- = *src--;
                    for (size_t i = 0; i < columns - oldColumns; ++i)
                        *dst-- = 0;
                }
            }
            else if (columns < oldColumns)
            {
                auto src = m_Buffer.data() + oldColumns;
                auto dst = m_Buffer.data() + columns;
                for (size_t i = 0; i < oldRows - 1; ++i)
                {
                    for (size_t j = 0; j < columns; ++j)
                        *dst++ = *src++;
                    src += oldColumns - columns;
                }
            }
        }

        iterator begin() noexcept
        {
            return m_Buffer.begin();
        }

        const_iterator begin() const noexcept
        {
            return m_Buffer.begin();
        }

        iterator end() noexcept
        {
            return m_Buffer.end();
        }

        const_iterator end() const noexcept
        {
            return m_Buffer.end();
        }

        [[nodiscard]]
        std::vector<T> release()
        {
            m_Size = {};
            auto tmp = std::move(m_Buffer);
            return std::move(tmp);
        }

        void fill(const T& value)
        {
            std::fill(m_Buffer.begin(), m_Buffer.end(), value);
        }
    private:
        std::vector<T> m_Buffer;
        std::pair<size_t, size_t> m_Size;
    };

    template <typename T>
    bool operator==(const Array2D<T>& a, const Array2D<T>& b)
    {
        return a.rows() == b.rows()
               && a.columns() == b.columns()
               && std::equal(a.begin(), a.end(), b.begin());
    }

    template <typename T>
    bool operator!=(const Array2D<T>& a, const Array2D<T>& b)
    {
        return !(a == b);
    }
}
