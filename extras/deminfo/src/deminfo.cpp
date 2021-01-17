//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-09-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <iostream>
#include <fstream>
#include <Argos/Argos.hpp>
#include <DemReader/DemReader.hpp>

struct RecordBStats
{
    unsigned count = 0;
    unsigned max_row = 0;
    unsigned max_column = 0;
    unsigned missing = 0;
};

int main(int argc, char* argv[])
{
    using namespace Argos;
    auto args = ArgumentParser(argv[0], true)
        .add(Argument("FILE"))
        .parse(argc, argv);

    std::ifstream file(args.value("FILE").asString());
    if (!file)
        args.value("FILE").error("no such file!");

    std::ios::sync_with_stdio(false);

    try
    {
        Dem::DemReader reader(file);
        const auto& a = reader.record_a();
        print(a, std::cout);
        RecordBStats stats;
        for (auto b = reader.next_record_b(); b; b = reader.next_record_b())
        {
            stats.count++;
            stats.max_row = std::max(stats.max_row, unsigned(b->row + b->rows - 1));
            stats.max_column = std::max(stats.max_column, unsigned(b->column + b->columns - 1));
            stats.missing = std::count(b->elevations.begin(), b->elevations.end(), -32767);
            std::cout << "\r" << stats.count << std::flush;
        }
        std::cout << '\r'
                  << "instances of record B: " << stats.count << '\n'
                  << "rowCount: " << stats.max_row << '\n'
                  << "columnCount: " << stats.max_column << '\n'
                  << "number of missing elevations: " << stats.missing << '\n';
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << "\n";
    }
    return 0;
}
