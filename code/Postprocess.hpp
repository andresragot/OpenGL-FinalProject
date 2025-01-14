//
//  Postprocess.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 14/1/25.
//

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
