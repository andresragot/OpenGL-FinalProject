//
//  MyKernel.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 31/12/24.
//

#pragma once

#include "Task.hpp"
#include <thread>
#include <vector>


namespace Ragot
{
    using std::vector;

    class Kernel
    {
        vector < Task * > tasks;
        vector < Critical_Task * > render_tasks;
    
        std::atomic<bool> exit;
        std::atomic <bool> is_running;
        
        std::condition_variable cv;
        
    public:
        /// Función que agrega todas las Tasks a el núcleo.
        /// @param new_task este parámetro define la nueva Tarea que agregaremos a la cola.
        void add (Task * new_task);
        
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
