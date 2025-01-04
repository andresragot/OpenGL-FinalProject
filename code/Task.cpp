//
//  Task.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 31/12/24.
//

#include <thread>
#include "Task.hpp"

namespace Ragot
{
    /*
        Declaration de variables estáticas.
     */
    
    atomic<bool> Task::          is_stop (false);
    atomic<bool> Task:: finish_execution (false);
    
    condition_variable Task::cv;
    
    mutex Task::mtx;

    void Light_Task::execute()
    {
        while (not shouldFinish())
        {
            wait_for_resume();
            task_func();
        }
    }

    void Critical_Task::execute()
    {
            task_func();
    }
    
    void Once_Task::execute()
    {
        wait_for_resume();
        task_func();
    }
}
