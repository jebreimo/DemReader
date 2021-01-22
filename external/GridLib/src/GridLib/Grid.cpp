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
        : m_Grid(rows, columns)
    {}

    void Grid::clear()
    {
        m_Grid.fill(0);
    }

    size_t Grid::rowCount() const
    {
        return m_Grid.rowCount();
    }

    size_t Grid::columnCount() const
    {
        return m_Grid.columnCount();
    }

    void Grid::resize(size_t rows, size_t columns)
    {
        m_Grid.resize(rows, columns);
    }

    Chorasmia::ArrayView2D<double> Grid::elevations() const
    {
        return m_Grid;
    }

    Chorasmia::MutableArrayView2D<double> Grid::elevations()
    {
        return {m_Grid.data(), m_Grid.rowCount(), m_Grid.columnCount()};
    }

    std::optional<double> Grid::unknownElevation() const
    {
        return m_UnknownElevation;
    }

    Grid& Grid::setUnknownElevation(std::optional<double> elevation)
    {
        m_UnknownElevation = elevation;
        return *this;
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

    const std::optional<SphericalCoords>& Grid::sphericalCoords() const
    {
        return m_SphericalCoords;
    }

    Grid& Grid::setSphericalCoords(const std::optional<SphericalCoords>& coords)
    {
        m_SphericalCoords = coords;
        return *this;
    }

    const std::optional<PlanarCoords>& Grid::planarCoords() const
    {
        return m_PlanarCoords;
    }

    Grid& Grid::setPlanarCoords(const std::optional<PlanarCoords>& coords)
    {
        m_PlanarCoords = coords;
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

    const std::optional<ReferenceSystem>& Grid::referenceSystem() const
    {
        return m_ReferenceSystem;
    }

    Grid& Grid::setReferenceSystem(std::optional<ReferenceSystem> system)
    {
        m_ReferenceSystem = system;
        return *this;
    }

    GridView Grid::subgrid(size_t row, size_t column,
                           size_t nrows, size_t ncolumns) const
    {
        auto planarCoords = m_PlanarCoords;
        if (planarCoords && (row != 0 || column != 0))
        {
            constexpr double METERS_PER_FOOT = 0.3048;
            std::pair<double, double> dr = {m_Axis[0].resolution, 0};
            std::pair<double, double> dc = {0, m_Axis[1].resolution};
            if (m_Axis[0].unit == Unit::FEET)
                dr.first /= METERS_PER_FOOT;
            if (m_Axis[1].unit == Unit::FEET)
                dc.second /= METERS_PER_FOOT;
            if (m_AxisOrientation == RotationDir::CLOCKWISE)
                dc.second = -dc.second;
            if (m_RotationAngle != 0)
            {
                dr = {dr.first * cos(m_RotationAngle), dr.first * sin(m_RotationAngle)};
                dc = {-dc.second * sin(m_RotationAngle), dc.second * cos(m_RotationAngle)};
            }
            planarCoords->easting += dr.first * row + dc.first * column;
            planarCoords->northing += dr.second * row + dc.second * column;
        }
        auto sphericalCoords = row == 0 && column == 0
                               ? m_SphericalCoords
                               : std::optional<SphericalCoords>();
        return GridView(*this, m_Grid.subarray(row, column, nrows, ncolumns),
                        sphericalCoords, planarCoords);
    }

    Chorasmia::Array2D<double> Grid::release()
    {
        return std::move(m_Grid);
    }
}
