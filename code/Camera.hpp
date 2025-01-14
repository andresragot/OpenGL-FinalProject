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
    class Camera
    {
        using Point    = glm::vec4;
        using Vector   = glm::vec4;
        using Matrix44 = glm::mat4;

    private:

        float    fov;
        float    near_z;
        float    far_z;
        float    ratio;

        Point    location;
        Point    target;

        Matrix44 projection_matrix;

    public:

        Camera(float ratio = 1.f)
        {
            reset (60.f, 0.1f, 1000.f, ratio);
        }

        Camera(float near_z, float far_z, float ratio = 1.f)
        {
            reset (60.f, near_z, far_z, ratio);
        }

        Camera(float fov_degrees, float near_z, float far_z, float ratio)
        {
            reset (fov_degrees, near_z, far_z, ratio);
        }

    public:

        float         get_fov      () const { return fov;    }
        float         get_near_z   () const { return near_z; }
        float         get_far_z    () const { return far_z;  }
        float         get_ratio    () const { return ratio;  }

        const Point & get_location () const { return location; }
        const Point & get_target   () const { return target;   }

    public:

        void set_fov      (float new_fov   ) { fov    = new_fov;    calculate_projection_matrix (); }
        void set_near_z   (float new_near_z) { near_z = new_near_z; calculate_projection_matrix (); }
        void set_far_z    (float new_far_z ) { far_z  = new_far_z;  calculate_projection_matrix (); }
        void set_ratio    (float new_ratio ) { ratio  = new_ratio;  calculate_projection_matrix (); }

        void set_location (float x, float y, float z) { location[0] = x; location[1] = y; location[2] = z; }
        void set_target   (float x, float y, float z) { target  [0] = x; target  [1] = y; target  [2] = z; }

        void reset (float new_fov, float new_near_z, float new_far_z, float new_ratio)
        {
            set_fov      (new_fov   );
            set_near_z   (new_near_z);
            set_far_z    (new_far_z );
            set_ratio    (new_ratio );
            set_location (0.f,  0.f,  0.f);
            set_target   (0.f,  0.f, -1.f);
            calculate_projection_matrix ();
        }

    public:

        void move (const glm::vec3 & translation)
        {
            location += glm::vec4 (translation, 0.f);
            // target   += glm::vec4 (translation, 1.f);
        }

        void rotate (const glm::mat4 & rotation)
        {
            target = location + rotation * (target - location);
        }

    public:

        const glm::mat4 & get_projection_matrix () const
        {
            return projection_matrix;
        }

        glm::mat4 get_transform_matrix_inverse () const
        {
            return glm::lookAt
            (
                glm::vec3(location[0], location[1], location[2]),
                glm::vec3(target  [0], target  [1], target  [2]),
                glm::vec3(       0.0f,        1.0f,        0.0f)
            );
        }

    private:

        void calculate_projection_matrix ()
        {
            projection_matrix = glm::perspective (glm::radians (fov), ratio, near_z, far_z);
        }

    };
}
