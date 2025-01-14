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
            INDICES_EBO,
            VBO_COUNT
        };
    private:
        Shader_Program shader_program;
        
        static const string   vertex_shader_code;
        static const string fragment_shader_code;
        
    
        GLsizei number_of_vertices;
        GLsizei number_of_indices;
        GLuint vbo_ids[VBO_COUNT];
        GLuint vao_id;
        
        GLint  model_view_matrix_id;
        GLint  projection_matrix_id;
        GLint  view_position_id;
        GLint  light_position_id;
        
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
