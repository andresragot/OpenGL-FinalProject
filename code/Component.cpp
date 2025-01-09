//
//  Component.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#include "Component.hpp"
#include <cassert>
#include <iostream>


namespace Ragot
{

// void check_gl_error(const std::string &label) { GLenum error; while ((error = glGetError()) != GL_NO_ERROR) { std::cerr << "OpenGL error [" << label << "]: " << error << std::endl; } }

    using namespace std;
    using namespace glm;
    
    const GLfloat Skybox_Component::coordinates[] =
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
    
    const string Skybox_Component::vertex_shader_code =
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

    const string Skybox_Component::fragment_shader_code =

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
        
    Skybox_Component::Skybox_Component (const string & texture_base_path)
    :
        texture_cube (texture_base_path),
        render_task([this] { render(); })
    {
        assert(texture_cube.is_ok ());
        
        shader_program_id = compile_shaders();
        
        model_view_matrix_id = glGetUniformLocation (shader_program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (shader_program_id, "projection_matrix");
        
        glGenBuffers (1, &vbo_id);
        glGenVertexArrays (1, &vao_id);
        
        glBindVertexArray (vao_id);
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_id);
        glBufferData (GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindVertexArray (0);
        
        has_task = true;
    }
    
    Skybox_Component::~Skybox_Component()
    {
        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (1, &vbo_id);
    }
    
    void Skybox_Component::render()
    {
        if (!camera)
            return;
            
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
        glUseProgram (shader_program_id);
        
        texture_cube.bind ();
        const mat4   model_view_matrix = camera->get_transform_matrix_inverse ();
        const mat4 & projection_matrix = camera->get_projection_matrix ();
        
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
    
    GLuint Skybox_Component::compile_shaders()
    {
        GLint succeeded = GL_FALSE;
        
        GLuint   vertex_shader_id = glCreateShader (GL_VERTEX_SHADER  );
        GLuint fragment_shader_id = glCreateShader (GL_FRAGMENT_SHADER);
        
        const char *   vertex_shaders_code[] = {         vertex_shader_code.c_str () };
        const char * fragment_shaders_code[] = {       fragment_shader_code.c_str () };
        const GLint    vertex_shaders_size[] = { (GLint)  vertex_shader_code.size () };
        const GLint  fragment_shaders_size[] = { (GLint)fragment_shader_code.size () };
        
        glShaderSource  (  vertex_shader_id, 1,   vertex_shaders_code,   vertex_shaders_size);
        glShaderSource  (fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);
        
        glCompileShader (  vertex_shader_id);
        glCompileShader (fragment_shader_id);
        
        glGetShaderiv   (  vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (  vertex_shader_id);
        
        glGetShaderiv   (fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (fragment_shader_id);
        
        GLuint program_id = glCreateProgram();
        
        glAttachShader (program_id,   vertex_shader_id);
        glAttachShader (program_id, fragment_shader_id);
        
        glLinkProgram  (program_id);
        
        glGetProgramiv (program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error (program_id);
        
        glDeleteShader (  vertex_shader_id);
        glDeleteShader (fragment_shader_id);
        
        return program_id;
    }
    
    void Skybox_Component::show_compilation_error (GLuint shader_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetShaderiv (shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize (info_log_length);

        glGetShaderInfoLog (shader_id, info_log_length, NULL, &info_log.front ());

        cerr << info_log.c_str () << endl;
        
        assert(false);
    }

    void Skybox_Component::show_linkage_error (GLuint program_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize (info_log_length);

        glGetProgramInfoLog (program_id, info_log_length, NULL, &info_log.front ());

        cerr << info_log.c_str () << endl;

        assert(false);
    }
    
}
