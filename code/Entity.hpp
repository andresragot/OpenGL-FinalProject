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

#include "MySystem.hpp"
#include "Component.hpp"

namespace Ragot
{
class Entity : public std::enable_shared_from_this<Entity>
    {
        Scene * scene;
        
    public:
        Transform_Component transform;
    
    private:
        map <string, shared_ptr < Component > > components;
        vector < shared_ptr < Entity > > children;
        
    public:
        void set_scene (Scene * scene) { this->scene = scene; }
        
        const Scene * get_scene()       { return scene; }
        const Scene * get_scene() const { return scene; }
        
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
