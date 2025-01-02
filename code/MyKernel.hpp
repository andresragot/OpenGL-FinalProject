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


namespace project_source
{
    using std::vector;

    class Kernel
    {
        // Dividimos ya que uno se dedicará a tener
        
        // TODO: incluir las once tasks si ya está running
        vector < Task * > tasks;
        vector < Critical_Task * > render_tasks;
    
        std::atomic<bool> exit;
        std::atomic <bool> is_running;
        
        std::condition_variable cv;
        
    public:
        void add (Task * new_task);
        
        void run();
        
        void stop ()
        {
            tasks.front()->stop_execution();
            exit = true;
            cv.notify_all();
        }
        
        void execute_critial(); 
    };
}
