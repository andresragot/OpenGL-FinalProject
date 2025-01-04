//
//  Mesh.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#include "Mesh.hpp"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Ragot
{
    // using glm::vec3;
    using namespace glm;
    
    using std::string;
    
    Mesh::Mesh(const string & mesh_file_path) : Mesh()
    {
        load_mesh(mesh_file_path);
    }
    
    void Mesh::build_mesh()
    {
        // Se generan los índices para los VBOs del cubo:
        glGenBuffers (VBO_COUNT, vbo_ids);
        
        glGenVertexArrays (1, &vao_id);
        
        // Se activa el VAO del cubo para configurarlo;
        
        glBindVertexArray (vao_id);
        
        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, coordinates.size() * sizeof (vec3), coordinates.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        // Se suben a un VBO los datos de color y se vinculan al VAO:
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLOR_VBO]);
        glBufferData (GL_ARRAY_BUFFER, colors.size() * sizeof (vec3), colors.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (GLuint), indices.data(), GL_STATIC_DRAW);
        
        number_of_indices = (GLsizei) sizeof(indices);
        
        glBindVertexArray (0);
    }

    void Mesh::render()
    {
        glBindVertexArray (vao_id);
        glDrawElements    (GL_TRIANGLES, number_of_indices, GL_UNSIGNED_BYTE, 0);
        glBindVertexArray (0);
    }
    
    void Mesh::load_mesh(const std::string &mesh_file_path)
    {
        Assimp::Importer importer;
        
        auto scene = importer.ReadFile
        (
            mesh_file_path,
            aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
        );
        
        
        if (scene && scene->mNumMeshes > 0)
        {
            // TODO: Cargar todas las meshes del file
            std::cout << "Num of meshes in file: " << scene->mNumMeshes << std::endl;
            
            auto mesh = scene->mMeshes[0];
            
            size_t number_of_vertices = mesh->mNumVertices;
                
            glGenBuffers (VBO_COUNT, vbo_ids);
            glGenVertexArrays (1, &vao_id);
            
            glBindVertexArray (vao_id);
            
            static_assert(sizeof (aiVector3D) == sizeof (fvec3), "aiVector3D should be composed of three floats");
            
            glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
            glBufferData (GL_ARRAY_BUFFER, number_of_vertices * sizeof (aiVector3D), mesh->mVertices, GL_STATIC_DRAW);
            
            glEnableVertexAttribArray (0);
            glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            colors.reserve(number_of_vertices);
            
            for (auto & color : colors)
            {
                color = {0.1f, 0.1f, 0.1f};
            }
            
            glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLOR_VBO]);
            glBufferData (GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), colors.data(), GL_STATIC_DRAW);
            
            glEnableVertexAttribArray (1);
            glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            number_of_indices = mesh->mNumFaces * 3;
            
            indices.reserve(number_of_vertices);
            
            auto vertex_index = indices.begin();
            
            for (unsigned i = 0; i < mesh->mNumFaces; ++i)
            {
                auto & face = mesh->mFaces[i];
                
                assert(face.mNumIndices == 3);
                
                *vertex_index++ = face.mIndices[0];
                *vertex_index++ = face.mIndices[1];
                *vertex_index++ = face.mIndices[2];
            }
            
            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
            glBufferData (GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (GLuint), indices.data(), GL_STATIC_DRAW);
        }
    }
    
    vec3 Mesh::random_color()
    {
        return vec3
        (
            float (rand ()) / float (RAND_MAX),
            float (rand ()) / float (RAND_MAX),
            float (rand ()) / float (RAND_MAX)
        );
    }
}
