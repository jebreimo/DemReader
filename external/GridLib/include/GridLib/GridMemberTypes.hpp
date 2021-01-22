//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-20.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

namespace GridLib
{
    enum class Unit
    {
        UNDEFINED = 0,
        FEET = 1,
        METERS = 2,
        ARC_SECONDS = 3
    };

    const char* toString(Unit unit);

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
        double easting = 0;
        double northing = 0;
        int zone = 0;
    };

    struct ReferenceSystem
    {
        int horizontal = 0;
        int vertical = 0;
    };

    enum class RotationDir
    {
        CLOCKWISE,
        COUNTERCLOCKWISE
    };
}
