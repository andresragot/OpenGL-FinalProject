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
