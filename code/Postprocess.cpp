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
 
#include "Postprocess.hpp"
#include <cassert>
#include <iostream>
#include <ctime>

namespace Ragot
{
    float Frame_Buffer::vertices[] = {   -1.f, +1.f, +0.f,
                                         -1.f, -1.f, +0.f,
                                         +1.f, -1.f, +0.f,
                                         -1.f, +1.f, +0.f,
                                         +1.f, -1.f, +0.f,
                                         +1.f, +1.f, +0.f
                                     };
                                     
    float Frame_Buffer::uv_coordenates[] = { +0.f, +1.f,
                                             +0.f, +0.f,
                                             +1.f, +0.f,
                                             +0.f, +1.f,
                                             +1.f, +0.f,
                                             +1.f, +1.f
                                            };
                                            
    const string Frame_Buffer::vertex_code_shader =
        "#version 330\n"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec2 uv_coordinates;"
        ""
        "out vec2 texture_coordinates;"
        ""
        "void main()"
        "{"
        "   gl_Position = vec4(vertex_coordinates, 1.0);"
        "   texture_coordinates = uv_coordinates;"
        "}";
    
    const string Frame_Buffer::fragment_code_shader =
        "#version 330\n"
        ""
        "uniform float time;"
        "uniform sampler2D screen_texture;"
        ""
        "in vec2 texture_coordinates;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "   vec2 uv = texture_coordinates;"
        "   vec2 centered = uv * 2.0 - 1.0;"
        "   float r2 = dot (centered, centered);"
        "   uv = uv + centered * r2 * 0.008;"
        ""
        "   float scan_line = sin (uv.y * 800.0 + time * 10.0) * 0.1;"
        ""
        "   float amount = 0.005;"
        "   vec4 color;"
        "   color.r = texture(screen_texture, uv + vec2(amount, 0.0)).r;"
        "   color.g = texture(screen_texture, uv).g;"
        "   color.b = texture(screen_texture, uv - vec2(amount, 0.0)).b;"
        ""
        "   fragment_color = color + vec4 (0.0, scan_line, scan_line, 0.0);"
        "}";
        
    Frame_Buffer::Frame_Buffer(unsigned width, unsigned height)
    : shader_program({vertex_code_shader}, {fragment_code_shader})
    {
        glGenVertexArrays (1, &vao_id);
        glGenBuffers (VBO_COUNT, vbo_id);
        
        
        glBindVertexArray (vao_id);
        glBindBuffer (GL_ARRAY_BUFFER, vbo_id[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);;
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_id[UV_COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof (uv_coordenates), uv_coordenates, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindVertexArray (0);
        
        {
            glGenFramebuffers (1, &frame_buffer_id);
            bind_frame_buffer();
        }
        
        {
            glGenTextures (1, &texture_id);
            bind_texture();
        
            glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        
        {
            glGenRenderbuffers        (1, &depthbuffer_id);
            glBindRenderbuffer        (GL_RENDERBUFFER, depthbuffer_id);
            glRenderbufferStorage     (GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer (GL_FRAMEBUFFER,  GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,   depthbuffer_id);
        }
        
        {
            glFramebufferTexture (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, 0);
            
            const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
            
            glDrawBuffers (1, &draw_buffer);
            
        }
        
        assert (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        
        unbind_frame_buffer();
        
        
        shader_program.use();
        glUniform1i (shader_program.get_uniform_location("screen_texture"), 1);
        
        current_time_id = shader_program.get_uniform_location("time");
        
        
        glBindTexture (GL_TEXTURE_2D, 0);
        
        glUseProgram (0);
    }
    
    void Frame_Buffer::render()
    {
        time_t timestamp;
        time (&timestamp);
        shader_program.use();
        
        glUniform1f (current_time_id, (float (timestamp) * 0.000001f));
        
        glActiveTexture (GL_TEXTURE1);
        bind_texture();
        
        glBindVertexArray (vao_id);
        glDrawArrays (GL_TRIANGLES, 0, 6);
        
        glBindVertexArray (0);
        
        
        unbind_texture();
        glActiveTexture (GL_TEXTURE0);
        
        glUseProgram (0);
        
    }
    
    Frame_Buffer::~Frame_Buffer()
    {
        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers (VBO_COUNT, vbo_id);
        
        
        glDeleteTextures (1, &texture_id);
        glDeleteRenderbuffers (1, &depthbuffer_id);
        
        unbind_frame_buffer();
        
        glDeleteFramebuffers (1, &frame_buffer_id);
    }
}
