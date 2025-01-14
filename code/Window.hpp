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

#include <SDL.h>
#include <string>
#include <utility>

#include <iostream>

namespace Ragot
{
    class Window
    {
    public:
        enum Position
        {
            UNDEFINED = SDL_WINDOWPOS_UNDEFINED,
            CENTERED  = SDL_WINDOWPOS_CENTERED,
        };
        
        struct OpenGL_Context_Settings
        {
            unsigned version_major       = 3;
            unsigned version_minor       = 3;
            bool     core_profile        = true;
            unsigned depth_buffer_size   = 24;
            unsigned stencil_buffer_size = 0;
            bool     enable_vsync        = true;
        };

    private:

        SDL_Window  * window_handle;
        SDL_GLContext opengl_context;
        
        unsigned  width;
        unsigned height;
        
    public:
        Window
        (
            const std::string & title,
            int left_x,
            int top_y,
            unsigned width,
            unsigned height,
            const OpenGL_Context_Settings & context_details
        )
        :
            Window (title.c_str(), left_x, top_y, width, height, context_details)
        {
        }
        
        Window
        (
            const char * title,
            int left_x,
            int top_y,
            unsigned width,
            unsigned height,
            const OpenGL_Context_Settings & context_details
        );
        
       ~Window();
       
        
    public:
        Window (const Window & ) = delete;
        Window & operator = (const Window & ) = delete;
        
        Window(Window && other) noexcept
        {
            this->window_handle  = std::exchange (other.window_handle,  nullptr);
            this->opengl_context = std::exchange (other.opengl_context, nullptr);
        }

        Window & operator = (Window && other) noexcept
        {
            this->window_handle  = std::exchange (other.window_handle,  nullptr);
            this->opengl_context = std::exchange (other.opengl_context, nullptr);
            
            return *this;
        }
        
        void swap_buffers ()
        {
            SDL_GL_SwapWindow (window_handle);
        }
        
        unsigned get_width()  { return width;}
        unsigned get_height() { return height;}
    };
}
