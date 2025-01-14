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

#include "Camera.hpp"
#include "Shader_Program.hpp"
#include "Mesh.hpp"
#include "Color.hpp"

#include <glad/glad.h>
#include <string>

namespace Ragot
{
    using namespace std;

    /**
     * @class Skybox
     * @brief Class for rendering a skybox in the scene.
     */
    class Skybox
    {
    private:
        Shader_Program shader_program; ///< Shader program for the skybox.
        static const GLfloat coordinates[]; ///< Array of coordinates for the skybox vertices.
        static const string vertex_shader_code; ///< Vertex shader code.
        static const string fragment_shader_code; ///< Fragment shader code.
        GLuint vbo_id; ///< Vertex Buffer Object ID.
        GLuint vao_id; ///< Vertex Array Object ID.
        GLint model_view_matrix_id; ///< Model-view matrix uniform location.
        GLint projection_matrix_id; ///< Projection matrix uniform location.
        shared_ptr<Camera> camera = nullptr; ///< Shared pointer to the camera.
        Texture_Cube texture_cube; ///< Texture cube for the skybox.

    public:
        /**
         * @brief Constructor for the Skybox class.
         * @param texture_path Path to the texture for the skybox.
         */
        Skybox(const string & texture_path);

        /**
         * @brief Destructor for the Skybox class.
         */
        ~Skybox();

        /**
         * @brief Sets the camera for the skybox.
         * @param cam Shared pointer to the camera.
         */
        void set_camera(shared_ptr<Camera> cam) { camera = cam; }

        /**
         * @brief Renders the skybox.
         */
        void render();
    };

    /**
     * @class Terrain
     * @brief Class for rendering a terrain in the scene.
     */
    class Terrain
    {
    private:
        enum
        {
            COORDINATES_VBO, ///< VBO for coordinates.
            TEXTURE_UVS_VBO, ///< VBO for texture UVs.
            INDICES_EBO, ///< EBO for indices.
            VBO_COUNT ///< Number of VBOs.
        };

        Shader_Program shader_program; ///< Shader program for the terrain.
        static const string vertex_shader_code; ///< Vertex shader code.
        static const string fragment_shader_code; ///< Fragment shader code.
        GLsizei number_of_vertices; ///< Number of vertices.
        GLsizei number_of_indices; ///< Number of indices.
        GLuint vbo_ids[VBO_COUNT]; ///< Array of VBO IDs.
        GLuint vao_id; ///< Vertex Array Object ID.
        GLint model_view_matrix_id; ///< Model-view matrix uniform location.
        GLint projection_matrix_id; ///< Projection matrix uniform location.
        GLint view_position_id; ///< View position uniform location.
        GLint light_position_id; ///< Light position uniform location.
        shared_ptr< Camera > camera = nullptr; ///< Shared pointer to the camera.
        Texture2D< Monochrome8 > texture; ///< Texture for the terrain.

    public:
        /**
         * @brief Constructor for the Terrain class.
         * @param width Width of the terrain.
         * @param depth Depth of the terrain.
         * @param x_slices Number of slices along the x-axis.
         * @param z_slices Number of slices along the z-axis.
         */
        Terrain(float width, float depth, unsigned x_slices, unsigned z_slices);

        /**
         * @brief Destructor for the Terrain class.
         */
        ~Terrain();

        /**
         * @brief Sets the camera for the terrain.
         * @param cam Shared pointer to the camera.
         */
        void set_camera(shared_ptr<Camera> cam) { camera = cam; }

        /**
         * @brief Renders the terrain.
         */
        void render();
    };

    /**
     * @class Light
     * @brief Base class for different types of lights.
     */
    class Light
    {
    public:
        glm::vec3 color; ///< Color of the light.

        /**
         * @brief Constructor for the Light class.
         * @param color Color of the light.
         */
        Light(const glm::vec3 & color) : color(color) {}

        /**
         * @brief Virtual destructor for the Light class.
         */
        virtual ~Light() = default;
    };

    /**
     * @class DirectionalLight
     * @brief Class for directional light.
     */
    class DirectionalLight : public Light
    {
    public:
        glm::vec3 direction; ///< Direction of the light.

        /**
         * @brief Constructor for the DirectionalLight class.
         * @param color Color of the light.
         * @param direction Direction of the light.
         */
        DirectionalLight(const glm::vec3 & color, const glm::vec3 direction)
            : Light(color), direction(direction) {}
    };

    /**
     * @class PointLight
     * @brief Class for point light.
     */
    class PointLight : public Light
    {
    public:
        glm::vec3 position; ///< Position of the light.

        /**
         * @brief Constructor for the PointLight class.
         * @param color Color of the light.
         * @param position Position of the light.
         */
        PointLight(const glm::vec3 & color, const glm::vec3 & position)
            : Light(color), position(position) {}
    };

    /**
     * @class AreaLight
     * @brief Class for area light.
     */
    class AreaLight : public Light
    {
    public:
        glm::vec3 position; ///< Position of the light.
        glm::vec3 size; ///< Size of the area light.

        /**
         * @brief Constructor for the AreaLight class.
         * @param color Color of the light.
         * @param position Position of the light.
         * @param size Size of the area light.
         */
        AreaLight(const glm::vec3 & color, const glm::vec3 & position, const glm::vec3 & size)
            : Light(color), position(position), size(size) {}
    };
}
