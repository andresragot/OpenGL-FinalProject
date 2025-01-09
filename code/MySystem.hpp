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

#include <mutex>
#include <condition_variable>
#include <queue>

namespace Ragot
{

    using namespace std;
    
    // Declaración adelantada de Entity
    class Entity;

    class Scene
    {
    public:
        
    
    private:
        map <string, shared_ptr <Entity>> entities;
        
    public:
        void    add_entities (shared_ptr<Entity> entity, const string & name);
        void remove_entities (const string & name);
    };
    
    class System
    {
        Critical_Task buffer_swap;
        Critical_Task handle_events;
        Light_Task    process_events;
        
        queue<SDL_Event> eventQueue;
        mutex queueMutex;
        condition_variable queueCondition;
        
        Scene scene;
        
        Kernel kernel;
        
    private:
        void input();
        void initialize();
        void sdl_events();
        
    public:
        Window window;
        System(const string & Window_Name, const int width, const int height);
        System();
        
    public:
        void add_entities (shared_ptr<Entity> entity, const string & name);
    
        void run()
        {
            kernel.run();
        }
    
        void stop()
        {
            kernel.stop();
        }
    };
    
}
