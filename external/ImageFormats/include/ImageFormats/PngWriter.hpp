//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <string>
#include <iosfwd>
#include "PngInfo.hpp"
#include "PngTransform.hpp"

namespace ImageFormats
{
    class PngWriter
    {
    public:
        PngWriter();

        PngWriter(std::ostream& stream, PngInfo info, PngTransform transform);

        PngWriter(PngWriter&& obj) noexcept;

        ~PngWriter();

        PngWriter& operator=(PngWriter&& obj) noexcept;

        explicit operator bool() const;

        void writeInfo();

        void write(const void* image, size_t size);

        void writeRows(const void* rows[], uint32_t count, size_t rowSize);

        void writeRow(const void* row, size_t size);

        void writeEnd();
    private:
        void assertIsValid() const;

        PngInfo m_Info;
        PngTransform m_Transform;
        png_structp m_PngPtr;
        png_infop m_InfoPtr;
    };

    void writePng(std::ostream& stream,
                  const void* image, size_t imageSize,
                  PngInfo options, PngTransform transform);

    void writePng(const std::string& fileName,
                  const void* image, size_t imageSize,
                  PngInfo info, PngTransform transform);
}
