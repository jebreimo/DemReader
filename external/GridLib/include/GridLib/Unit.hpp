//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-11-13.
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
}