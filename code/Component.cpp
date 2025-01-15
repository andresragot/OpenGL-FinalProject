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
 
#include "Component.hpp"
#include <cassert>
#include <iostream>
#include "Entity.hpp"

#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective

namespace Ragot
{

    using namespace std;
    using namespace glm;
    
    const string Model_Component::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "uniform mat4 normal_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_normal;"
        "layout (location = 2) in vec2 vertex_texture_uv;"
        ""
        "out vec3 normal;"
        "out vec3 fragment_position;"
        "out vec2 texture_uv;"
        ""
        "void main()"
        "{"
        "   vec4 view_space_pos = model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   fragment_position = vec3(view_space_pos);"
        "   normal = normalize(mat3(normal_matrix) * vertex_normal);"
        "   texture_uv = vertex_texture_uv;"
        "   gl_Position = projection_matrix * view_space_pos;"
        "}";

    const string Model_Component::fragment_shader_code =

        "#version 330\n"
        ""
        "struct Light"
        "{"
        "   vec4 position;"
        "   vec3 color;"
        "   float constant;"
        "   float linear;"
        "   float quadratic;"
        "};"
        ""
        "uniform Light lights[3];"
        "uniform float ambient_intensity;"
        "uniform float diffuse_intensity;"
        "uniform float specular_intensity;"
        "uniform float shininess;"
        ""
        "uniform vec3 view_pos;"
        "uniform vec3 material_color;"
        ""
        "uniform sampler2D sampler2d;"
        ""
        "in  vec3            normal;"
        "in  vec3 fragment_position;"
        "in  vec2 texture_uv;"
        ""
        "out vec4    fragment_color;"
        ""
        "void main()"
        "{"
        "   vec3 result    = vec3(0.0);"
        "   vec3 light_dir = vec3(0.0);"
        "   vec3 norm      = normalize(normal);"
        "   vec3 view_dir  = normalize(view_pos - fragment_position);"
        ""
        "   for (int i = 0; i < 3 ; ++i)\n "
        "   {\n"
        "       vec3 light_dir    = normalize(vec3(lights[i].position) - fragment_position);"
        "       float distance    = length(vec3(lights[i].position) - fragment_position);"
        "       float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));"
        "       vec3 ambient      = ambient_intensity * lights[i].color;"
        "       float diff        = max(dot(norm, light_dir), 0.0);"
        "       vec3 diffuse      = diffuse_intensity * diff * lights[i].color;"
        "       vec3 halfway_dir  = normalize(light_dir + view_dir);"
        "       float spec        = pow(max(dot(norm, halfway_dir), 0.0), shininess);"
        "       vec3 specular     = specular_intensity * spec * lights[i].color;"
        ""
        "       ambient   *= attenuation;"
        "       diffuse   *= attenuation;"
        "       specular  *= attenuation;"
        "       result    += (ambient + diffuse + specular) * material_color;"
        "   }\n"
        "   fragment_color = vec4(result, 0.9) * vec4(texture (sampler2d, texture_uv.st).rgb, 0.9);"
        "}";
        
    Model_Component::Model_Component(const string & model_file_path, const string & texture_file_path)
    :
            mesh (model_file_path),
            material ({vertex_shader_code}, {fragment_shader_code}, texture_file_path),
            render_task ([this] { render(); }),
            update_task ([this] { update(); })
    {
        has_task = true;
        
        material.use_shader_program();

        model_view_matrix_id = material.get_shader_program_uniform_location("model_view_matrix");
        projection_matrix_id = material.get_shader_program_uniform_location("projection_matrix");
            normal_matrix_id = material.get_shader_program_uniform_location(    "normal_matrix");
                 view_pos_id = material.get_shader_program_uniform_location(         "view_pos");
                 
                 
        configure_material();
        
        glUseProgram (0);
        
    }
    
    void Model_Component::configure_material()
    {
        GLint material_color_id = material.get_shader_program_uniform_location("material_color");
        GLint      shininess_id = material.get_shader_program_uniform_location(     "shininess");
        
        
        glUniform3fv (material_color_id, 1, glm::value_ptr(material.get_color()));
        glUniform1f  (shininess_id, material.get_shininess());
    }
    
    void Model_Component::render()
    {
        auto entity = get_entity();
        if (entity)
        {
            auto scene = entity->get_scene();
            auto camera = scene->get_camera();
            
            material.use_shader_program();
            
            if (is_transparent)
            {
                glDepthMask (GL_FALSE);
                glEnable    (GL_BLEND);
                glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            
            material.bind_texture();
    
            glm::mat4 model_matrix = entity->transform.get_transform_matrix();
            glm::mat4 view_matrix = camera ? camera->get_transform_matrix_inverse() : glm::mat4(1.0f);
            glm::mat4 model_view_matrix = view_matrix * model_matrix;
            
            glm::vec3 view_pos = camera ? camera->get_location() : glm::vec3 (1.0f);
            glm::mat4 projection_matrix = camera ? camera->get_projection_matrix() : glm::perspective(45.f, GLfloat(1024) / 640, 1.f, 5000.f);
            glm::mat4 normal_matrix = glm::transpose (glm::inverse (model_view_matrix));
                                    
            glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
            glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
            glUniformMatrix4fv (    normal_matrix_id, 1, GL_FALSE, glm::value_ptr(    normal_matrix));
            glUniform3fv       (         view_pos_id, 1,           glm::value_ptr(         view_pos));
            
            glBindVertexArray (mesh.get_vao_id());
            glDrawElements    (GL_TRIANGLES, mesh.get_number_of_indices(), GL_UNSIGNED_INT, 0);
            
            glBindVertexArray (0);
            glUseProgram      (0);
            
            if (is_transparent)
            {
                glDisable   (GL_BLEND);
                glDepthMask (GL_TRUE );
            }
        }
    }
    
    void Model_Component::update()
    {
        auto entity = get_entity();
        
        if (entity->transform.get_parent())
        {
            orbit_angle  += orbit_speed;
            if (orbit_angle > 360.0f)
            {
                orbit_angle -= 360.0f;
            }
            
            glm::vec3 parent_pos = glm::vec3(entity->transform.get_parent()->get_transform_matrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

            
            float radians = glm::radians(orbit_angle);
            float x = parent_pos.x + 20 * cos(radians);
            float z = parent_pos.z + 20 * sin(radians);
            float y = parent_pos.y + 20 * sin(glm::radians(10.f));
             
             entity->transform.set_position(glm::vec3(x, y, z));
             
             glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
             glm::vec3 direction = glm::normalize(entity->transform.get_parent()->get_position() - entity->transform.get_position());
             glm::vec3 right = glm::normalize(glm::cross(up, direction));
             
            glm::mat4 rotation_matrix = glm::mat4(1.0f);
            rotation_matrix[0] = glm::vec4(right, 0.0f);
            rotation_matrix[1] = glm::vec4(up, 0.0f);
            rotation_matrix[2] = glm::vec4(direction, 0.0f);
            
            //rotation_matrix = rotation_matrix * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::quat rotation = glm::quat_cast(rotation_matrix);
            
            glm::vec3 euler_angles = glm::degrees(glm::eulerAngles(rotation));
             
            entity->transform.set_rotation(euler_angles);
        }
        else
        {
            vertical_position += vertical_speed;
            if (abs(vertical_position) > 2.0f)
            {
                vertical_speed *= -1.f;
            }
            
            entity->transform.set_position (glm::vec3 (entity->transform.get_position().x, vertical_position, entity->transform.get_position().z));
        }
        
    }
}
