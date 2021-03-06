//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <iostream>
#include <filesystem>
#include <fstream>
#include <Argos/Argos.hpp>
#include <DemReader/ReadDemGrid.hpp>
#include <fmt/format.h>
#include "GridLib/WriteGrid.hpp"
#include <ImageFormats/PngWriter.hpp>

struct RGBA
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};

RGBA determineColor(double elevation)
{
    constexpr RGBA COLORS[] = {
        {0xAE, 0xD1, 0xFC, 0xFF},

        {0x7C, 0x90, 0x74, 0xFF},
        {0x8A, 0xA0, 0x81, 0xFF},
        {0x99, 0xB1, 0x90, 0xFF},
        {0xAB, 0xC3, 0xA1, 0xFF},
        {0xBF, 0xD6, 0xB6, 0xFF},
        {0xD7, 0xE8, 0xD0, 0xFF},

        {0xE4, 0xCF, 0xC6, 0xFF},
        {0xD2, 0xB8, 0xAE, 0xFF},
        {0xBF, 0xA5, 0x9A, 0xFF},
        {0xAE, 0x94, 0x89, 0xFF},
        {0x9D, 0x85, 0x7B, 0xFF},
        {0x8C, 0x78, 0x6F, 0xFF},

        {0x90, 0x80, 0x69, 0xFF},
        {0xA1, 0x8E, 0x73, 0xFF},
        {0xB3, 0x9E, 0x80, 0xFF},
        {0xC5, 0xAF, 0x90, 0xFF},
        {0xD7, 0xC2, 0xA4, 0xFF},
        {0xE8, 0xD6, 0xBD, 0xFF}
    };

    auto index = std::max(0, (int(elevation) + 24) / 25);
    if (index < std::size(COLORS))
        return COLORS[index];
    return {0xFF, 0xFF, 0xFF, 0xFF};
}

void makePng(const std::string& fileName,
             const Chorasmia::ArrayView2D<double>& grid)
{
    std::cout << fileName << "\n";
    Chorasmia::Array2D<RGBA> bmp(grid.columnCount(), grid.rowCount());
    for (auto i = 0; i < grid.rowCount(); ++i)
    {
        for (auto j = 0; j < grid.columnCount(); ++j)
        {
            auto ii = grid.columnCount() - j - 1;
            bmp(ii, i) = determineColor(grid(i, j));
        }
    }
    ImageFormats::writePng(fileName, bmp.data(), bmp.valueCount() * sizeof(RGBA),
                           ImageFormats::PngInfo()
                               .width(bmp.columnCount())
                               .height(bmp.rowCount()), {});
}

void makeTiles(const GridLib::GridView& grid,
               unsigned rows, unsigned cols,
               const std::string& fileName)
{
    std::filesystem::path path(fileName);
    auto extension = path.extension().string();
    auto prefix = path.replace_extension().string();
    for (size_t i = 0; i < grid.rowCount(); i += rows)
    {
        for (size_t j = 0; j < grid.columnCount(); j += cols)
        {
            makePng(fmt::format("{}_{:04}_{:04}{}",
                                prefix, i, j, extension),
                    grid.elevations().subarray(i, j, rows, cols));
        }
    }
}

int main(int argc, char* argv[])
{
    using namespace Argos;
    auto args = ArgumentParser(argv[0], true)
        .allowAbbreviatedOptions(true)
        .add(Argument("FILE").text("The name of the DEM file."))
        .add(Argument("OUTPUT")
                 .text("The name of the output PNG file(s)."))
        .add(Option{"-p", "--position"}.argument("ROW,COL")
                 .text("Set the bottom left row and column in the input file"
                       " that will be processed. Defaults to 0, 0."))
        .add(Option{"-s", "--size"}.argument("ROWS,COLS")
                 .text("The tile size. Defaults to whichever is smaller of"
                       " the size of the grid and 1024x1024."))
        .parse(argc, argv);

    auto size = args.value("--size").split(',', 2, 2).asUInts({1024, 1024});

    auto inFileName = args.value("FILE").asString();
    std::ifstream file(inFileName);
    if (!file)
        args.value("FILE").error("no such file!");

    auto outFileName = args.value("OUTPUT").asString();

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
        if (args.has("-p"))
        {
            auto pos = args.value("--position").split(',', 2, 2).asUInts();
            makePng(outFileName, grid.elevations().subarray(pos[0], pos[1], size[0], size[1]));
        }
        else
        {
            makeTiles(grid, size[0], size[1], outFileName);
        }
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << "\n";
    }
    return 0;
}
