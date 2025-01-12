//
//  Shader_Program.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 12/1/25.
//

#include "Shader_Program.hpp"

#include <cassert>
#include <iostream>

namespace Ragot
{
    Shader::Shader (const vector < string > & source_code, GLenum type)
    {
        GLint succeded = GL_FALSE;
        
        id = glCreateShader (type);
        
        vector < const char * > shader_code;
        vector < GLint        > shader_size;
        
        shader_code.reserve(source_code.size());
        shader_size.reserve(shader_code.size());
        
        for (const auto & str : source_code)
        {
            shader_code.push_back(        str.c_str());
            shader_size.push_back((GLint) str.size() );
        }
        
        glShaderSource  (id, (GLsizei) shader_code.size(), shader_code.data(), shader_size.data());
        
        glCompileShader (id);
        
        glGetShaderiv   (id, GL_COMPILE_STATUS, &succeded);
        if (!succeded)  show_compilation_error();
    }
    
    void Shader::show_compilation_error()
    {
        GLint info_log_lenght;
        glGetShaderiv (id, GL_INFO_LOG_LENGTH, &info_log_lenght);
        
        error.resize (info_log_lenght);
        
        glGetShaderInfoLog (id, info_log_lenght, NULL, &error.front());
        
        cerr << error.c_str() << endl;
        
        assert (false);
    }
    
    Shader_Program::Shader_Program (const vector < string > & source_code_vertex, const vector < string > & source_code_fragment)
    {
          Vertex_Shader   vertex_shader (source_code_vertex  );
        Fragment_Shader fragment_shader (source_code_fragment);
        
        initialize(vertex_shader.get_id(), fragment_shader.get_id());
    }
    
    void Shader_Program::initialize(GLuint vertex_shadex_id, GLuint fragment_shader_id)
    {
        GLint succeded = GL_FALSE;
        
        program_id = glCreateProgram ();
        
        glAttachShader (program_id,   vertex_shadex_id);
        glAttachShader (program_id, fragment_shader_id);
        glLinkProgram  (program_id);
        
        glGetProgramiv (program_id, GL_LINK_STATUS, &succeded);
        if(!succeded)  show_linkage_error();
     }
    
    void Shader_Program::show_linkage_error()
    {
        string info_log;
        GLint  info_log_length;
        
        glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        
        info_log.resize (info_log_length + 1);
        
        glGetProgramInfoLog (program_id, info_log_length, NULL, &info_log.front ());
        
        cerr << info_log.c_str() << endl;
        
        assert(false);
    
    }

    
}
