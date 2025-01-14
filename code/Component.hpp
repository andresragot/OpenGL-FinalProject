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
    /**
     * @brief Base class for components.
     */
    class Component
    {
    public:
        /**
         * @brief Virtual destructor for the Component class.
         */
        virtual ~Component() = default;

        /**
         * @brief Gets the entity associated with this component.
         * @return Shared pointer to the associated entity.
         */
        std::shared_ptr<Entity> get_entity() const { return entity.lock(); }
        
        /**
         * @brief Sets the entity associated with this component.
         * @param ent Shared pointer to the entity to associate.
         */
        void set_entity(std::shared_ptr<Entity> ent) { entity = ent; }
        
        /**
         * @brief Checks if the component has a task.
         * @return True if the component has a task, false otherwise.
         */
        bool get_has_task() const { return has_task; }
        
    private:
        std::weak_ptr<Entity> entity; ///< Weak pointer to the associated entity.
        
    protected:
        bool has_task = false; ///< Indicates whether the component has a task.
    };
    
    /**
     * @brief Component for managing transformations.
     */
    class Transform_Component : public Component
    {
    public:
        /**
         * @brief Constructor for the Transform_Component class.
         */
        Transform_Component() : position(0.f), rotation(0.f), scale(1.0f), parent(nullptr) {}
        
        /**
         * @brief Gets the transformation matrix.
         * @return Transformation matrix.
         */
        mat4 get_transform_matrix()
        {
            mat4 transform_matrix(1);
            transform_matrix  = glm::translate(transform_matrix, position);
            transform_matrix  = glm::scale(transform_matrix, scale);
            
            glm::quat quaternion_rotation = glm::quat(glm::radians(rotation));
            transform_matrix *= glm::mat4_cast(quaternion_rotation);
            
            if (parent)
                transform_matrix = parent->get_transform_matrix() * transform_matrix;
            
            return transform_matrix;
        }
        
        /**
         * @brief Sets the position.
         * @param pos New position.
         */
        void set_position(const vec3 &pos) { position = pos; }

        /**
         * @brief Gets the position.
         * @return Current position.
         */
        vec3 get_position() const { return position; }
        
        /**
         * @brief Sets the rotation.
         * @param rot New rotation.
         */
        void set_rotation(const vec3 &rot) { rotation = rot; }

        /**
         * @brief Gets the rotation.
         * @return Current rotation.
         */
        vec3 get_rotation() const { return rotation; }
        
        /**
         * @brief Sets the scale.
         * @param scal New scale.
         */
        void set_scale(const vec3 &scal) { scale = scal; }

        /**
         * @brief Gets the scale.
         * @return Current scale.
         */
        vec3 get_scale() const { return scale; }
        
        /**
         * @brief Sets the parent transformation component.
         * @param par Pointer to the parent transformation component.
         */
        void set_parent(Transform_Component *par) { parent = par; }

        /**
         * @brief Gets the parent transformation component.
         * @return Pointer to the parent transformation component.
         */
        Transform_Component* get_parent() const { return parent; }

    private:
        vec3 position; ///< Position of the component.
        vec3 rotation; ///< Rotation of the component.
        vec3 scale; ///< Scale of the component.
        Transform_Component* parent; ///< Pointer to the parent transformation component.
    };
    
    /**
     * @brief Component for managing models.
     */
    class Model_Component : public Component
    {
    public:
        /**
         * @brief Deleted default constructor.
         */
        Model_Component() = delete;
        
        /**
         * @brief Constructor for the Model_Component class.
         * @param model_file_path Path to the model file.
         * @param texture_file_path Path to the texture file.
         */
        Model_Component(const string &model_file_path, const string &texture_file_path);
        
        Critical_Task render_task; ///< Task for rendering the model.
        
        /**
         * @brief Gets the shader program ID.
         * @return Shader program ID.
         */
        const GLuint get_shader_program_id() const { return material.get_shader_program_id(); }
        
        /**
         * @brief Sets the transparency of the model.
         * @param trans True to set the model as transparent, false otherwise.
         */
        void set_transparency(bool trans) { is_transparent = trans; }
        
    private:
        Mesh mesh; ///< Mesh of the model.
        Material material; ///< Material of the model.
        bool is_transparent = false; ///< Indicates whether the model is transparent.
        
        static const string vertex_shader_code; ///< Vertex shader code.
        static const string fragment_shader_code; ///< Fragment shader code.
        
        GLint model_view_matrix_id; ///< Uniform location for the model-view matrix.
        GLint projection_matrix_id; ///< Uniform location for the projection matrix.
        GLint normal_matrix_id; ///< Uniform location for the normal matrix.
        GLint view_pos_id; ///< Uniform location for the view position.
        
    private:
        /**
         * @brief Configures the material for the model.
         */
        void configure_material();

        /**
         * @brief Renders the model.
         */
        void render();
    };
}
