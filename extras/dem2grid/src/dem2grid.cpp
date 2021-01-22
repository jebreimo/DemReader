//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-10-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <iostream>
#include <fstream>
#include <Argos/Argos.hpp>
//#include <Yson/JsonWriter.hpp>
#include <DemReader/ReadDemGrid.hpp>
#include "GridLib/WriteGrid.hpp"

int main(int argc, char* argv[])
{
    using namespace Argos;
    auto args = ArgumentParser(argv[0], true)
        .allowAbbreviatedOptions(true)
        .add(Argument("FILE").text("The name of the DEM file."))
        .add(Argument("OUTPUT").optional(true)
                 .text("The name of the output file. Output is written to"
                       " stdout if this is not given."))
        .add(Option{"-p", "--position"}.argument("ROW,COL")
                 .text("Set the bottom left row and column in the input file"
                       " that will be processed. Defaults to 0, 0."))
        .add(Option{"-s", "--size"}.argument("ROWS,COLS")
                 .text("The number of rowCount and columnCount in the input file that"
                       " will be processed. Values that are too large are"
                       " adjusted automatically. Defaults to everything."))
        .add(Option{"-u", "--unit"}.argument("UNIT")
                 .text("Set the units used in the output: 'm' for meters,"
                       " 'f' for feet, and 'r' for 'raw', i.e. same as input."
                       " Default is meters."))
        .parse(argc, argv);

    auto size = args.value("--size").split(',', 2, 2).asUInts({UINT_MAX, UINT_MAX});
    auto position = args.value("--position").split(',', 2, 2).asUInts({0, 0});
    auto unitStr = args.value("--unit").asString("m");
    if (unitStr != "m" && unitStr != "f" && unitStr != "r")
        args.value("--unit").error();

    std::ifstream file(args.value("FILE").asString());
    if (!file)
        args.value("FILE").error("no such file!");

    try
    {
        auto grid = Dem::read_dem_grid(
            file, GridLib::Unit::METERS,
            [](size_t step, size_t steps)
            {
                std::cerr << "\r" << step << " of " << steps;
                return true;
            });
        std::cout << "\n";
        if (args.has("OUTPUT"))
            GridLib::writeAsJson(args.value("OUTPUT").asString(), grid);
        else
            GridLib::writeAsJson(std::cout, grid);
        std::cout << std::endl;
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << "\n";
    }
    return 0;
}
