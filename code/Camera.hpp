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

#include <glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr


namespace Ragot
{
    /**
     * @brief Class for managing a camera in OpenGL.
     */
    class Camera
    {
        using Point = glm::vec4; ///< Type alias for points in 4D space.
        using Vector = glm::vec4; ///< Type alias for vectors in 4D space.
        using Matrix44 = glm::mat4; ///< Type alias for 4x4 matrices.

    private:
        float fov; ///< Field of view in degrees.
        float near_z; ///< Near clipping plane.
        float far_z; ///< Far clipping plane.
        float ratio; ///< Aspect ratio.

        Point location; ///< Location of the camera.
        Point target; ///< Target point the camera is looking at.

        Matrix44 projection_matrix; ///< Projection matrix.

    public:
        /**
         * @brief Constructor with default ratio.
         * @param ratio Aspect ratio (default is 1.0f).
         */
        Camera(float ratio = 1.f)
        {
            reset(60.f, 0.1f, 1000.f, ratio);
        }

        /**
         * @brief Constructor with near and far clipping planes.
         * @param near_z Near clipping plane.
         * @param far_z Far clipping plane.
         * @param ratio Aspect ratio (default is 1.0f).
         */
        Camera(float near_z, float far_z, float ratio = 1.f)
        {
            reset(60.f, near_z, far_z, ratio);
        }

        /**
         * @brief Constructor with field of view, near and far clipping planes, and aspect ratio.
         * @param fov_degrees Field of view in degrees.
         * @param near_z Near clipping plane.
         * @param far_z Far clipping plane.
         * @param ratio Aspect ratio.
         */
        Camera(float fov_degrees, float near_z, float far_z, float ratio)
        {
            reset(fov_degrees, near_z, far_z, ratio);
        }

        /**
         * @brief Gets the field of view.
         * @return Field of view in degrees.
         */
        float get_fov() const { return fov; }

        /**
         * @brief Gets the near clipping plane.
         * @return Near clipping plane.
         */
        float get_near_z() const { return near_z; }

        /**
         * @brief Gets the far clipping plane.
         * @return Far clipping plane.
         */
        float get_far_z() const { return far_z; }

        /**
         * @brief Gets the aspect ratio.
         * @return Aspect ratio.
         */
        float get_ratio() const { return ratio; }

        /**
         * @brief Gets the location of the camera.
         * @return Location of the camera.
         */
        const Point & get_location() const { return location; }

        /**
         * @brief Gets the target point the camera is looking at.
         * @return Target point.
         */
        const Point & get_target() const { return target; }

        /**
         * @brief Sets the field of view and recalculates the projection matrix.
         * @param new_fov New field of view in degrees.
         */
        void set_fov(float new_fov) { fov = new_fov; calculate_projection_matrix(); }

        /**
         * @brief Sets the near clipping plane and recalculates the projection matrix.
         * @param new_near_z New near clipping plane.
         */
        void set_near_z(float new_near_z) { near_z = new_near_z; calculate_projection_matrix(); }

        /**
         * @brief Sets the far clipping plane and recalculates the projection matrix.
         * @param new_far_z New far clipping plane.
         */
        void set_far_z(float new_far_z) { far_z = new_far_z; calculate_projection_matrix(); }

        /**
         * @brief Sets the aspect ratio and recalculates the projection matrix.
         * @param new_ratio New aspect ratio.
         */
        void set_ratio(float new_ratio) { ratio = new_ratio; calculate_projection_matrix(); }

        /**
         * @brief Sets the location of the camera.
         * @param x X-coordinate of the location.
         * @param y Y-coordinate of the location.
         * @param z Z-coordinate of the location.
         */
        void set_location(float x, float y, float z) { location[0] = x; location[1] = y; location[2] = z; }

        /**
         * @brief Sets the target point the camera is looking at.
         * @param x X-coordinate of the target.
         * @param y Y-coordinate of the target.
         * @param z Z-coordinate of the target.
         */
        void set_target(float x, float y, float z) { target[0] = x; target[1] = y; target[2] = z; }

        /**
         * @brief Resets the camera with new parameters and recalculates the projection matrix.
         * @param new_fov New field of view in degrees.
         * @param new_near_z New near clipping plane.
         * @param new_far_z New far clipping plane.
         * @param new_ratio New aspect ratio.
         */
        void reset(float new_fov, float new_near_z, float new_far_z, float new_ratio)
        {
            set_fov(new_fov);
            set_near_z(new_near_z);
            set_far_z(new_far_z);
            set_ratio(new_ratio);
            set_location(0.f, 0.f, 0.f);
            set_target(0.f, 0.f, -1.f);
            calculate_projection_matrix();
        }

        /**
         * @brief Moves the camera by a given translation vector.
         * @param translation Translation vector.
         */
        void move(const glm::vec3 & translation)
        {
            location += glm::vec4(translation, 0.f);
            // target   += glm::vec4(translation, 1.f);
        }

        /**
         * @brief Rotates the camera by a given rotation matrix.
         * @param rotation Rotation matrix.
         */
        void rotate(const glm::mat4 & rotation)
        {
            target = location + rotation * (target - location);
        }

        /**
         * @brief Gets the projection matrix of the camera.
         * @return Projection matrix.
         */
        const glm::mat4 & get_projection_matrix() const
        {
            return projection_matrix;
        }

        /**
         * @brief Gets the inverse of the transformation matrix for the camera.
         * @return Inverse of the transformation matrix.
         */
        glm::mat4 get_transform_matrix_inverse() const
        {
            return glm::lookAt(
                glm::vec3(location[0], location[1], location[2]),
                glm::vec3(target[0], target[1], target[2]),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }

    private:
        /**
         * @brief Calculates the projection matrix based on the current parameters.
         */
        void calculate_projection_matrix()
        {
            projection_matrix = glm::perspective(glm::radians(fov), ratio, near_z, far_z);
        }
    };
}
