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
#include "Ambient.hpp"
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
    private:
        shared_ptr<Camera> camera = make_shared < Camera > ();
        map <string, shared_ptr <Entity>> entities;
        Skybox skybox;
        vector < shared_ptr < Light > > lights;
        
    private:
        
        mutex scene_mutex;
    
        int width;
        int height;
        
        float  angle_around_x;
        float  angle_around_y;
        float  angle_delta_x;
        float  angle_delta_y;
        
        float camera_speed = 0.025f;

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
        
        
    public:
        Scene();
    
        
    public:
        void    add_entities (shared_ptr<Entity> entity, const string & name);
        void remove_entities (const string & name);
        shared_ptr < Entity > get_entity (const string & name) const;
        shared_ptr < Camera > get_camera () const { return camera; }
        
    private:
        void set_lights (GLuint shader_program_id);
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
        
        Window window;
        Scene scene;
        
        Kernel kernel;
        
    private:
        void input();
        void initialize();
        void sdl_events();
        
    public:
        System(const string & Window_Name, const int width, const int height);
        System();
       ~System() { SDL_Quit(); }
        
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
