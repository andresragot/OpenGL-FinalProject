//
//  Task.hpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 31/12/24.
//

#pragma once


#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

namespace project_source {

using std::condition_variable;
using std::mutex;
using std::atomic;
using std::function;
    
    class Task
    {
        static condition_variable cv;
        static mutex mtx;
        static atomic <bool> is_stop; // Variable para detener la tarea
        static atomic <bool> finish_execution;
    public:
        Task (function<void()> task_func) : task_func(task_func) {}
       ~Task () = default;
        
        virtual void execute () = 0;
        
        void stop_execution()
        {
            std::lock_guard <mutex> lock (mtx);
            finish_execution = true;
            cv.notify_all();
        }
        
        void stop()
        {
            std::lock_guard<mutex> lock (mtx);
            is_stop = true;
            cv.notify_all();
        }
        
        void resume()
        {
            std::lock_guard<mutex> lock (mtx);
            is_stop = false;
            cv.notify_all();
        }
        
    protected:
        bool shouldStop()
        {
            return is_stop.load();
        }
        
        bool shouldFinish()
        {
            return finish_execution.load();
        }
        
        void wait_for_resume()
        {
            std::unique_lock<mutex> lock (mtx);
            
            cv.wait(lock, [this] {return !shouldStop() || shouldFinish(); });
        }
        
    protected:
        function<void()> task_func;
    };

    class Light_Task : public Task
    {
    public:
        Light_Task (function<void()> task_func) : Task(task_func) {}
        
        void execute() override;
    };

    class Critical_Task : public Task
    {
    public:
        Critical_Task (function<void()> task_func) : Task(task_func) {}
        
        void execute() override;
    };

    class Once_Task : public Task
    {
    public:
        Once_Task (function<void()> task_func) : Task(task_func) {}
        
        void execute() override;
    };

    /*
     Si quiero más tasks agregar aquí
    */
}
