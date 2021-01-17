//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <list>
#include <optional>
#include <string>
#include <vector>
#include <png.h>

namespace ImageFormats
{
    class PngInfo
    {
    public:
        [[nodiscard]]
        unsigned width() const;

        PngInfo& width(unsigned value);

        [[nodiscard]]
        unsigned height() const;

        PngInfo& height(unsigned value);

        [[nodiscard]]
        int bitDepth() const;

        PngInfo& bitDepth(int value);

        [[nodiscard]]
        int colorType() const;

        PngInfo& colorType(int value);

        [[nodiscard]]
        int interlaceType() const;

        PngInfo& interlaceType(int value);

        [[nodiscard]]
        int compressionMethod() const;

        PngInfo& compressionMethod(int value);

        [[nodiscard]]
        int filterMethod() const;

        PngInfo& filterMethod(int value);

        [[nodiscard]]
        const std::optional<double>& gamma() const;

        PngInfo& gamma(std::optional<double> gamma);

        using Text = std::tuple<std::string, std::string, int>;

        [[nodiscard]]
        const std::vector<png_text>& texts() const;

        PngInfo& addText(std::string key, std::string value);

        PngInfo& addText(std::string key, std::string value, int compression);

        [[nodiscard]]
        const std::optional<std::pair<unsigned, unsigned>>& pixelsPerMeter() const;

        PngInfo& pixelsPerMeter(unsigned value);

        PngInfo& pixelsPerMeter(unsigned horValue, unsigned verValue);
    private:
        unsigned m_Width = 0;
        unsigned m_Height = 0;
        int m_BitDepth = 8;
        int m_ColorType = PNG_COLOR_TYPE_RGB_ALPHA;
        int m_InterlaceType = PNG_INTERLACE_NONE;
        int m_CompressionMethod = PNG_COMPRESSION_TYPE_DEFAULT;
        int m_FilterMethod = PNG_FILTER_TYPE_DEFAULT;
        std::optional<double> m_Gamma;
        std::vector<png_text> m_Texts;
        std::list<std::string> m_Strings;
        std::optional<std::pair<unsigned, unsigned>> m_PixelsPerMeter;
    };
}
