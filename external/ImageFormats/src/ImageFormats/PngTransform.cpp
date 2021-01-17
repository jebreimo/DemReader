//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "ImageFormats/PngTransform.hpp"

namespace ImageFormats
{
    bool PngTransform::invertAlpha() const
    {
        return m_InvertAlpha;
    }

    PngTransform& PngTransform::invertAlpha(bool value)
    {
        m_InvertAlpha = value;
        return *this;
    }

    const std::optional<uint32_t>& PngTransform::pixelFiller() const
    {
        return m_PixelFiller;
    }

    PngTransform& PngTransform::pixelFiller(std::optional<uint32_t> filler)
    {
        m_PixelFiller = filler;
        return *this;
    }

    bool PngTransform::pixelPacking() const
    {
        return false;
    }

    PngTransform& PngTransform::pixelPacking(bool value)
    {
        m_PixelPacking = value;
        return *this;
    }
}
