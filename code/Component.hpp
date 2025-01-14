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

#include "MySystem.hpp"
#include "Mesh.hpp"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr, quat
#include "Camera.hpp"
#include "Shader_Program.hpp"

using glm::vec3;
using glm::mat4;

namespace Ragot
{
    class Component
    {
    public:
        virtual ~Component() = default;
        std::shared_ptr<Entity> get_entity() const { return entity.lock(); }
        
        void set_entity (std::shared_ptr<Entity> ent) { entity = ent; }
        
        bool get_has_task() const { return has_task; }
        
    private:
        std::weak_ptr<Entity> entity;
        
    protected:
        bool has_task = false;
    };
    
    class Transform_Component : public Component
    {
    public:
        Transform_Component() : position (0.f), rotation (0.f), scale (1.0f), parent (nullptr) {}
        
        mat4 get_transform_matrix ()
        {
            mat4 transform_matrix (1);
            transform_matrix  = glm::translate (transform_matrix, position);
            transform_matrix  = glm::scale     (transform_matrix,    scale);
            
            glm::quat quaternion_rotation = glm::quat (glm::radians (rotation));
            transform_matrix *= glm::mat4_cast (quaternion_rotation);
            
            if (parent)
                transform_matrix = parent->get_transform_matrix() * transform_matrix;
            
            return transform_matrix;
        }
        
        void set_position (const vec3 & pos) { position =  pos; }
        vec3 get_position() const { return position; }
        
        void set_rotation (const vec3 & rot) { rotation =  rot; }
        vec3 get_rotation() const { return rotation; }
        
        void set_scale    (const vec3& scal) {    scale = scal; }
        vec3 get_scale()    const { return scale; }
        
        void set_parent (Transform_Component * par) { parent = par; }
        Transform_Component * get_parent() const { return parent; }

    private:
        
        vec3 position;
        vec3 rotation;
        vec3 scale;
        
        Transform_Component * parent;
        
    };
    
    class Model_Component : public Component
    {
    public:
        Model_Component() = delete;
        
        Model_Component (const string & model_file_path, const string & texture_file_path);
                
        Critical_Task render_task;
        
        const GLuint      get_shader_program_id() const { return material.get_shader_program_id(); };
        
        void set_transparency (bool trans) { is_transparent = trans; }
        
    private:
        Mesh mesh;
        Material material;
        bool is_transparent = false;
        
        static const string   vertex_shader_code;
        static const string fragment_shader_code;
        
        GLint model_view_matrix_id;
        GLint projection_matrix_id;
        GLint     normal_matrix_id;
        GLint          view_pos_id;
        
    private:
        void configure_material ();
        void render ();
    };
}
