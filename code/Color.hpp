/*
 *  This file is part of OpenGL-FinalProject
 *
 *  Developed by Andr�s Ragot - github.com/andresragot
 *
 *  MIT License
 *
 *  Copyright (c) 2024 Andr�s Ragot
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

#include <cstdint>

namespace Ragot
{
    /**
     * @brief Type alias for an 8-bit monochrome color.
     */
    using Monochrome8 = uint8_t;

    /**
     * @brief Union for managing RGBA color values.
     */
    union Rgba8888
    {
        /**
         * @brief Enum for the RGBA component indices.
         */
        enum { RED, GREEN, BLUE, ALPHA };

        uint32_t value; ///< 32-bit RGBA color value.
        uint8_t  components[4]; ///< Array of RGBA components.
    };
}
