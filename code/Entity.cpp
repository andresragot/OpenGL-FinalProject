//
//  Entity.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#include "Entity.hpp"

#include <map>
#include <memory>

namespace Ragot
{
    void Entity::add_components(shared_ptr < Component > component, const string & name)
    {
        component->set_entity(shared_from_this());
        
        if (!components.contains(name))
        {
            components.emplace(name, component);
        }
        else
        {
            components.insert_or_assign(name, component);
        }
    }
    
    void Entity::remove_component(const string & name)
    {
        auto iterator = components.find (name);
        
        if (iterator != components.end())
        {
            components.erase(iterator);
        }
    }
}
