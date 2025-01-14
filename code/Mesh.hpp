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
#include <glm.hpp>
#include <vector>
#include <string>
#include "Camera.hpp"
#include "Shader_Program.hpp"
#include "Color.hpp"
#include "Color_Buffer.hpp"

namespace Ragot
{
    using std::vector;

    /**
     * @brief Class for managing a 3D mesh.
     */
    class Mesh
    {
    protected:
        /**
         * @brief Enum for VBO indices.
         */
        enum
        {
            COORDINATES_VBO, ///< VBO index for coordinates.
            NORMALS_VBO, ///< VBO index for normals.
            TEXTURE_UVS_VBO, ///< VBO index for texture UVs.
            INDICES_EBO, ///< VBO index for indices.
            VBO_COUNT ///< Total number of VBOs.
        };

        vector<glm::vec3> coordinates; ///< Vector of vertex coordinates.
        vector<glm::vec3> normals; ///< Vector of vertex normals.
        vector<glm::vec2> texture_coords; ///< Vector of texture coordinates.
        vector<GLuint> indices; ///< Vector of indices.

    private:
        GLuint vbo_ids[VBO_COUNT]; ///< Array of VBO IDs.
        GLuint vao_id; ///< Vertex Array Object ID.

        GLsizei number_of_indices; ///< Number of indices.

        float angle; ///< Angle for transformations.

        /**
         * @brief Loads a mesh from a file.
         * @param mesh_file_path Path to the mesh file.
         */
        void load_mesh(const std::string& mesh_file_path);

    private:
        Mesh(const Mesh&) = delete; ///< Deleted copy constructor.
        Mesh& operator=(const Mesh&) = delete; ///< Deleted copy assignment operator.

    public:
        /**
         * @brief Default constructor for the Mesh class.
         */
        Mesh() = default;

        /**
         * @brief Constructor for the Mesh class.
         * @param mesh_file_path Path to the mesh file.
         */
        Mesh(const std::string& mesh_file_path);

        /**
         * @brief Destructor for the Mesh class.
         */
        ~Mesh()
        {
            glDeleteVertexArrays(1, &vao_id);
            glDeleteBuffers(VBO_COUNT, vbo_ids);
        }

    public:
        /**
         * @brief Gets the vertex coordinates.
         * @return Vector of vertex coordinates.
         */
        const vector<glm::vec3>& get_coordinates() const { return coordinates; }

        /**
         * @brief Gets the vertex normals.
         * @return Vector of vertex normals.
         */
        const vector<glm::vec3>& get_normals() const { return normals; }

        /**
         * @brief Gets the texture coordinates.
         * @return Vector of texture coordinates.
         */
        const vector<glm::vec2>& get_textures_uv() const { return texture_coords; }

        /**
         * @brief Gets the indices.
         * @return Vector of indices.
         */
        const vector<GLuint>& get_indices() const { return indices; }

        /**
         * @brief Gets the Vertex Array Object ID.
         * @return VAO ID.
         */
        const GLuint get_vao_id() const { return vao_id; }

        /**
         * @brief Gets the number of indices.
         * @return Number of indices.
         */
        const GLsizei get_number_of_indices() const { return number_of_indices; }
    };

    /**
     * @brief Template class for managing a 2D texture.
     * @tparam COLOR_FORMAT Color format of the texture.
     */
    template <typename COLOR_FORMAT>
    class Texture2D
    {
    protected:
        typedef Color_Buffer<COLOR_FORMAT> Color_Buffer; ///< Type alias for color buffer.

        GLuint texture_id; ///< Texture ID.
        bool texture_is_loaded; ///< Indicates if the texture is loaded.

    private:
        bool is_uint8 = false; ///< Flag to indicate if the texture uses uint8 format.

    public:
        /**
         * @brief Constructor for the Texture2D class.
         * @param texture_base_path Path to the base texture file.
         */
        Texture2D(const string& texture_base_path);

        /**
         * @brief Destructor for the Texture2D class.
         */
        ~Texture2D();

    private:
        Texture2D(const Texture2D&) = delete; ///< Deleted copy constructor.
        Texture2D& operator=(const Texture2D&) = delete; ///< Deleted copy assignment operator.

    protected:
        /**
         * @brief Default constructor for the Texture2D class.
         */
        Texture2D() : texture_id(0), texture_is_loaded(false) {}

    public:
        /**
         * @brief Checks if the texture is loaded.
         * @return True if the texture is loaded, false otherwise.
         */
        bool is_ok() const
        {
            return texture_is_loaded;
        }

        /**
         * @brief Binds the texture.
         * @return True if the texture is successfully bound, false otherwise.
         */
        virtual bool bind() const
        {
            return texture_is_loaded ? glBindTexture(GL_TEXTURE_2D, texture_id), true : false;
        }

    protected:
        /**
         * @brief Creates a 2D texture from a file.
         * @param texture_path Path to the texture file.
         * @return The texture ID.
         */
        GLint create_texture_2d(const string& texture_path);

        /**
         * @brief Loads an image from a file.
         * @param texture_path Path to the texture file.
         * @return Unique pointer to the color buffer.
         */
        unique_ptr<Color_Buffer> load_image(const string& texture_path);
    };

    /**
     * @brief Class for managing a cube texture.
     */
    class Texture_Cube : public Texture2D<Rgba8888>
    {
    public:
        /**
         * @brief Constructor for the Texture_Cube class.
         * @param texture_base_path Path to the base texture file.
         */
        Texture_Cube(const string& texture_base_path);

        /**
         * @brief Binds the cube texture.
         * @return True if the texture is successfully bound, false otherwise.
         */
        bool bind() const override
        {
            return texture_is_loaded ? glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id), true : false;
        }
    };

    /**
     * @brief Class for managing a material.
     */
    class Material
    {
    private:
        Shader_Program shader_program; ///< Shader program for the material.
        Texture2D<Rgba8888> texture; ///< Texture for the material.
        glm::vec3 color; ///< Color of the material.
        float shininess; ///< Shininess of the material.

    public:
        Material() = delete; ///< Deleted default constructor.

        /**
         * @brief Constructor for the Material class.
         * @param source_code_vertex Vector of vertex shader source code.
         * @param source_code_fragment Vector of fragment shader source code.
         * @param texture_base_path Path to the base texture file.
         */
        Material(const vector<string>& source_code_vertex, const vector<string>& source_code_fragment, const string& texture_base_path);

        /**
         * @brief Default destructor for the Material class.
         */
        ~Material() = default;

        /**
         * @brief Uses the shader program.
         */
        void use_shader_program() { shader_program.use(); }

        /**
         * @brief Gets the uniform location in the shader program.
         * @param uniform Name of the uniform.
         * @return Uniform location.
         */
        GLint get_shader_program_uniform_location(const string& uniform) { return shader_program.get_uniform_location(uniform); }

        /**
         * @brief Gets the shader program ID.
         * @return Shader program ID.
         */
        GLuint get_shader_program_id() const { return shader_program.get_id(); }

        /**
         * @brief Binds the texture.
         * @return True if the texture is successfully bound, false otherwise.
         */
        const bool bind_texture() const { return texture.bind(); }

        /**
         * @brief Gets the color of the material.
         * @return Color of the material.
         */
        const glm::vec3 get_color() { return color; }

        /**
         * @brief Gets the shininess of the material.
         * @return Shininess of the material.
         */
        const float get_shininess() { return shininess; }
    };
}
