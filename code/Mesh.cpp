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
    using namespace glm;
    using namespace std;
        
    Mesh::Mesh(const string & mesh_file_path)
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
        
        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (GLuint), indices.data(), GL_STATIC_DRAW);
        
        number_of_indices = (GLsizei) sizeof(indices);
        
        glBindVertexArray (0);
    }
    
    void Mesh::load_mesh(const std::string & mesh_file_path)
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
            
            if (mesh->HasNormals())
            {
                glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
                glBufferData (GL_ARRAY_BUFFER, number_of_vertices * sizeof (aiVector3D), mesh->mNormals, GL_STATIC_DRAW);
                
                glEnableVertexAttribArray (1);
                glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);
                
            }
            
            if (mesh->HasTextureCoords(0))
            {
                texture_coords.resize(number_of_vertices);
                for (unsigned i = 0; i < number_of_vertices; ++i)
                {
                    texture_coords[i] = vec2 ( mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                }
                                
                glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
                glBufferData (GL_ARRAY_BUFFER, texture_coords.size() * sizeof(vec2), texture_coords.data(), GL_STATIC_DRAW);
                glEnableVertexAttribArray (TEXTURE_UVS_VBO);
                glVertexAttribPointer (TEXTURE_UVS_VBO, 2, GL_FLOAT, GL_FALSE, 0, 0);
            }
            
            
            glEnableVertexAttribArray (2);
            glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            number_of_indices = mesh->mNumFaces * 3;
            
            indices.resize(number_of_indices);
            
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
            
            std::cout << "Number of vertices: " << number_of_vertices << std::endl;
            std::cout << "Number of indices: "  << number_of_indices  << std::endl;
            
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
    
    Material::Material(const vector < string > & source_code_vertex, const vector < string > & source_code_fragment)
    : shader_program(source_code_vertex, source_code_fragment)
    {
        shininess = 32.0f;
        color = { 1.f, 1.f, 1.f };
    }
}
