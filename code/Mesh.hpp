//
//  Mesh.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <vector>
#include <string>
#include "Task.hpp"
#include "Camera.hpp"

namespace Ragot
{
    using std::vector;
        
    class Mesh
    {
            
    protected:
        enum
        {
            COORDINATES_VBO,
            COLOR_VBO,
            INDICES_EBO,
            VBO_COUNT
        };
        
        vector < glm::vec3 > coordinates;
        vector < glm::vec3 >     normals;
        vector < GLuint >        indices;
        vector < glm::vec3 >      colors;
        
        static const std::string   vertex_shader_code;
        static const std::string fragment_shader_code;
        
        GLuint  program_id;
        
        GLint   model_view_matrix_id;
        GLint   projection_matrix_id;
        
    private:
        GLuint vbo_ids[VBO_COUNT];
        GLuint vao_id;
        
        GLsizei number_of_indices;
        
        float angle;
        
        void load_mesh(const std::string & mesh_file_path);
        GLuint compile_shaders        ();
        void   show_compilation_error (GLuint  shader_id);
        void   show_linkage_error     (GLuint program_id);
        
    protected:
        glm::vec3 random_color();
        
        void build_mesh();
        
    public:
        Mesh() = default;
        Mesh (const std::string & mesh_file_path);
       ~Mesh()
        {
            glDeleteVertexArrays (1, &vao_id);
            glDeleteBuffers      (VBO_COUNT, vbo_ids);
        }
       
    public:
        void render(std::shared_ptr<Camera> camera);
        
        const vector < glm::vec3 > & get_coordinates() const { return coordinates; }
        const vector < glm::vec3 > &     get_normals() const { return     normals; }
        const vector < glm::vec3 > &      get_colors() const { return      colors; }
        const vector <    GLuint > &     get_indices() const { return     indices; }
    };
}
