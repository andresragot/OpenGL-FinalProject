//
//  MySystem.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 4/1/25.
//

#include "MySystem.hpp"

#include <iostream>
#include <string>

#include <glad/glad.h>
#include "Entity.hpp"

namespace Ragot
{
    System::System(const string & Window_Name, const int width, const int height)
    :
        window(Window_Name, Window::Position::CENTERED, Window::Position::CENTERED, width, height, { 3, 3 }),
        buffer_swap   ([this] { window.swap_buffers(); } ),
        handle_events ([this] { sdl_events(); }),
        scene_render  ([this] { scene.render(); }),
        scene_update  ([this] { scene.update(); }),
        process_events([this] { input(); })
    {
        initialize();
    }
    
    System::System()
    :
        window("Final Project", Window::Position::CENTERED, Window::Position::CENTERED, 2000, 640, { 3, 3 }),
        buffer_swap   ([this] { window.swap_buffers(); } ),
        handle_events ([this] { sdl_events(); }),
        scene_render  ([this] { scene.render(); }),
        scene_update  ([this] { scene.update(); }),
        process_events([this] { input(); })
    {
        initialize();
    }
    
    void System::initialize()
    {
        glViewport   (0, 0, window.get_width(), window.get_height());
        glEnable     (GL_CULL_FACE );
        glEnable     (GL_DEPTH_TEST);
        glClearColor (0.1f, 0.1f, 0.1f, 1.f);
    
        kernel.add (make_shared <Critical_Task> (buffer_swap   ));
        kernel.add (make_shared <Critical_Task> (scene_render  ));
        kernel.add (make_shared <Critical_Task> (handle_events ));
        kernel.add (make_shared <Light_Task   > (scene_update  ));
        kernel.add (make_shared <Light_Task   > (process_events));
    }
    
    void System::sdl_events()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            lock_guard<mutex> lock (queueMutex);
            eventQueue.push(event);
            
            queueCondition.notify_one();
        }
    }
    
    std::string getEventString(const SDL_Event& event)
    {
        switch (event.type)
        {
            case SDL_QUIT:
                return "SDL_QUIT";
            case SDL_KEYDOWN:
                return "SDL_KEYDOWN";
            case SDL_KEYUP:
                return "SDL_KEYUP";
            case SDL_MOUSEBUTTONDOWN:
                return "SDL_MOUSEBUTTONDOWN";
            case SDL_MOUSEBUTTONUP:
                return "SDL_MOUSEBUTTONUP";
            case SDL_MOUSEMOTION: return "SDL_MOUSEMOTION";
            // Agrega más casos según los tipos de eventos que desees manejar
            default:
                return "Unknown Event";
        }
    }
    
    void System::input()
    {
        while (true)
        {
            SDL_Event event;
            {
                unique_lock<mutex> lock (queueMutex);
                
                queueCondition.wait(lock, [this] {return !eventQueue.empty(); });
                
                event = eventQueue.front();
                
                eventQueue.pop();
            }
            
            string eventString = getEventString(event);
            cout << "Event: " << eventString << endl;
            
            if (event.type == SDL_QUIT)
            {
                stop();
            }
        }
    }
    
    void System::add_entities(shared_ptr<Entity> entity, const string & name)
    {
        scene.add_entities(entity, name);
        
        // Se recorre toda la lista de componentes para poder identificar cuales tienen listas que tienen que ser agregaadas al kernel.
        for (const auto & it : entity->components)
        {
            if (it.second->get_has_task())
            {
                auto component = it.second;
                
                if (auto skybox_component = std::dynamic_pointer_cast<Skybox_Component>(component))
                {
                    skybox_component->set_camera(scene.camera);
                    kernel.add (make_shared < Critical_Task > (skybox_component->render_task));
                }
                else if (auto mesh_component = std::dynamic_pointer_cast<Mesh_Component>(component))
                {
                    kernel.add (make_shared < Critical_Task > (mesh_component->render_task));
                }
            }
        }
    }
    
    void Scene::add_entities(shared_ptr<Entity> entity, const string & name)
    {
        if (!entities.contains(name))
        {
            entity->set_scene(this);
            entities.emplace (name, entity);
        }
    }
    
    void Scene::remove_entities(const string &name)
    {
        auto iterator = entities.find (name);
        
        if (iterator != entities.end())
        {
            entities.erase(iterator);
        }
    }
    
    
    
}
