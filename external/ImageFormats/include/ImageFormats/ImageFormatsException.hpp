//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <stdexcept>

namespace ImageFormats
{
    class ImageFormatsException : public std::runtime_error
    {
    public:
        explicit ImageFormatsException(const std::string& message) noexcept
            : std::runtime_error(message)
        {}
    };
}

#define IMGFMT_THROW_3_(file, line, msg) \
    throw ::ImageFormats::ImageFormatsException(file ":" #line ": " msg)

#define IMGFMT_THROW_2_(file, line, msg) \
    IMGFMT_THROW_3_(file, line, msg)

#define IMGFMT_THROW(msg) \
    IMGFMT_THROW_2_(__FILE__, __LINE__, msg)
