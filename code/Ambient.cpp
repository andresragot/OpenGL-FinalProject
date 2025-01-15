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
#include "Ambient.hpp"
#include <iostream>

namespace Ragot
{
    using namespace glm;
    
    const GLfloat Skybox::coordinates[] =
    {
        -1.0f, +1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, +1.0f, +1.0f,
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f,
    };
    
    const string Skybox::vertex_shader_code =
        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        ""
        "out vec3 texture_coordinates;"
        ""
        "void main()"
        "{"
        "   texture_coordinates = vec3(vertex_coordinates.x, -vertex_coordinates.y, vertex_coordinates.z);"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "}";

    const string Skybox::fragment_shader_code =

        "#version 330\n"
        ""
        "in  vec3 texture_coordinates;"
        "out vec4 fragment_color;"
        ""
        "uniform samplerCube sampler;"
        ""
        "void main()"
        "{"
        "    fragment_color = texture (sampler, texture_coordinates);"
        "}";
        
    Skybox::Skybox (const string & texture_base_path)
    :
        shader_program ({ vertex_shader_code }, { fragment_shader_code }),
        texture_cube (texture_base_path)
    {
        assert(texture_cube.is_ok ());
        
        model_view_matrix_id = shader_program.get_uniform_location("model_view_matrix");
        projection_matrix_id = shader_program.get_uniform_location("projection_matrix");
        
        glGenBuffers (1, &vbo_id);
        glGenVertexArrays (1, &vao_id);
        
        glBindVertexArray (vao_id);
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_id);
        glBufferData (GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindVertexArray (0);
    }
    
    Skybox::~Skybox()
    {
        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (1, &vbo_id);
    }
    
    void Skybox::render()
    {
        if (!camera)
        {
            return;
        }
            
        shader_program.use();
        
        texture_cube.bind ();
              mat4   model_view_matrix = camera->get_transform_matrix_inverse ();
        const mat4 & projection_matrix = camera->get_projection_matrix ();

        // Se elimina la parte de la traslación
        model_view_matrix[3][0] = 0.0f;
        model_view_matrix[3][1] = 0.0f;
        model_view_matrix[3][2] = 0.0f;
        
        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, value_ptr(model_view_matrix));
        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, value_ptr(projection_matrix));
        
        glDepthMask (GL_FALSE);
        
        glBindVertexArray (vao_id);
        glDrawArrays (GL_TRIANGLES, 0, 36);
                
        glDepthMask (GL_TRUE);
        
        glBindVertexArray (0);
        glUseProgram (0);
        
        // cout << "Render Skybox task" << endl;
    }
    
    const string Terrain::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_position;"
        "layout (location = 1) in vec2 vertex_uv;"
        ""
        "uniform sampler2D height_map;"
        "uniform float max_height;"
        ""
        "out vec3 frag_normal;"
        "out vec2 frag_uv;"
        "out float frag_height;"
        ""
        "void main()"
        "{"
        "   float height = texture(height_map, vertex_uv).r * max_height;"
        "   frag_height = height;"
        ""
        "   vec3 position = vec3(vertex_position.x, height, vertex_position.z);"
        ""
        "   frag_uv = vertex_uv;"
        ""
        "   frag_normal = vec3(0.0, 1.0, 0.0);"
        ""
        "   gl_Position = projection_matrix * model_view_matrix * vec4(position, 1.0);"
        "}";


    const string Terrain::fragment_shader_code =

        "#version 330\n"
        ""
        "in vec3 frag_normal;"
        "in vec2 frag_uv;"
        "in float frag_height;"
        ""
        "out vec4 fragment_color;"
        ""
        "uniform vec3 light_position;"
        "uniform vec3 view_position;"
        ""
        "void main()"
        "{"
        "    vec3 light_dir = normalize(light_position - vec3(frag_uv, frag_height));"
        ""
        "    vec3 view_dir = normalize(view_position - vec3(frag_uv, frag_height));"
        ""
        "    float ambient_strength = 0.1;"
        "    vec3 ambient = ambient_strength * vec3(1.0);"
        ""
        "    float diff = max(dot(frag_normal, light_dir), 0.0);"
        "    vec3 diffuse = diff * vec3(0.5);"
        ""
        "    float specular_strength = 0.5;"
        "    vec3 halfway_dir = normalize(light_dir + view_dir);"
        "    float spec = pow(max(dot(frag_normal, halfway_dir), 0.0), 32.0);"
        "    vec3 specular = specular_strength * spec * vec3(1.0);"
        ""
        "    vec3 color = (ambient + diffuse + specular) * vec3(0.6, 0.8, 0.4);"
        ""

        "    color = mix(vec3(0.2, 0.4, 0.1), color, frag_height / 10.0);"
        ""
        "    fragment_color = vec4(color, 1.0);"
        "}";


    Terrain::Terrain (float width, float depth, unsigned x_slices, unsigned z_slices)
    :
        shader_program({ vertex_shader_code }, { fragment_shader_code }),
        texture("/../../assets/fotos/NvF5e.jpg")
    {
    
        assert(texture.is_ok());
        
        shader_program.use();
        
        model_view_matrix_id = shader_program.get_uniform_location("model_view_matrix");
        projection_matrix_id = shader_program.get_uniform_location("projection_matrix");
            view_position_id = shader_program.get_uniform_location("view_position");
           light_position_id = shader_program.get_uniform_location("light_position");
        
        glUniform1f (shader_program.get_uniform_location("max_height"), 10.f);
        glUniform3fv (light_position_id, 1, glm::value_ptr(glm::vec3(0.f, -2.f, 0.f)));
        
        glUseProgram (0);
    
        number_of_vertices = (x_slices + 1) * (z_slices + 1);
        number_of_indices = x_slices * z_slices * 6;
        
        vector < float > coordinates (number_of_vertices * 3);
        vector < float > texture_uv  (number_of_vertices * 2);
        vector < GLuint > indices (number_of_indices);
        
        float x = - width * .5f;
        float z = - depth * .5f;
        float u = 0.f;
        float v = 0.f;
        
        float x_step = width / float (x_slices);
        float z_step = depth / float (z_slices);
        float u_step =   1.f / float (x_slices);
        float v_step =   1.f / float (z_slices);
        
        int coordinate_index = 0;
        int uv_index = 0;
        
        for (unsigned j = 0; j < z_slices + 1; ++j, z += z_step, v += v_step)
        {
            for (unsigned i = 0; i < x_slices + 1; ++i, x += x_step, u += u_step)
            {
                coordinates [coordinate_index++] = x;
                coordinates [coordinate_index++] = 0.f;
                coordinates [coordinate_index++] = z;
                
                 texture_uv [uv_index++] = u;
                 texture_uv [uv_index++] = v;
            }
            
            x = - width * 5.f;
            u = 0.f;
            
        }
        
        int index = 0;
        for (unsigned j = 0; j < z_slices; ++j)
        {
            for (unsigned i = 0; i < x_slices; ++i)
            {
                GLuint top_left = (j * (x_slices + 1)) + 1;
                GLuint bottom_left = ((j + 1) * (x_slices + 1)) + i;
                
                // primer triangulo
                indices [index++] = top_left;
                indices [index++] = bottom_left;
                indices [index++] = top_left + 1;
                
                // segundo triangulo
                indices [index++] = top_left + 1;
                indices [index++] = bottom_left;
                indices [index++] = bottom_left + 1;
            }
        }
        
        glGenVertexArrays (1, &vao_id);
        glGenBuffers (VBO_COUNT, vbo_ids);
        
        glBindVertexArray (vao_id);
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, coordinates.size () * sizeof (float), coordinates.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
        glBufferData (GL_ARRAY_BUFFER,  texture_uv.size () * sizeof (float),  texture_uv.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        
        glBindVertexArray (0);
    }
    
    Terrain::~Terrain()
    {
        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers (VBO_COUNT, vbo_ids);
    }
    
    void Terrain::render()
    {
        shader_program.use();
    
        texture.bind();
            
              mat4   model_view_matrix = camera->get_transform_matrix_inverse ();
        const mat4 & projection_matrix = camera->get_projection_matrix ();
              vec3   view_position     = camera->get_location();
        
        
        model_view_matrix = glm::translate(model_view_matrix, glm::vec3(50.f, -7.f, -0.f));
        
        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, value_ptr(model_view_matrix));
        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, value_ptr(projection_matrix));
        glUniform3fv       (view_position_id, 1, value_ptr(view_position));
        
        glDepthMask (GL_FALSE);
        
        glBindVertexArray (vao_id);
        glDrawElements (GL_TRIANGLES, number_of_indices * sizeof (GLuint), GL_UNSIGNED_INT, nullptr);
                
        glDepthMask (GL_TRUE);
        
        glBindVertexArray (0);
        glUseProgram (0);
        
    
    }
}
