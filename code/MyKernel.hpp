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

#include "Task.hpp"
#include <thread>
#include <vector>
#include <memory>
#include <mutex>


namespace Ragot
{
    using std::vector;

    class Kernel
    {
        vector < std::shared_ptr < Task > > tasks;
        vector < std::shared_ptr < Critical_Task > > render_tasks;
        
        std::mutex tasks_mutex;
    
        std::atomic<bool> exit;
        std::atomic <bool> is_running;
        
        std::condition_variable cv;
        
    public:
        /// Función que agrega todas las Tasks a el núcleo.
        /// @param new_task este parámetro define la nueva Tarea que agregaremos a la cola.
        void add (std::shared_ptr <Task> new_task);
        
        /// Esta función es necesaria que se llame para que todas las funciones se hagan
        void run();
        
        /// Esta función va a hacer que todo el Kernel se detenga
        void stop ()
        {
            tasks.front()->stop_execution();
            exit = true;
            cv.notify_all();
        }
        
        ///  Función que va a correr todas las funciones criticas del código a la vez.
        void execute_critical();
    };
}
