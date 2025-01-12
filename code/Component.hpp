//
//  Component.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#pragma once

#include "MySystem.hpp"
#include "Mesh.hpp"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr, quat
#include "Task.hpp"
#include "Camera.hpp"
#include "Texture_Cube.hpp"
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
        
        Model_Component (const string & model_file_path);
                
        Critical_Task render_task;
        
        const GLuint      get_shader_program_id() const { return material.get_shader_program_id(); };
    private:
        Mesh mesh;
        Material material;
        
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
