//
//  Entity.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#pragma once

#include "MySystem.hpp"
#include "Component.hpp"

namespace Ragot
{
    class Entity
    {
        Scene * scene;
        
        Transform_Component transform;
    
    public:
        map <string, shared_ptr < Component > > components;
    };

}
