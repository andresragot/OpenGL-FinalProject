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
        
    public:
        Transform_Component transform;
    
    private:
        map <string, shared_ptr < Component > > components;
        vector < shared_ptr < Entity > > children;
        
    public:
        void set_scene (Scene * scene) { this->scene = scene; }
        
        void add_components (shared_ptr < Component > component, const string & name);
        
        void remove_component (const string & name);
        
        void add_child (shared_ptr < Entity > child)
        {
            child->set_transform_parent (&transform);
            children.push_back(child);
        }
        
        void remove_child (shared_ptr < Entity > child)
        {
            child->set_transform_parent (nullptr);
            children.erase (remove (children.begin(), children.end(), child), children.end());
        }
        
        void set_transform_parent (Transform_Component * parent)
        {
            transform.set_parent (parent);
        }
                
        const map < string, shared_ptr < Component > > & get_components() const { return components; }
    };

}
