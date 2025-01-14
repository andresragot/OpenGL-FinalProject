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

namespace Ragot {

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
        /// Constructor que acepta la función que correrá esa Task
        Task (function<void()> task_func) : task_func(task_func) {}
        /// Destructor por defecto.
       ~Task () = default;
        
        /// Función que ejcutará virtualmente para que se divida entre todos los casos que tenemos.
        virtual void execute () = 0;
        
        /// esta funcion está hecha de cierta manera para que siempre incluso si lo ejecuta un hilo, se detienen todos.
        void stop_execution()
        {
            std::lock_guard <mutex> lock (mtx);
            finish_execution = true;
            cv.notify_all();
        }
        
        /// Función que hace que se detengan durante un tiempo por si hay una parte crítica del código.
        void stop()
        {
            std::lock_guard<mutex> lock (mtx);
            is_stop = true;
            cv.notify_all();
        }
        
        /// Función que hace que las ejecucuibes se reanuden.
        void resume()
        {
            std::lock_guard<mutex> lock (mtx);
            is_stop = false;
            cv.notify_all();
        }
        
    protected:
        /// Función que indica si se debería de detener la ejecución
        ///  @return regresará si se debe de detener o no
        bool shouldStop()
        {
            return is_stop.load();
        }
        
        /// Función que indica si se debería de finalizar la ejecución
        ///  @return regresará si se debe de finalizar o no
        bool shouldFinish()
        {
            return finish_execution.load();
        }
        
        ///  pequeña funcion para esperar una vez se pueda continuat con la ejecución
        void wait_for_resume()
        {
            std::unique_lock<mutex> lock (mtx);
            
            cv.wait(lock, [this] {return !shouldStop() || shouldFinish(); });
        }
        
    protected:
        function<void()> task_func;
    };

    /// @class Clase que ejecutará tareas cíclicas como lo son el Update o Input.
    class Light_Task : public Task
    {
    public:
        /// Constructor que llama al padre.
        Light_Task (function<void()> task_func) : Task(task_func) {}
        
        /// Función de ejecución específica.
        void execute() override;
    };

    /// @class Clase que ejecutará tareas críticas como lo son el Render que luego el Kernel podrá detener el resto de tareas para ejecutar estas.
    /// estas tareas se ejecutan todas en el hilo principal.
    class Critical_Task : public Task
    {
    public:
        /// Constructor que llama al constructor del padre
        Critical_Task (function<void()> task_func) : Task(task_func) {}
        
        /// Función de ejecución específica.
        void execute() override;
    };

    /// @class Tipo de tarea que solamente se ejecutará una única vez.
    class Once_Task : public Task
    {
    public:
        /// Constructor que llama al padre
        Once_Task (function<void()> task_func) : Task(task_func) {}
        
        /// Función de ejecución específica.
        void execute() override;
    };
}
