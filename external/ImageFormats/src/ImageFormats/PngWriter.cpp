//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "ImageFormats/PngWriter.hpp"
#include "ImageFormats/ImageFormatsException.hpp"

#include <ostream>
#include <utility>
#include <fstream>

namespace ImageFormats
{
    namespace
    {
        size_t getPixelComponents(int colorType)
        {
            switch (colorType)
            {
            default:
                return 1;
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                return 2;
            case PNG_COLOR_TYPE_RGB:
                return 3;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                return 4;
            }
        }

        size_t getPixelSize(const PngInfo& info,
                            const PngTransform& transform)
        {
            if (info.bitDepth() == 0)
                IMGFMT_THROW("Bit depth can not be 0.");
            size_t componentCount = getPixelComponents(info.colorType());
            size_t componentSize = info.bitDepth();
            if (componentSize < 8 && componentCount > 1)
                IMGFMT_THROW("Invalid combination of color type and bit depth.");

            if (componentSize < 8)
                return transform.pixelPacking() ? 8 : componentSize;

            if (transform.pixelFiller() && componentCount % 2 == 1)
                componentCount++;
            return componentCount * componentSize;
        }

        size_t getRowSize(const PngInfo& info, const PngTransform& transform)
        {
            return (info.width() * getPixelSize(info, transform) + 7) / 8;
        }

        extern "C" {

        void user_write_data(png_structp png_ptr,
                             png_bytep data,
                             png_size_t length)
        {
            auto stream = static_cast<std::ostream*>(png_get_io_ptr(png_ptr));
            stream->write(reinterpret_cast<const char*>(data), length);
        }

        void user_flush_data(png_structp png_ptr)
        {
            auto stream = static_cast<std::ostream*>(png_get_io_ptr(png_ptr));
            stream->flush();
        }

        }
    }

    PngWriter::PngWriter() = default;

    PngWriter::PngWriter(std::ostream& stream, PngInfo  info, PngTransform transform)
        : m_Info(std::move(info)),
          m_Transform(transform)
    {
        m_PngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!m_PngPtr)
            IMGFMT_THROW("Can not create PNG struct.");
        m_InfoPtr = png_create_info_struct(m_PngPtr);
        if (!m_InfoPtr)
            IMGFMT_THROW("Can not create PNG info struct.");
        png_set_write_fn(m_PngPtr, &stream, user_write_data, user_flush_data);
    }

    PngWriter::PngWriter(PngWriter&& obj) noexcept
        : m_Info(std::move(obj.m_Info)),
          m_Transform(std::move(obj.m_Transform)),
          m_PngPtr(nullptr),
          m_InfoPtr(nullptr)
    {
        std::swap(m_PngPtr, obj.m_PngPtr);
        std::swap(m_InfoPtr, obj.m_InfoPtr);
    }

    PngWriter::~PngWriter()
    {
        if (m_PngPtr)
        {
            png_write_flush(m_PngPtr);
            png_destroy_write_struct(&m_PngPtr, &m_InfoPtr);
        }
    }

    PngWriter& PngWriter::operator=(PngWriter&& obj) noexcept
    {
        if (m_PngPtr)
            png_destroy_write_struct(&m_PngPtr, &m_InfoPtr);
        m_Info = std::move(m_Info);
        m_Transform = std::move(m_Transform);
        std::swap(m_PngPtr, obj.m_PngPtr);
        std::swap(m_InfoPtr, obj.m_InfoPtr);
        return *this;
    }

    PngWriter::operator bool() const
    {
        return m_PngPtr && m_InfoPtr;
    }

    void PngWriter::writeInfo()
    {
        assertIsValid();
        if (setjmp(png_jmpbuf(m_PngPtr)))
        {
            png_destroy_write_struct(&m_PngPtr, &m_InfoPtr);
            IMGFMT_THROW("Error while setting PNG info values.");
        }

        png_set_IHDR(m_PngPtr, m_InfoPtr, m_Info.width(), m_Info.height(),
                     m_Info.bitDepth(), m_Info.colorType(),
                     m_Info.interlaceType(), m_Info.compressionMethod(),
                     m_Info.filterMethod());
        if (m_Info.gamma())
            png_set_gAMA(m_PngPtr, m_InfoPtr, *m_Info.gamma());

        if (!m_Info.texts().empty())
        {
            png_set_text(m_PngPtr, m_InfoPtr,
                         m_Info.texts().data(), m_Info.texts().size());
        }

        if (m_Transform.invertAlpha())
            png_set_invert_alpha(m_PngPtr);

        if (setjmp(png_jmpbuf(m_PngPtr)))
        {
            png_destroy_write_struct(&m_PngPtr, &m_InfoPtr);
            IMGFMT_THROW("Error while writing PNG info.");
        }

        png_write_info(m_PngPtr, m_InfoPtr);
    }

    void PngWriter::write(const void* image, size_t size)
    {
        auto rowSize = getRowSize(m_Info, m_Transform);
        if (size != m_Info.height() * rowSize)
            IMGFMT_THROW("Incorrect image size.");
        std::vector<unsigned char*> rows;
        rows.reserve(m_Info.height());

        auto ucImage = static_cast<unsigned char*>(const_cast<void*>(image));
        for (size_t i = 0; i < m_Info.height(); ++i)
            rows.push_back(ucImage + i * rowSize);

        assertIsValid();
        if (setjmp(png_jmpbuf(m_PngPtr)))
        {
            png_destroy_write_struct(&m_PngPtr, &m_InfoPtr);
            IMGFMT_THROW("Error while writing PNG image data.");
        }
        png_write_image(m_PngPtr, rows.data());
    }

    void PngWriter::writeRows(const void* rows[], uint32_t count,
                              size_t rowSize)
    {
        if (rowSize != getRowSize(m_Info, m_Transform))
            IMGFMT_THROW("Incorrect row size.");

        assertIsValid();
        if (setjmp(png_jmpbuf(m_PngPtr)))
        {
            png_destroy_write_struct(&m_PngPtr, &m_InfoPtr);
            IMGFMT_THROW("Error while writing PNG rows.");
        }
        png_write_rows(
            m_PngPtr,
            reinterpret_cast<unsigned char**>(const_cast<void**>(rows)),
            count);
    }

    void PngWriter::writeRow(const void* row, size_t size)
    {
        if (size != getRowSize(m_Info, m_Transform))
            IMGFMT_THROW("Incorrect row size.");

        assertIsValid();
        if (setjmp(png_jmpbuf(m_PngPtr)))
        {
            png_destroy_write_struct(&m_PngPtr, &m_InfoPtr);
            IMGFMT_THROW("Error while writing PNG row.");
        }
        png_write_row(
            m_PngPtr,
            reinterpret_cast<unsigned char*>(const_cast<void*>(row)));
    }

    void PngWriter::writeEnd()
    {
        assertIsValid();
        png_write_end(m_PngPtr, nullptr);
    }

    void PngWriter::assertIsValid() const
    {
        if (!m_InfoPtr || !m_PngPtr)
            IMGFMT_THROW("");
    }

    void writePng(std::ostream& stream,
                  const void* image, size_t imageSize,
                  PngInfo options, PngTransform transform)
    {
        PngWriter writer(stream, std::move(options), std::move(transform));
        writer.writeInfo();
        writer.write(image, imageSize);
        writer.writeEnd();
    }

    void writePng(const std::string& fileName,
                  const void* image, size_t imageSize,
                  PngInfo options, PngTransform transform)
    {
        std::ofstream stream(fileName);
        if (!stream)
            IMGFMT_THROW("Can not create " + fileName);
        writePng(stream, image, imageSize,
                 std::move(options),
                 std::move(transform));
    }
}
