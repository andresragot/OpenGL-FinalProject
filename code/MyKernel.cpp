//
//  MyKernel.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 31/12/24.
//

#include "MyKernel.hpp"

namespace Ragot
{
    using namespace std;
    
    void Kernel::add (shared_ptr <Task> new_task)
    {
        lock_guard<mutex> lock(tasks_mutex);
        if (auto render_task = dynamic_pointer_cast<Critical_Task>(new_task))
        {
            render_tasks.push_back (render_task);
        }
        else
        {
           tasks.push_back (new_task);
                   
            if (is_running)
            {
                std:: thread unique (&Task::execute, new_task);
                
                unique.detach();
            }
        }
    }

    void Kernel::run()
    {
        if (is_running)
            return;
        
        std::vector<std::thread> threads;
        
        for (auto task : tasks)
        {
            threads.emplace_back(&Task::execute, task);
        }
        
        for (auto& t : threads)
        {
            t.detach();
        }
        
        is_running = true;
                
        execute_critical();
    }

    void Kernel::execute_critical()
    {
        exit = false;
        do
        {
            render_tasks.front()->stop();
            for (auto task : render_tasks) task->execute();
            render_tasks.front()->resume();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        while (not exit);
    }
}
