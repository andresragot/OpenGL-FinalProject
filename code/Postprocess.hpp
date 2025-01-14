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
#include "Shader_Program.hpp"

namespace Ragot
{
    class Frame_Buffer
    {
    private:
        
        enum
        {
            COORDINATES_VBO,
            UV_COORDINATES_VBO,
            VBO_COUNT
        };
    
        static float       vertices[];
        static float uv_coordenates[];
        
        static const string   vertex_code_shader;
        static const string fragment_code_shader;
        
        GLuint frame_buffer_id;
        GLuint      texture_id;
        GLuint  depthbuffer_id;
        GLint  current_time_id;
        
        Shader_Program shader_program;
        
        GLuint vbo_id[VBO_COUNT];
        GLuint vao_id;
        
    public:
        Frame_Buffer(unsigned width, unsigned height);
        Frame_Buffer() = delete;
       ~Frame_Buffer();
       
        void bind_frame_buffer() const { glBindFramebuffer (GL_FRAMEBUFFER, frame_buffer_id); }
        void unbind_frame_buffer() const { glBindFramebuffer (GL_FRAMEBUFFER, 0); }
        void bind_texture() const { glBindTexture (GL_TEXTURE_2D, texture_id); }
        void unbind_texture() const { glBindTexture (GL_TEXTURE_2D, 0); }
        
        void render();        
    };
}
