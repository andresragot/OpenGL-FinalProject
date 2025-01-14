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
    /**
     * @brief Class for managing a frame buffer in OpenGL.
     */
    class Frame_Buffer
    {
    private:
        /**
         * @brief Enum for VBO indices.
         */
        enum
        {
            COORDINATES_VBO, ///< VBO index for coordinates.
            UV_COORDINATES_VBO, ///< VBO index for UV coordinates.
            VBO_COUNT ///< Total number of VBOs.
        };
    
        static float vertices[]; ///< Array of vertices for the frame buffer.
        static float uv_coordenates[]; ///< Array of UV coordinates for the frame buffer.
        
        static const string vertex_code_shader; ///< Vertex shader code.
        static const string fragment_code_shader; ///< Fragment shader code.
        
        GLuint frame_buffer_id; ///< Frame buffer ID.
        GLuint texture_id; ///< Texture ID.
        GLuint depthbuffer_id; ///< Depth buffer ID.
        GLint current_time_id; ///< Uniform location for current time.
        
        Shader_Program shader_program; ///< Shader program for rendering.
        
        GLuint vbo_id[VBO_COUNT]; ///< Array of VBO IDs.
        GLuint vao_id; ///< Vertex Array Object ID.
        
    public:
        /**
         * @brief Constructor for the Frame_Buffer class.
         * @param width Width of the frame buffer.
         * @param height Height of the frame buffer.
         */
        Frame_Buffer(unsigned width, unsigned height);

        /**
         * @brief Default constructor is deleted.
         */
        Frame_Buffer() = delete;

        /**
         * @brief Destructor for the Frame_Buffer class.
         */
        ~Frame_Buffer();
       
        /**
         * @brief Binds the frame buffer.
         */
        void bind_frame_buffer() const { glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id); }

        /**
         * @brief Unbinds the frame buffer.
         */
        void unbind_frame_buffer() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

        /**
         * @brief Binds the texture.
         */
        void bind_texture() const { glBindTexture(GL_TEXTURE_2D, texture_id); }

        /**
         * @brief Unbinds the texture.
         */
        void unbind_texture() const { glBindTexture(GL_TEXTURE_2D, 0); }
        
        /**
         * @brief Renders the frame buffer.
         */
        void render();        
    };
}
