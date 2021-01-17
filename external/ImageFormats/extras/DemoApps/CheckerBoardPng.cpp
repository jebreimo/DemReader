//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-14.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "ImageFormats/PngWriter.hpp"
#include <iostream>

constexpr uint32_t TRANSPARENT = 0;
constexpr uint32_t BLACK = 0xFF000000u;
constexpr uint32_t COLORS[] = {TRANSPARENT, BLACK};
constexpr size_t TILE_SIZE = 32;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <file name>\n";
        return 1;
    }

    std::vector<uint32_t> board;
    board.reserve(TILE_SIZE * 8 * 8);
    auto size = TILE_SIZE * 8;
    for (size_t i = 0; i < size; ++i)
    {
        auto a = (i / TILE_SIZE) % 2;
        for (size_t j = 0; j < size; ++j)
        {
            auto b = (j / TILE_SIZE) % 2;
            board.push_back(COLORS[a ^ b]);
        }
    }

    ImageFormats::writePng(argv[1], board.data(), board.size() * 4,
                           ImageFormats::PngInfo()
                               .width(size)
                               .height(size),
                           {});
    return 0;
}
