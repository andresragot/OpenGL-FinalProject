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

    /**
     * @brief Class for managing the kernel that executes tasks.
     */
    class Kernel
    {
        vector<std::shared_ptr<Task>> tasks; ///< Vector of tasks.
        vector<std::shared_ptr<Critical_Task>> render_tasks; ///< Vector of critical render tasks.
        
        std::mutex tasks_mutex; ///< Mutex for synchronizing access to tasks.
    
        std::atomic<bool> exit; ///< Atomic flag to signal exit.
        std::atomic<bool> is_running; ///< Atomic flag to indicate if the kernel is running.
        
        std::condition_variable cv; ///< Condition variable for task control.
        
    public:
        /**
         * @brief Adds a new task to the kernel.
         * @param new_task Shared pointer to the new task to add.
         */
        void add(std::shared_ptr<Task> new_task);
        
        /**
         * @brief Runs the kernel, executing all tasks.
         */
        void run();
        
        /**
         * @brief Stops the kernel and all tasks.
         */
        void stop()
        {
            tasks.front()->stop_execution();
            exit = true;
            cv.notify_all();
        }
        
        /**
         * @brief Executes all critical functions at once.
         */
        void execute_critical();
    };
}
