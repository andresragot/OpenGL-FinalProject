//
//  Component.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#include "MySystem.hpp"
#include <glm.hpp>

using glm::vec3;
using glm::mat4;

namespace Ragot
{
    class Component
    {
        Entity * entity;
    };
    
    class Transform_Component : public Component
    {
        vec3 position;
        vec3 rotation;
        vec3 scale;
        
        Transform_Component * parent;
        
        mat4 get_transform_matrix ()
        {
            mat4 transform_matrix (1);
            
            if (parent)
                transform_matrix = parent->get_transform_matrix() * transform_matrix;
            
            return transform_matrix;
        }
    };
}
