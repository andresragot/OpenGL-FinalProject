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

#include "Window.hpp"
#include "Task.hpp"
#include "MyKernel.hpp"
#include "Camera.hpp"
#include "Ambient.hpp"
#include "Postprocess.hpp"
#include <string>
#include <memory>
#include <map>
#include <vector>

#include <mutex>
#include <condition_variable>
#include <queue>

namespace Ragot
{
    using namespace std;
    
    // Declaración adelantada de Entity
    class Entity;

    /**
     * @brief Class for managing a scene in OpenGL.
     */
    class Scene
    {
    private:
        shared_ptr<Camera> camera = make_shared<Camera>(); ///< Shared pointer to the camera.
        map<string, shared_ptr<Entity>> entities; ///< Map of entities in the scene.
        Frame_Buffer framebuffer; ///< Frame buffer for the scene.
        Skybox skybox; ///< Skybox for the scene.
        Terrain terrain; ///< Terrain for the scene.
        vector<shared_ptr<Light>> lights; ///< Vector of lights in the scene.
        
        mutex scene_mutex; ///< Mutex for synchronizing access to the scene.
    
        int width; ///< Width of the scene.
        int height; ///< Height of the scene.
        
        float angle_around_x; ///< Angle around the x-axis.
        float angle_around_y; ///< Angle around the y-axis.
        float angle_delta_x; ///< Delta angle around the x-axis.
        float angle_delta_y; ///< Delta angle around the y-axis.
        
        float camera_speed = 0.025f; ///< Speed of the camera.

        bool pointer_pressed; ///< Indicates if the pointer is pressed.
        int last_pointer_x; ///< Last x-coordinate of the pointer.
        int last_pointer_y; ///< Last y-coordinate of the pointer.
        bool turbo; ///< Indicates if turbo mode is enabled.
        
        float camera_turbo_speed = 2.f; ///< Turbo speed of the camera.
        
        glm::vec3 camera_translation; ///< Translation vector for the camera.

    public:
        /**
         * @brief Resizes the scene.
         * @param width New width of the scene.
         * @param height New height of the scene.
         */
        void resize(int width, int height);

        /**
         * @brief Handles pointer drag events.
         * @param pointer_x X-coordinate of the pointer.
         * @param pointer_y Y-coordinate of the pointer.
         */
        void on_drag(int pointer_x, int pointer_y);

        /**
         * @brief Handles pointer click events.
         * @param pointer_x X-coordinate of the pointer.
         * @param pointer_y Y-coordinate of the pointer.
         * @param down Indicates if the pointer is pressed down.
         */
        void on_click(int pointer_x, int pointer_y, bool down);

        /**
         * @brief Handles translation events.
         * @param translation Translation vector.
         */
        void on_translation(glm::vec3 translation);

        /**
         * @brief Handles shift key press events.
         * @param down Indicates if the shift key is pressed down.
         */
        void on_shift_pressed(bool down);

    public:
        /**
         * @brief Updates the scene.
         */
        void update();

        /**
         * @brief Renders the scene.
         */
        void render();

        /**
         * @brief Post-processes the scene.
         */
        void postproccess();

    public:
        /**
         * @brief Constructor for the Scene class.
         */
        Scene();

    public:
        /**
         * @brief Adds an entity to the scene.
         * @param entity Shared pointer to the entity.
         * @param name Name of the entity.
         */
        void add_entities(shared_ptr<Entity> entity, const string& name);

        /**
         * @brief Removes an entity from the scene.
         * @param name Name of the entity.
         */
        void remove_entities(const string& name);

        /**
         * @brief Gets an entity from the scene.
         * @param name Name of the entity.
         * @return Shared pointer to the entity.
         */
        shared_ptr<Entity> get_entity(const string& name) const;

        /**
         * @brief Gets the camera of the scene.
         * @return Shared pointer to the camera.
         */
        shared_ptr<Camera> get_camera() const { return camera; }

    private:
        /**
         * @brief Sets the lights in the scene.
         * @param shader_program_id Shader program ID.
         */
        void set_lights(GLuint shader_program_id);
    };

    /**
     * @brief Class for managing the system in OpenGL.
     */
    class System
    {
        Critical_Task buffer_swap; ///< Task for swapping buffers.
        Critical_Task handle_events; ///< Task for handling events.
        Critical_Task scene_render; ///< Task for rendering the scene.
        Light_Task scene_update; ///< Task for updating the scene.
        Light_Task process_events; ///< Task for processing events.
        
        queue<SDL_Event> eventQueue; ///< Queue of SDL events.
        mutex queueMutex; ///< Mutex for synchronizing access to the event queue.
        condition_variable queueCondition; ///< Condition variable for the event queue.
        
        Window window; ///< Window for rendering.
        Scene scene; ///< Scene to be rendered.
        
        Kernel kernel; ///< Kernel for managing tasks.

    private:
        /**
         * @brief Handles input events.
         */
        void input();

        /**
         * @brief Initializes the system.
         */
        void initialize();

        /**
         * @brief Handles SDL events.
         */
        void sdl_events();

    public:
        /**
         * @brief Constructor for the System class.
         * @param Window_Name Name of the window.
         * @param width Width of the window.
         * @param height Height of the window.
         */
        System(const string& Window_Name, const int width, const int height);

        /**
         * @brief Default constructor for the System class.
         */
        System();

        /**
         * @brief Destructor for the System class.
         */
        ~System() { SDL_Quit(); }

    public:
        /**
         * @brief Adds an entity to the system.
         * @param entity Shared pointer to the entity.
         * @param name Name of the entity.
         */
        void add_entities(shared_ptr<Entity> entity, const string& name);

        /**
         * @brief Runs the system.
         */
        void run()
        {
            kernel.run();
        }

        /**
         * @brief Stops the system.
         */
        void stop()
        {
            kernel.stop();
        }
    };
}
