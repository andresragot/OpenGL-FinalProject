//
//  Component.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#include "Component.hpp"
#include <cassert>
#include <iostream>
#include "Entity.hpp"

namespace Ragot
{

    void check_gl_error(const std::string &label)
    {
        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR)
        {
            std::cerr << "OpenGL error [" << label << "]: " << error << std::endl;
                // if (error != 1280) assert(false);
        }
    }

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
        "   fragment_color = vec4(result, 1.0) * vec4(texture (sampler2d, texture_uv.st).rgb, 1.0);"
        "}";
        
    Model_Component::Model_Component(const string & model_file_path)
    :
            mesh (model_file_path),
            material ({vertex_shader_code}, {fragment_shader_code}, "Intergalactic Spaceship_color_4.jpg"),
            render_task ([this] { render(); })
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
    
            glm::mat4 model_matrix = entity->transform.get_transform_matrix();
            glm::mat4 view_matrix = camera ? camera->get_transform_matrix_inverse() : glm::mat4(1.0f);
            glm::vec3 view_pos = camera ? camera->get_location() : glm::vec3 (1.0f);
            glm::mat4 model_view_matrix = model_matrix * view_matrix;
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
            
            // TODO: hacer render
            // mesh->render(mvp_matrix);
        }
    }
}
