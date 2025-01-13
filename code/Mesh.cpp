//
//  Mesh.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#include "Mesh.hpp"

#include <iostream>
#include <SOIL2.h>

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
                    // std::cout << "Texture Coord " << i << ": " << mesh->mTextureCoords[0][i].x << ", " << mesh->mTextureCoords[0][i].y << std::endl;
                    texture_coords[i] = vec2 ( mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
                }
                                
                glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
                glBufferData (GL_ARRAY_BUFFER, texture_coords.size() * sizeof(vec2), texture_coords.data(), GL_STATIC_DRAW);
                glEnableVertexAttribArray (TEXTURE_UVS_VBO);
                glVertexAttribPointer (TEXTURE_UVS_VBO, 2, GL_FLOAT, GL_FALSE, 0, 0);
            }
            
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
    
    template < typename COLOR_FORMAT >
    Texture2D < COLOR_FORMAT >::Texture2D(const string & texture_base_path)
    {
        texture_id = create_texture_2d(texture_base_path);
        texture_is_loaded = texture_id > 0;
    }
    
    template < typename COLOR_FORMAT >
    Texture2D < COLOR_FORMAT >::~Texture2D()
    {
        if (texture_is_loaded)
        {
            glDeleteTextures (1, &texture_id);
        }
    }
    
    // Asegúrate de incluir este código en el archivo de implementación (.cpp)
    template class Texture2D<unsigned char>;  // Esto garantiza que el destructor para 'Monochrome8' esté instanciado

    
    template < typename COLOR_FORMAT >
    GLint Texture2D < COLOR_FORMAT >::create_texture_2d(const string & texture_path)
    {
        auto image = load_image (texture_path);
        
        if (image)
        {
            GLuint texture_id;
            glEnable (GL_TEXTURE_2D);
            glGenTextures (1, &texture_id);
            glBindTexture (GL_TEXTURE_2D, texture_id);
            
            glTexImage2D
            (
                 GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image->get_width (),
                 image->get_height (),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image->colors()
            );
            
            glGenerateMipmap (GL_TEXTURE_2D);
            
            glTexParameteri  (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
            glTexParameteri  (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
            glTexParameteri  (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri  (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            return texture_id;
            
        }
        
        return -1;
    }
    
    template < typename COLOR_FORMAT >
    unique_ptr< Color_Buffer < COLOR_FORMAT > > Texture2D < COLOR_FORMAT > ::load_image(const string &texture_path)
    {
        int image_width = 0;
        int image_height = 0;
        int image_channels = 0;
        
        uint8_t * loaded_pixels = SOIL_load_image
        (
            texture_path.c_str(),
            &image_width,
            &image_height,
            &image_channels,
             SOIL_LOAD_RGBA
        );
        
        if (loaded_pixels)
        {
            auto image = make_unique < Color_Buffer > (image_width, image_height);
            
            std::copy_n
            (
                loaded_pixels,
                size_t (image_width) * size_t (image_height) * sizeof (COLOR_FORMAT),
                reinterpret_cast< uint8_t *>(image->colors ())
            );
            
            SOIL_free_image_data(loaded_pixels);
            
            return image;
        }
        
        return nullptr;
    }
    
    Texture_Cube::Texture_Cube (const string & texture_base_path)
    {
        vector < shared_ptr< Color_Buffer > > texture_sides (6);
        
        for (size_t texture_index = 0; texture_index < 6; texture_index++)
        {
        
            texture_sides[texture_index] = load_image (texture_base_path + char('0' + texture_index) + ".png");
            
            if (!texture_sides [texture_index])
            {
                return;
            }
        }
        
        glEnable        (GL_TEXTURE_CUBE_MAP);
                
        glGenTextures   (1, &texture_id);
        
        glActiveTexture (GL_TEXTURE0);
        glBindTexture   (GL_TEXTURE_CUBE_MAP, texture_id);
        
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_EDGE);
        
        static const GLenum texture_target[] =
        {
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
        };
        
        for (size_t texture_index = 0; texture_index < 6; ++texture_index)
        {
            
            Color_Buffer & texture = *texture_sides[texture_index];
            
            glTexImage2D
            (
                texture_target [texture_index],
                0,
                GL_RGBA,
                texture.get_width  (),
                texture.get_height (),
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                texture.colors ()
            );
        }
        
        texture_is_loaded = true;
    }
    
    Material::Material(const vector < string > & source_code_vertex, const vector < string > & source_code_fragment, const string & texture_base_path)
    : shader_program(source_code_vertex, source_code_fragment),
      texture(texture_base_path)
    {
        assert (texture.is_ok());
    
        shininess = 32.0f;
        color = { 1.f, 1.f, 1.f };
    }
}
