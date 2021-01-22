//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-20.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "GridLib/GridView.hpp"
#include "GridLib/Grid.hpp"
#include "GridLib/GridLibException.hpp"

namespace GridLib
{
    GridView::GridView() noexcept = default;

    GridView::GridView(const Grid& grid) noexcept
        : GridView(grid,
                   grid.elevations(),
                   grid.sphericalCoords(),
                   grid.planarCoords())
    {}

    GridView::GridView(const Grid& grid,
                       Chorasmia::ArrayView2D<double> elevations,
                       std::optional<SphericalCoords> sphericalCoords,
                       std::optional<PlanarCoords> planarCoords) noexcept
        : m_Grid(&grid),
          m_Elevations(elevations),
          m_SphericalCoords(move(sphericalCoords)),
          m_PlanarCoords(move(planarCoords))
    {}

    size_t GridView::rowCount() const
    {
        return m_Elevations.rowCount();
    }

    size_t GridView::columnCount() const
    {
        return m_Elevations.columnCount();
    }

    Chorasmia::ArrayView2D<double> GridView::elevations() const
    {
        return m_Elevations;
    }

    std::optional<double> GridView::unknownElevation() const
    {
        assertGrid();
        return m_Grid->unknownElevation();
    }

    const Axis& GridView::rowAxis() const
    {
        assertGrid();
        return m_Grid->rowAxis();
    }

    const Axis& GridView::columnAxis() const
    {
        assertGrid();
        return m_Grid->columnAxis();
    }

    const Axis& GridView::verticalAxis() const
    {
        assertGrid();
        return m_Grid->verticalAxis();
    }

    const std::optional<SphericalCoords>& GridView::sphericalCoords() const
    {
        return m_SphericalCoords;
    }

    const std::optional<PlanarCoords>& GridView::planarCoords() const
    {
        return m_PlanarCoords;
    }

    double GridView::rotationAngle() const
    {
        assertGrid();
        return m_Grid->rotationAngle();
    }

    RotationDir GridView::axisOrientation() const
    {
        assertGrid();
        return m_Grid->axisOrientation();
    }

    const std::optional<ReferenceSystem>& GridView::referenceSystem() const
    {
        assertGrid();
        return m_Grid->referenceSystem();
    }

    const Grid* GridView::baseGrid() const
    {
        return m_Grid;
    }

    void GridView::assertGrid() const
    {
        if (!m_Grid)
            GRIDLIB_THROW("grid is NULL");
    }
}