/*
 *  This file is part of OpenGL-FinalProject
 *
 *  Developed by Andrés Ragot - github.com/andresragot
 *
 *  MIT License
 *
 *  Copyright (c) 2024 Andrés Ragot
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include <vector>

namespace Ragot
{
    /**
     * @brief Template class for managing a color buffer.
     * @tparam COLOR The color format for the buffer.
     */
    template<typename COLOR>
    class Color_Buffer
    {
    public:
        using Color = COLOR; ///< Type alias for the color format.

    private:
        unsigned width; ///< Width of the buffer.
        unsigned height; ///< Height of the buffer.

        std::vector<Color> buffer; ///< Vector to store the color data.

    public:
        /**
         * @brief Constructor for the Color_Buffer class.
         * @param width Width of the buffer.
         * @param height Height of the buffer.
         */
        Color_Buffer(unsigned width, unsigned height) 
        :
            width(width), 
            height(height),
            buffer(width * height)
        {
        }

        /**
         * @brief Gets the width of the buffer.
         * @return Width of the buffer.
         */
        unsigned get_width() const
        {
            return width;
        }

        /**
         * @brief Gets the height of the buffer.
         * @return Height of the buffer.
         */
        unsigned get_height() const
        {
            return height;
        }

        /**
         * @brief Gets a pointer to the color data.
         * @return Pointer to the color data.
         */
        Color* colors()
        {
            return buffer.data();
        }

        /**
         * @brief Gets a constant pointer to the color data.
         * @return Constant pointer to the color data.
         */
        const Color* colors() const
        {
            return buffer.data();
        }

        /**
         * @brief Gets the color at a specific offset.
         * @param offset The offset in the buffer.
         * @return Reference to the color at the specified offset.
         */
        Color& get(unsigned offset)
        {
            return buffer[offset];
        }

        /**
         * @brief Gets the color at a specific offset (constant version).
         * @param offset The offset in the buffer.
         * @return Constant reference to the color at the specified offset.
         */
        const Color& get(unsigned offset) const
        {
            return buffer[offset];
        }

        /**
         * @brief Sets the color at a specific offset.
         * @param offset The offset in the buffer.
         * @param color The color to set.
         */
        void set(unsigned offset, const Color& color)
        {
            buffer[offset] = color;
        }
    };
}
