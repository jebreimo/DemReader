//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <optional>

namespace ImageFormats
{
    class PngTransform
    {
    public:
        [[nodiscard]]
        bool invertAlpha() const;

        PngTransform& invertAlpha(bool value);

        [[nodiscard]]
        const std::optional<uint32_t>& pixelFiller() const;

        PngTransform& pixelFiller(std::optional<uint32_t> filler);

        [[nodiscard]]
        bool pixelPacking() const;

        PngTransform& pixelPacking(bool value);
    private:
        std::optional <uint32_t> m_PixelFiller;
        bool m_InvertAlpha = false;
        bool m_PixelPacking = false;
    };
}
