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
    
    const string Terrain::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec2 vertex_xz;"
        "layout (location = 1) in vec2 vertex_uv;"
        ""
        "uniform sampler2D sampler;"
        "uniform float     max_height;"
        ""
        "out float intensity;"
        ""
        "void main()"
        "{"
        "   float sample = texture (sampler, vertex_uv).r;"
        "   intensity    = sample * 0.75 + 0.25;"
        "   float height = sample * max_height;"
        "   vec4  xyzw   = vec4(vertex_xz.x, height, vertex_xz.y, 1.0);"
        "   gl_Position  = projection_matrix * model_view_matrix * xyzw;"
        "}";

    const string Terrain::fragment_shader_code =

        "#version 330\n"
        ""
        "in  float intensity;"
        "out vec4  fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = vec4(intensity, intensity, intensity, 1.0);"
        "}";

    Terrain::Terrain (float width, float depth, unsigned x_slices, unsigned z_slices)
    : shader_program({ vertex_shader_code }, { fragment_shader_code })
    {
        number_of_vertices = x_slices * z_slices;
        
        vector < float > coordinates (number_of_vertices * 2);
        vector < float > texture_uv  (number_of_vertices * 2);
        
        float x = - width * .5f;
        float z = - depth * .5f;
        float u = 0.f;
        float v = 0.f;
        
        float x_step = width / float (x_slices);
        float z_step = depth / float (z_slices);
        float u_step =   1.f / float (x_slices);
        float v_step =   1.f / float (z_slices);
        
        int coordinate_index = 0;
        
        for (unsigned j = 0; j < z_slices; ++j, z += z_step, v += v_step)
        {
            for (unsigned i = 0; i < x_slices; ++i, coordinate_index += 2, x += x_step, u += u_step)
            {
                coordinates [coordinate_index + 0] = x;
                coordinates [coordinate_index + 1] = z;
                coordinates [coordinate_index + 0] = u;
                coordinates [coordinate_index + 1] = v;
            }
            
            x += x_step = - x_step;
            u += u_step = - u_step;
            
        }
        
        glGenVertexArrays (1, &vao_id);
        glGenBuffers (VBO_COUNT, vbo_ids);
        
        glBindVertexArray (vao_id);
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, coordinates.size () * sizeof (float), coordinates.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
        glBufferData (GL_ARRAY_BUFFER,  texture_uv.size () * sizeof (float),  texture_uv.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
    Terrain::~Terrain()
    {
        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers (VBO_COUNT, vbo_ids);
    }
}
