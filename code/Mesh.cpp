//
//  Mesh.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "Mesh.hpp"
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr
#include <gtx/string_cast.hpp>              // to_string

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Ragot
{
    using namespace glm;
    using namespace std;
    
    const string Mesh::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_color;"
        ""
        "out vec3 front_color;"
        ""
        "void main()"
        "{"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   front_color = vertex_color;"
        "}";

    const string Mesh::fragment_shader_code =

        "#version 330\n"
        ""
        "in  vec3    front_color;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = vec4(1.0, 0.0, 0.0, 1.0);"
        "}";

    
    Mesh::Mesh(const string & mesh_file_path)
    {
        program_id = compile_shaders();
        
        model_view_matrix_id = glGetUniformLocation (program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (program_id, "projection_matrix");
        
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

    void Mesh::render(shared_ptr<Camera> camera)
    {
        angle += 0.01f;
    
        glUseProgram(program_id);
        
        if (camera)
        {
            glm::mat4 projection_matrix = camera->get_projection_matrix();
            glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
            
            glm::mat4 model_view_matrix = camera->get_transform_matrix_inverse();
            model_view_matrix = glm::translate(model_view_matrix, glm::vec3(0.f, 0.f, -2.f));
            glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));            
        }
        else
        {
            glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(1024) / 640, 1.f, 5000.f);
            glm::mat4 model_view_matrix(1);

            model_view_matrix = translate  (model_view_matrix, glm::vec3(0.f, 0.f, -2.f));
            model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
            
            glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
            glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        }
        
        
        glBindVertexArray (vao_id);
        glDrawElements    (GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
            
        glBindVertexArray (0);
        glUseProgram(0); ///< TODO: mejorar el flujo del useprogram
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
            
            colors.resize(number_of_vertices);
            
            for (auto & color : colors)
            {
                color = {1.f, 1.f, 1.f};
            }
            
            glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLOR_VBO]);
            glBufferData (GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), colors.data(), GL_STATIC_DRAW);
            
            glEnableVertexAttribArray (1);
            glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
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
    
    GLuint Mesh::compile_shaders ()
    {
        GLint succeeded = GL_FALSE;

        // Se crean objetos para los shaders:

        GLuint   vertex_shader_id = glCreateShader (GL_VERTEX_SHADER  );
        GLuint fragment_shader_id = glCreateShader (GL_FRAGMENT_SHADER);

        // Se carga el código de los shaders:

        const char *   vertex_shaders_code[] = {          vertex_shader_code.c_str () };
        const char * fragment_shaders_code[] = {        fragment_shader_code.c_str () };
        const GLint    vertex_shaders_size[] = { (GLint)  vertex_shader_code.size  () };
        const GLint  fragment_shaders_size[] = { (GLint)fragment_shader_code.size  () };

        glShaderSource  (  vertex_shader_id, 1,   vertex_shaders_code,   vertex_shaders_size);
        glShaderSource  (fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        // Se compilan los shaders:

        glCompileShader (  vertex_shader_id);
        glCompileShader (fragment_shader_id);

        // Se comprueba que si la compilación ha tenido éxito:

        glGetShaderiv   (  vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (  vertex_shader_id);

        glGetShaderiv   (fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (fragment_shader_id);

        // Se crea un objeto para un programa:

        GLuint program_id = glCreateProgram ();

        // Se cargan los shaders compilados en el programa:

        glAttachShader  (program_id,   vertex_shader_id);
        glAttachShader  (program_id, fragment_shader_id);

        // Se linkan los shaders:

        glLinkProgram   (program_id);

        // Se comprueba si el linkage ha tenido éxito:

        glGetProgramiv  (program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error (program_id);

        // Se liberan los shaders compilados una vez se han linkado:

        glDeleteShader (  vertex_shader_id);
        glDeleteShader (fragment_shader_id);

        return (program_id);
    }

    void Mesh::show_compilation_error (GLuint shader_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetShaderiv (shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize (info_log_length);

        glGetShaderInfoLog (shader_id, info_log_length, NULL, &info_log.front ());

        cerr << info_log.c_str () << endl;

        assert(false);
    }

    void Mesh::show_linkage_error (GLuint program_id)
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
