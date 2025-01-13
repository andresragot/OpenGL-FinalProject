//
//  Ambient.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 12/1/25.
//

#pragma once

#include "Camera.hpp"
#include "Shader_Program.hpp"
#include "Mesh.hpp"
#include "Color.hpp"

#include <glad/glad.h>
#include <string>

namespace Ragot
{
    using namespace std;
    class Skybox
    {
    private:
        Shader_Program shader_program;
    
        static const GLfloat coordinates[];
        static const string   vertex_shader_code;
        static const string fragment_shader_code;
        
        GLuint vbo_id;
        GLuint vao_id;
        
        GLint  model_view_matrix_id;
        GLint  projection_matrix_id;
        
        shared_ptr<Camera> camera = nullptr;
        
        Texture_Cube texture_cube;
        
    public:
        Skybox (const string & texture_path);
       ~Skybox ();
       
        void set_camera(shared_ptr<Camera> cam) { camera = cam; }
        void render ();
        
    };
    
    class Terrain
    {
    private:
        enum
        {
            COORDINATES_VBO,
            TEXTURE_UVS_VBO,
            VBO_COUNT
        };
    private:
        Shader_Program shader_program;
        
        static const string   vertex_shader_code;
        static const string fragment_shader_code;
        
    
        GLsizei number_of_vertices;
        GLuint vbo_ids[VBO_COUNT];
        GLuint vao_id;
        
        GLint  model_view_matrix_id;
        GLint  projection_matrix_id;
        
        shared_ptr< Camera > camera = nullptr;
        
        Texture2D< Monochrome8 > texture;
    public:
        Terrain (float width, float depth, unsigned x_slices, unsigned z_slices);
       ~Terrain ();
       
    public:
        void set_camera(shared_ptr<Camera> cam) { camera = cam; }
        void render();
    };
    
    class Light
    {
    public:
        glm::vec3 color;
        Light (const glm:: vec3 & color) : color(color) {}
        virtual ~Light() = default;
    };
    
    class DirectionalLight : public Light
    {
    public:
        glm::vec3 direction;
        DirectionalLight(const glm::vec3 & color, const glm::vec3 direction) : Light (color), direction (direction) {}
    };
    
    class PointLight : public Light
    {
    public:
        glm::vec3 position;
        PointLight(const glm::vec3 & color, const glm::vec3 & position) : Light (color), position (position) {}
    };
    
    class AreaLight : public Light
    {
    public:
        glm::vec3 position;
        glm::vec3 size;
        AreaLight (const glm::vec3 & color, const glm::vec3 & position, const glm::vec3 & size) :
            Light (color), position (position), size (size) {}
        
    };

}
