//
//  Shader_Program.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 12/1/25.
//

#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>

namespace Ragot
{
    using namespace std;

    class Shader
    {
    private:
        GLuint  id;
        string  error;
        bool    compilation_succeeded;
        
    protected:
        Shader (const vector< string > & source_code, GLenum type);
        
        GLuint compile_shader();
        void show_compilation_error ();
        
    public:
        Shader () = delete;

       ~Shader ()
        {
            glDeleteShader (id);
        }
        
        GLuint get_id () const
        {
            return id;
        }
        
        string * get_error ()
        {
            return error.empty() ? nullptr : &error;
        }
        
        bool is_ok () const
        {
            return compilation_succeeded;
        }
        
    };

    class Vertex_Shader : public Shader
    {
    public:
        Vertex_Shader (const vector < string > & source_code) : Shader (source_code, GL_VERTEX_SHADER)
        {
        }
    };

    class Fragment_Shader : public Shader
    {
    public:
        Fragment_Shader (const vector <string > & source_code) : Shader (source_code, GL_FRAGMENT_SHADER)
        {
        }
    };

    class Shader_Program
    {
    private:
        GLuint program_id;
    
    public:
        Shader_Program (const vector < string > & source_code_vertex, const vector < string > & source_code_fragment);
        Shader_Program () = delete;
       ~Shader_Program ()
        {
            glDeleteProgram (program_id);
        }
        
        void use() const
        {
            glUseProgram(program_id);
        }
        
        GLuint get_id() const
        {
            return program_id;
        }
        
        GLuint get_uniform_location (string uniform_name ) const
        {
            return glGetUniformLocation (program_id, uniform_name.c_str());
        }
        
    private:
        void initialize (GLuint vertex_shadex_id, GLuint fragment_shader_id);
        
        void show_linkage_error();
        
    };
}
