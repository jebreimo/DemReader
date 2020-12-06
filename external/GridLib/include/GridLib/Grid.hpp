//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-11-15.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <variant>
#include "Array2D.hpp"
#include "BitArray2D.hpp"
#include "MutableArrayView2D.hpp"
#include "MutableBitArrayView2D.hpp"
#include "Unit.hpp"

namespace GridLib
{
    struct Axis
    {
        double resolution = {};
        Unit unit = {};
    };

    struct SphericalCoords
    {
        double latitude = 0;
        double longitude = 0;
    };

    struct PlanarCoords
    {
        double northing = 0;
        double easting = 0;
    };

    using Coords = std::variant<PlanarCoords, SphericalCoords>;

    struct ReferenceSystem
    {
        int horizontal = 0;
        int vertical = 0;
    };

    enum class RotationDir
    {
        CLOCKWISE,
        COUNTER_CLOCKWISE
    };

    class Grid
    {
    public:
        Grid();

        Grid(size_t rows, size_t columns);

        void clear();

        [[nodiscard]]
        size_t rows() const;

        [[nodiscard]]
        size_t columns() const;

        void resize(size_t rows, size_t columns);

        [[nodiscard]]
        ArrayView2D<double> elevations() const;

        MutableArrayView2D<double> elevations();

        [[nodiscard]]
        BitArrayView2D unknownElevations() const;

        MutableBitArrayView2D unknownElevations();

        [[nodiscard]]
        const Axis& rowAxis() const;

        Grid& setRowAxis(const Axis& axis);

        [[nodiscard]]
        const Axis& columnAxis() const;

        Grid& setColumnAxis(const Axis& axis);

        [[nodiscard]]
        const Axis& verticalAxis() const;

        Grid& setVerticalAxis(const Axis& axis);

        [[nodiscard]]
        const Coords& location() const;

        Grid& setLocation(const Coords& coords);

        [[nodiscard]]
        double rotationAngle() const;

        /**
         * @brief Set the ccw angle that the row (major) axis has been
         * rotated relative to due east.
         *
         * North is zero degrees.
         * @param angle The counter-clockwise angle from due east in radians.
         */
        Grid& setRotationAngle(double angle);

        [[nodiscard]]
        RotationDir axisOrientation() const;

        /**
         * @brief Set the orientation of the column (minor) axis relative to
         *  the row (major) axis.
         */
        Grid& setAxisOrientation(RotationDir dir);

        [[nodiscard]]
        const ReferenceSystem& referenceSystem() const;

        Grid& setReferenceSystem(const ReferenceSystem& system);

        [[nodiscard]]
        std::pair<Array2D<double>, BitArray2D> release();
    private:
        Array2D<double> m_Grid;
        BitArray2D m_Unknown;
        Axis m_Axis[3];
        Coords m_Location;
        double m_RotationAngle = 0;
        ReferenceSystem m_ReferenceSystem;
        RotationDir m_AxisOrientation = RotationDir::COUNTER_CLOCKWISE;
    };
}
