//
//  Mesh.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

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

    class Texture2D
    {
    private:
        typedef Color_Buffer < Rgba8888 > Color_Buffer;
        
    private:
        GLuint texture_id;
        bool   texture_is_loaded;
    
    public:
        Texture2D (const string & texture_base_path);
       ~Texture2D ();
       
    private:
        Texture2D (const Texture2D & ) = delete;
        Texture2D & operator = (const Texture2D & ) = delete;
        
    public:
        bool is_ok() const
        {
            return texture_is_loaded;
        }
        
        bool bind () const
        {
            return texture_is_loaded ? glBindTexture (GL_TEXTURE_2D, texture_id), true : false;
        }
        
    private:
        GLint create_texture_2d (const string & texture_path);
        unique_ptr < Color_Buffer > load_image (const string & texture_path);
    };
    
    class Material
    {
    private:
        Shader_Program shader_program;
        Texture2D texture;
        glm::vec3 color;
        float shininess;
    
    public:
        Material () = delete;
        Material (const vector < string > & source_code_vertex, const vector < string > & source_code_fragment, const string & texture_base_path);
       ~Material () = default;
        
        void use_shader_program() { shader_program.use(); }
        GLint get_shader_program_uniform_location (const string & uniform) { return shader_program.get_uniform_location(uniform); }
        GLuint get_shader_program_id() const { return shader_program.get_id(); }
        
        
        const glm::vec3 get_color() { return     color; }
        const float get_shininess() { return shininess; }
    
    };
    
}
