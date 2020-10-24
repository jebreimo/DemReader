//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-09-24.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <stdexcept>
#include <string>

/**
 * @file
 * @brief Defines the DemException class.
 */

/**
 * @brief The namespace for all DemRecordIterator classes and functions.
 */
namespace Dem
{
    /**
     * @brief The exception class used throughout DemRecordIterator.
     */
    class DemException : public std::runtime_error
    {
    public:
        DemException() noexcept
            : std::runtime_error("Unspecified error.")
        {}

        /**
         * @brief Passes @a message on to the base class.
         */
        explicit DemException(const std::string& message) noexcept
            : std::runtime_error(message)
        {}

        explicit DemException(const char* message) noexcept
            : std::runtime_error(message)
        {}
    };
}

#define _DEM_THROW_3(file, line, msg) \
    throw ::Dem::DemException(file ":" #line ": " msg)

#define _DEM_THROW_2(file, line, msg) \
    _DEM_THROW_3(file, line, msg)

#define DEM_THROW(msg) \
    _DEM_THROW_2(__FILE__, __LINE__, msg)

#define DEM_THROW_STRING(msg) \
    _DEM_THROW_2(__FILE__, __LINE__, + msg)
