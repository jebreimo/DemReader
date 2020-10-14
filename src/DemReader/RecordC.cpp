//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-10-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************

#include "RecordC.hpp"
#include "FortranReader.hpp"

namespace DemReader
{
    RecordC read_record_c(FortranReader& reader)
    {
        RecordC result;
        result.has_datum_rmse = reader.read_int16(6);
        for (auto& datum_rmse : result.datum_rmse)
            datum_rmse = reader.read_int16(6);
        result.datum_rmse_sample_size = reader.read_int16(6);
        result.has_dem_rmse = reader.read_int16(6);
        for (auto& dem_rmse : result.dem_rmse)
            dem_rmse = reader.read_int16(6);
        result.dem_rmse_sample_size = reader.read_int16(6);
        reader.skip(60);
        return result;
    }
}
