//
//  Ambient.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 12/1/25.
//

#include "Ambient.hpp"

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
}
