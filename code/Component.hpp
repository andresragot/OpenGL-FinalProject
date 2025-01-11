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
    
    class Mesh_Component : public Component
    {
    public:
        Mesh_Component() : render_task ([this] { render(); }) { has_task = true; }
        
        explicit Mesh_Component (shared_ptr <Mesh> mesh) : mesh (std::move(mesh)), render_task ([this] { render(); }) { has_task = true; }
        
        void set_mesh (shared_ptr <Mesh> new_mesh)
        {
            mesh = std::move(new_mesh);
        }
        
        shared_ptr <Mesh> get_mesh() const
        {
            return mesh;
        }
        
        void set_camera (shared_ptr<Camera> cam)
        {
            camera = cam;
        }
        
        shared_ptr<Camera> get_camera() const
        {
            return camera;
        }
                
        Critical_Task render_task;
        
    private:
        shared_ptr <Mesh  >   mesh;
        shared_ptr <Camera> camera;
        
        void render();
    };
    
    class Skybox_Component: public Component
    {
    private:
        static const GLfloat coordinates[];
        static const std::string vertex_shader_code;
        static const std::string fragment_shader_code;
        
        GLuint vbo_id;
        GLuint vao_id;
        
        GLuint shader_program_id;
        
        GLint  model_view_matrix_id;
        GLint  projection_matrix_id;
        
        shared_ptr<Camera> camera = nullptr;
        
        Texture_Cube texture_cube;
        
    public:
        Skybox_Component (const std::string & texture_path);
       ~Skybox_Component ();
       
        void set_camera(shared_ptr<Camera> cam) { camera = cam; }
       
        Critical_Task render_task;
        
    private:
        GLuint compile_shaders();
        void show_compilation_error (GLuint  shader_id);
        void     show_linkage_error (GLuint program_id);
        
        void render ();
        
    };
}
