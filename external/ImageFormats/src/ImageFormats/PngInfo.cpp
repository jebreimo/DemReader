//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "ImageFormats/PngInfo.hpp"
#include "ImageFormats/ImageFormatsException.hpp"

namespace ImageFormats
{
    unsigned PngInfo::width() const
    {
        return m_Width;
    }

    PngInfo& PngInfo::width(unsigned int value)
    {
        m_Width = value;
        return *this;
    }

    unsigned PngInfo::height() const
    {
        return m_Height;
    }

    PngInfo& PngInfo::height(unsigned int value)
    {
        m_Height = value;
        return *this;
    }

    int PngInfo::bitDepth() const
    {
        return m_BitDepth;
    }

    PngInfo& PngInfo::bitDepth(int value)
    {
        m_BitDepth = value;
        return *this;
    }

    int PngInfo::colorType() const
    {
        return m_ColorType;
    }

    PngInfo& PngInfo::colorType(int value)
    {
        m_ColorType = value;
        return *this;
    }

    int PngInfo::interlaceType() const
    {
        return m_InterlaceType;
    }

    PngInfo& PngInfo::interlaceType(int value)
    {
        m_InterlaceType = value;
        return *this;
    }

    int PngInfo::compressionMethod() const
    {
        return m_CompressionMethod;
    }

    PngInfo& PngInfo::compressionMethod(int value)
    {
        m_CompressionMethod = value;
        return *this;
    }

    int PngInfo::filterMethod() const
    {
        return m_FilterMethod;
    }

    PngInfo& PngInfo::filterMethod(int value)
    {
        m_FilterMethod = value;
        return *this;
    }

    const std::optional<double>& PngInfo::gamma() const
    {
        return m_Gamma;
    }

    PngInfo& PngInfo::gamma(std::optional<double> gamma)
    {
        m_Gamma = gamma;
        return *this;
    }

    const std::vector<png_text>& PngInfo::texts() const
    {
        return m_Texts;
    }

    PngInfo& PngInfo::addText(std::string key, std::string value)
    {
        return addText(move(key), move(value), PNG_TEXT_COMPRESSION_NONE);
    }

    PngInfo& PngInfo::addText(std::string key, std::string value, int compression)
    {
        m_Strings.push_back(std::move(key));
        auto keyPtr = &m_Strings.back();
        const std::string* valuePtr = nullptr;
        if (!value.empty())
        {
            m_Strings.push_back(value);
            valuePtr = &m_Strings.back();
        }
        m_Texts.push_back({compression,
                           const_cast<char*>(keyPtr->c_str()),
                           const_cast<char*>(valuePtr ? valuePtr->c_str() : nullptr),
                           valuePtr ? valuePtr->size() : 0,
                           0, nullptr, nullptr});
        return *this;
    }

    const std::optional<std::pair<unsigned, unsigned>>& PngInfo::pixelsPerMeter() const
    {
        return m_PixelsPerMeter;
    }

    PngInfo& PngInfo::pixelsPerMeter(unsigned int value)
    {
        return pixelsPerMeter(value, value);
    }

    PngInfo& PngInfo::pixelsPerMeter(unsigned int horValue, unsigned int verValue)
    {
        m_PixelsPerMeter = {horValue, verValue};
        return *this;
    }
}
