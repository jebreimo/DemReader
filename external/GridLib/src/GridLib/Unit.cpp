//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-11-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "GridLib/Unit.hpp"

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
        default: return "UNDEFINED";
        }
    }
}
