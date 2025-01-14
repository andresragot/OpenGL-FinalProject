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


#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

namespace Ragot
{
    using std::condition_variable;
    using std::mutex;
    using std::atomic;
    using std::function;

    /**
     * @brief Base class for managing tasks.
     */
    class Task
    {
        static condition_variable cv; ///< Condition variable for task control.
        static mutex mtx; ///< Mutex for task synchronization.
        static atomic<bool> is_stop; ///< Variable to stop the task.
        static atomic<bool> finish_execution; ///< Variable to finish execution.

    public:
        /**
         * @brief Constructor that accepts the function to run for this task.
         * @param task_func Function to run for this task.
         */
        Task(function<void()> task_func) : task_func(task_func) {}

        /**
         * @brief Default destructor.
         */
        virtual ~Task() = default;

        /**
         * @brief Virtual function to execute the task, to be overridden by derived classes.
         */
        virtual void execute() = 0;

        /**
         * @brief Stops execution of all tasks, even if executed by one thread.
         */
        void stop_execution()
        {
            std::lock_guard<mutex> lock(mtx);
            finish_execution = true;
            cv.notify_all();
        }

        /**
         * @brief Stops execution temporarily for critical sections of code.
         */
        void stop()
        {
            std::lock_guard<mutex> lock(mtx);
            is_stop = true;
            cv.notify_all();
        }

        /**
         * @brief Resumes execution after a stop.
         */
        void resume()
        {
            std::lock_guard<mutex> lock(mtx);
            is_stop = false;
            cv.notify_all();
        }

    protected:
        /**
         * @brief Checks if the task should stop execution.
         * @return True if the task should stop, false otherwise.
         */
        bool shouldStop()
        {
            return is_stop.load();
        }

        /**
         * @brief Checks if the task should finish execution.
         * @return True if the task should finish, false otherwise.
         */
        bool shouldFinish()
        {
            return finish_execution.load();
        }

        /**
         * @brief Waits for resume signal to continue execution.
         */
        void wait_for_resume()
        {
            std::unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this] {return !shouldStop() || shouldFinish(); });
        }

    protected:
        function<void()> task_func; ///< Function to run for this task.
    };

    /**
     * @brief Class to execute cyclic tasks such as Update or Input.
     */
    class Light_Task : public Task
    {
    public:
        /**
         * @brief Constructor that calls the base class constructor.
         * @param task_func Function to run for this task.
         */
        Light_Task(function<void()> task_func) : Task(task_func) {}

        /**
         * @brief Specific execution function for light tasks.
         */
        void execute() override;
    };

    /**
     * @brief Class to execute critical tasks such as rendering, which the Kernel can pause other tasks to execute. These tasks run in the main thread.
     */
    class Critical_Task : public Task
    {
    public:
        /**
         * @brief Constructor that calls the base class constructor.
         * @param task_func Function to run for this task.
         */
        Critical_Task(function<void()> task_func) : Task(task_func) {}

        /**
         * @brief Specific execution function for critical tasks.
         */
        void execute() override;
    };

    /**
     * @brief Class for tasks that are executed only once.
     */
    class Once_Task : public Task
    {
    public:
        /**
         * @brief Constructor that calls the base class constructor.
         * @param task_func Function to run for this task.
         */
        Once_Task(function<void()> task_func) : Task(task_func) {}

        /**
         * @brief Specific execution function for once-only tasks.
         */
        void execute() override;
    };
}

