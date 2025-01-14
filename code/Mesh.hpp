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

#include <glad/glad.h>
#include <glm.hpp>
#include <vector>
#include <string>
#include "Camera.hpp"
#include "Shader_Program.hpp"
#include "Color.hpp"
#include "Color_Buffer.hpp"

namespace Ragot
{
    using std::vector;
        
    class Mesh
    {
            
    protected:
        enum
        {
            COORDINATES_VBO,
            NORMALS_VBO,
            TEXTURE_UVS_VBO,
            INDICES_EBO,
            VBO_COUNT
        };
        
        vector < glm::vec3 >    coordinates;
        vector < glm::vec3 >        normals;
        vector < glm::vec2 > texture_coords;
        vector < GLuint >           indices;
        
    private:
        GLuint vbo_ids[VBO_COUNT];
        GLuint vao_id;
        
        GLsizei number_of_indices;
        
        float angle;
        
        void load_mesh(const std::string & mesh_file_path);
        
    protected:
        glm::vec3 random_color();
        
        void build_mesh();
        
    private:
        private:
        Mesh (const Mesh & ) = delete;
        Mesh & operator = (const Mesh & ) = delete;
        
    public:
        Mesh() = default;
        Mesh (const std::string & mesh_file_path);
       ~Mesh()
        {
            glDeleteVertexArrays (1, &vao_id);
            glDeleteBuffers      (VBO_COUNT, vbo_ids);
        }
       
    public:        
        const vector < glm::vec3 > & get_coordinates() const { return    coordinates; }
        const vector < glm::vec3 > &     get_normals() const { return        normals; }
        const vector < glm::vec2 > & get_textures_uv() const { return texture_coords; }
        const vector <    GLuint > &     get_indices() const { return        indices; }
        const GLuint                      get_vao_id() const { return         vao_id; }
        const GLsizei          get_number_of_indices() const { return number_of_indices; }
    };

    template < typename COLOR_FORMAT >
    class Texture2D
    {
    protected:
        typedef Color_Buffer < COLOR_FORMAT > Color_Buffer;
        
    protected:
        GLuint texture_id;
        bool   texture_is_loaded;
        
    private:
        bool is_uint8 = false;

    public:
        Texture2D (const string & texture_base_path);
       ~Texture2D ();
       
    private:
        Texture2D (const Texture2D & ) = delete;
        Texture2D & operator = (const Texture2D & ) = delete;
    
    protected:
        Texture2D () : texture_id(0), texture_is_loaded(false) {}
        
    public:
        bool is_ok() const
        {
            return texture_is_loaded;
        }
        
        virtual bool bind () const
        {
            return texture_is_loaded ? glBindTexture (GL_TEXTURE_2D, texture_id), true : false;
        }
        
    protected:
        GLint create_texture_2d (const string & texture_path);
        unique_ptr < Color_Buffer > load_image (const string & texture_path);
    };
    
    class Texture_Cube : public Texture2D < Rgba8888 >
    {
    public:
        Texture_Cube (const string & texture_base_path);
        
        bool bind() const override
        {
            return texture_is_loaded ? glBindTexture (GL_TEXTURE_CUBE_MAP, texture_id), true : false;
        }
    };
    
    class Material
    {
    private:
        Shader_Program shader_program;
        Texture2D< Rgba8888 > texture;
        glm::vec3 color;
        float shininess;
    
    public:
        Material () = delete;
        Material (const vector < string > & source_code_vertex, const vector < string > & source_code_fragment, const string & texture_base_path);
       ~Material () = default;
        
        void use_shader_program() { shader_program.use(); }
        GLint get_shader_program_uniform_location (const string & uniform) { return shader_program.get_uniform_location(uniform); }
        GLuint get_shader_program_id() const { return shader_program.get_id(); }
        
        const bool bind_texture() const { return texture.bind(); }
        
        const glm::vec3 get_color() { return     color; }
        const float get_shininess() { return shininess; }
    
    };
    
}
