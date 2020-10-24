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

int main(int argc, char* argv[])
{
    using namespace Argos;
    auto args = ArgumentParser(argv[0], true)
        .add(Argument("FILE"))
        .parse(argc, argv);

    std::ifstream file(args.value("FILE").asString());
    if (!file)
        args.value("FILE").error("no such file!");
    try
    {
        DemReader::DemReader reader(file);

        for (auto r = reader.read_next(); r; r = reader.read_next())
        {
            std::cout << r->position.row << ", " << r->position.column << '\n';
        }
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << "\n";
    }
    return 0;
}
