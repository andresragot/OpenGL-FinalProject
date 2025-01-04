//
//  Mesh.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#include <glad/glad.h>
#include <glm.hpp>
#include <vector>
#include <string>
#include "Task.hpp"

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
        
    private:
        GLuint vbo_ids[VBO_COUNT];
        GLuint vao_id;
        
        GLsizei number_of_indices;
        
        void render();
        
        void load_mesh(const std::string & mesh_file_path);
        
    protected:
        glm::vec3 random_color();
        
        void build_mesh();
        
    public:
        Mesh() : render_task([this] {render(); }) {}
        Mesh (const std::string & mesh_file_path);
       ~Mesh()
        {
            glDeleteVertexArrays (1, &vao_id);
            glDeleteBuffers      (VBO_COUNT, vbo_ids);
        }
       
    public:
        Ragot::Critical_Task render_task;
    };
}
