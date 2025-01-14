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
        Shader_Program (const Shader_Program & ) = delete;
        Shader_Program & operator = (const Shader_Program & ) = delete;
    
    private:
        void initialize (GLuint vertex_shadex_id, GLuint fragment_shader_id);
        
        void show_linkage_error();
        
    };
}
