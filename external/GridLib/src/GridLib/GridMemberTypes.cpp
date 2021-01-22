//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-20.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "GridLib/GridMemberTypes.hpp"

namespace GridLib
{
    #define TO_STRING(name) \
        case Unit::name: return #name

    const char* toString(Unit unit)
    {
        switch (unit)
        {
        TO_STRING(FEET);
        TO_STRING(METERS);
        TO_STRING(ARC_SECONDS);
        default:
            return "UNDEFINED";
        }
    }
}
