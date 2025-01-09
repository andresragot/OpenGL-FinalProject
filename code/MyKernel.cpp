//
//  MyKernel.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 31/12/24.
//

#include "MyKernel.hpp"


namespace Ragot {

    void Kernel::add(Task *new_task)
    {
        if (auto render_task = dynamic_cast<Critical_Task *> (new_task))
        {
            render_tasks.push_back (render_task);
        }
        else
        {
           tasks.push_back (new_task);
                   
            if (is_running)
            {
                std:: thread unique (&Task::execute, new_task);
                
                unique.join();
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
