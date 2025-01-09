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
#include "Camera.hpp"
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
        Camera camera;
        
    private:
        
        mutex scene_mutex;
    
        int width;
        int height;
        
        float  angle_around_x;
        float  angle_around_y;
        float  angle_delta_x;
        float  angle_delta_y;
        
        float camera_speed = 0.0025f;

        bool   pointer_pressed;
        int    last_pointer_x;
        int    last_pointer_y;
        bool   turbo;
        
        float camera_turbo_speed = 2.f;
        
        glm::vec3 camera_translation;
        
    public:
        void resize (int width, int height);
        void on_drag (int pointer_x, int pointer_y);
        void on_click (int pointer_x, int pointer_y, bool down);
        void on_translation (glm::vec3 translation);
        void on_shift_pressed(bool down);
        
    public:
        
        void update();
        void render();
    
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
        Critical_Task scene_render;
        Light_Task    scene_update;
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
