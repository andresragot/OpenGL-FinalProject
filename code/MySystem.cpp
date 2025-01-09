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
        glEnable     (GL_CULL_FACE );
        glEnable     (GL_DEPTH_TEST);
        glClearColor (0.1f, 0.1f, 0.1f, 1.f);
        
        scene.resize (window.get_width(), window.get_height());
    
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
        int  mouse_x     = 0;
        int  mouse_y     = 0;
        bool button_down = false;
        
        vec3 camera_translation (0);
        
        while (true)
        {
            SDL_Event event;
            {
                unique_lock<mutex> lock (queueMutex);
                
                queueCondition.wait(lock, [this] {return !eventQueue.empty(); });
                
                event = eventQueue.front();
                
                eventQueue.pop();
            }
            
            switch (event.type)
            {
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_w:
                            camera_translation.z = -1.f;
                        break;
                        case SDLK_s:
                            camera_translation.z = +1.0f;
                        break;
                        case SDLK_a:
                            camera_translation.x = -1.f;
                        break;
                        case SDLK_d:
                            camera_translation.x = +1.0f;
                        break;
                        case SDLK_q:
                            camera_translation.y = -1.0f;
                        break;
                        case SDLK_e:
                            camera_translation.y = +1.0f;
                        break;
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT:
                            auto shift_task = make_shared < Once_Task > ([&] { scene.on_shift_pressed(true); });
                            kernel.add (shift_task);
                        break;
                    }
                    
                    auto keyboard = make_shared < Once_Task > ([&] {scene.on_translation (camera_translation);});
                    kernel.add (keyboard);
                    
                    break;
                }
                
                case SDL_KEYUP:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_w:
                        case SDLK_s:
                            camera_translation.z = 0.0f;
                        break;
                        
                        case SDLK_a:
                        case SDLK_d:
                            camera_translation.x = 0.0f;
                        break;
                        
                        case SDLK_q:
                        case SDLK_e:
                            camera_translation.y = 0.0f;
                        break;
                        
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT:
                            auto shift_task = make_shared < Once_Task > ([&] { scene.on_shift_pressed(false); });
                            kernel.add (shift_task);
                        break;
                        
                    }
                    
                    auto keyboard = make_shared < Once_Task > ([&] {scene.on_translation (camera_translation);});
                    kernel.add (keyboard);
                    
                    break;
                }
                
            
                case SDL_MOUSEBUTTONDOWN:
                {
                    auto   buttons = SDL_GetMouseState (&mouse_x, &mouse_y);
                    auto left_down = buttons &  SDL_BUTTON(SDL_BUTTON_LEFT);
                    
                    if (left_down && not button_down)
                    {
                        auto mouse_task = std::make_shared < Once_Task >([&] { scene.on_click (mouse_x, mouse_y, button_down = true); });
                        kernel.add (mouse_task);
                    }
                    
                    break;
                }
                
                case SDL_MOUSEBUTTONUP:
                {
                    auto   buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
                    auto left_down = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
                    
                    if (not left_down && button_down)
                    {
                        auto mouse_task = std::make_shared < Once_Task > ([&] { scene.on_click (mouse_x, mouse_y, button_down = false);});
                        kernel.add (mouse_task);
                    }
                    
                    break;
                }
                
                case SDL_MOUSEMOTION:
                {
                    SDL_GetMouseState (&mouse_x, &mouse_y);
                    auto mouse_task = std::make_shared < Once_Task > ([&] {scene.on_drag (mouse_x, mouse_y); });
                    kernel.add (mouse_task);
                    
                    break;
                }
                
                case SDL_QUIT:
                {
                    stop();
                    
                    break;
                }
                
                default:
                    break;
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
    
    void Scene::resize(int width, int height)
    {
        this->width = width;
        this->height = height;
    
        camera.set_ratio (float(width) / height);
        
        glViewport   (0, 0, width, height);
    }
    
    void Scene::on_drag(int pointer_x, int pointer_y)
    {
        lock_guard <mutex> lock (scene_mutex);
        if (pointer_pressed)
        {
            angle_delta_x = 0.0000025f * float (last_pointer_y - pointer_y) / float (height);
            angle_delta_y = 0.0000025f * float (last_pointer_x - pointer_x) / float (width );
        }
    }
    
    void Scene::on_click(int pointer_x, int pointer_y, bool down)
    {
        lock_guard <mutex> lock (scene_mutex);
        
        if ((pointer_pressed = down) == true)
        {
            last_pointer_x = pointer_x;
            last_pointer_y = pointer_y;
        }
        else
        {
            angle_delta_x = angle_delta_y = 0.0f;
        }
    }
    
    void Scene::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void Scene::update()
    {
        lock_guard <mutex> lock (scene_mutex);
        angle_around_x += angle_delta_x;
        angle_around_y += angle_delta_y;
        
        if (angle_around_x > +1.5f)
        {
            angle_around_x = +1.5f;
        }
        else
        if (angle_around_x < -1.4f)
        {
            angle_around_x = -1.4f;
        }
        
        vec3 camera_forward = glm::normalize (vec3 (camera.get_target()) - vec3 (camera.get_location()));
        vec3   camera_right = glm::normalize (glm::cross(camera_forward, vec3 (0.f, 1.f, 0.f)));
        vec3      camera_up = glm::vec3(0.f, 1.f, 0.f);
        
        vec3 new_translation = camera_translation.x * camera_right + camera_translation.y * camera_up - camera_translation.z * camera_forward;
        camera_translation = new_translation * camera_speed;
        
        if (turbo) camera_translation *= camera_turbo_speed;
        
        camera.move(camera_translation);
        
        // cout << "angle_x: " << angle_around_x << "\n" << "angle_y: " << angle_around_y << endl;
        
        glm::mat4 camera_rotation(1);
        
        camera_rotation = glm::rotate (camera_rotation, angle_around_y, glm::vec3(0.f, 1.f, 0.f));
        camera_rotation = glm::rotate (camera_rotation, angle_around_x, glm::vec3(1.f, 0.f, 0.f));
        
        camera.set_target (0, 0, -1);
        camera.rotate     (camera_rotation);

    }
    
    void Scene::on_translation (vec3 translation)
    {
        lock_guard <mutex> lock (scene_mutex);
    
        camera_translation = translation;
    }
    
    void Scene::add_entities(shared_ptr<Entity> entity, const string & name)
    {
        if (!entities.contains(name))
        {
            entity->set_scene(this);
            entities.emplace (name, entity);
        }
    }
    
    void Scene::on_shift_pressed(bool down)
    {
        lock_guard <mutex> lock (scene_mutex);
        
        turbo = down;
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
