//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-11-15.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "GridLib/Grid.hpp"

namespace GridLib
{
    Grid::Grid() = default;

    Grid::Grid(size_t rows, size_t columns)
        : m_Grid(rows, columns),
          m_Unknown(rows, columns)
    {}

    void Grid::clear()
    {
        m_Grid.fill(0);
        m_Unknown.fill(false);
    }

    void Grid::resize(size_t rows, size_t columns)
    {
        m_Grid.resize(rows, columns);
        m_Unknown.resize(rows, columns);
    }

    ArrayView2D<double> Grid::elevations() const
    {
        return m_Grid;
    }

    MutableArrayView2D<double> Grid::elevations()
    {
        return MutableArrayView2D(m_Grid.rows(), m_Grid.columns(),
                                  m_Grid.data());
    }

    BitArrayView2D Grid::unknownElevations() const
    {
        return m_Unknown;
    }

    MutableBitArrayView2D Grid::unknownElevations()
    {
        return MutableBitArrayView2D(m_Unknown.rows(),
                                     m_Unknown.columns(),
                                     m_Unknown.data());
    }

    const Axis& Grid::rowAxis() const
    {
        return m_Axis[0];
    }

    Grid& Grid::setRowAxis(const Axis& axis)
    {
        m_Axis[0] = axis;
        return *this;
    }

    const Axis& Grid::columnAxis() const
    {
        return m_Axis[1];
    }

    Grid& Grid::setColumnAxis(const Axis& axis)
    {
        m_Axis[1] = axis;
        return *this;
    }

    const Axis& Grid::verticalAxis() const
    {
        return m_Axis[2];
    }

    Grid& Grid::setVerticalAxis(const Axis& axis)
    {
        m_Axis[2] = axis;
        return *this;
    }

    const Coords& Grid::location() const
    {
        return m_Location;
    }

    Grid& Grid::setLocation(const Coords& coords)
    {
        m_Location = coords;
        return *this;
    }

    double Grid::rotationAngle() const
    {
        return m_RotationAngle;
    }

    Grid& Grid::setRotationAngle(double angle)
    {
        m_RotationAngle = angle;
        return *this;
    }

    RotationDir Grid::axisOrientation() const
    {
        return m_AxisOrientation;
    }

    Grid& Grid::setAxisOrientation(RotationDir dir)
    {
        m_AxisOrientation = dir;
        return *this;
    }

    const ReferenceSystem& Grid::referenceSystem() const
    {
        return m_ReferenceSystem;
    }

    Grid& Grid::setReferenceSystem(const ReferenceSystem& system)
    {
        m_ReferenceSystem = system;
        return *this;
    }

    std::pair<Array2D<double>, BitArray2D> Grid::release()
    {
        std::pair<Array2D<double>, BitArray2D> p = {
            std::move(m_Grid), std::move(m_Unknown)};
        return p;
    }
}
