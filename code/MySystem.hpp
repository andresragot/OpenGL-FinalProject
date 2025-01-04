//
//  MySystem.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#pragma once

#include "Window.hpp"
#include "Task.hpp"
#include "MyKernel.hpp"
#include <string>
#include <memory>
#include <map>

using std::string;
using std::shared_ptr;
using std::map;

namespace Ragot
{
    // Declaración adelantada de Entity
    class Entity;

    class Scene
    {
        map <string, shared_ptr <Entity>> entities;
    };
    
    class System
    {
        Window window;
        Critical_Task buffer_swap;
        Light_Task input_task;
        
        Scene scene;
        
        Kernel kernel;
    };
    
}
