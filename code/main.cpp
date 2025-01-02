//
//  main.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 31/12/24.
//

#include <iostream>
#include <thread>
#include "Task.hpp"
#include "MyKernel.hpp"

void input_function()
{
    std::cout << "InputTask" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));// Simulación de trabajo
}

void render_function()
{
    std::cout << "RenderTask" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(2));// Simulación de trabajo
    
    std::cout << "Finish RenderTask" << std::endl;
}

void update_function()
{
    std::cout << "UpdateTask" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulación de trabajo
}

using namespace project_source;

const std::string RED = "\033";

int main (int , char * [])
{
    Light_Task input(input_function);
    Critical_Task render(render_function);
    Once_Task update (update_function);

    Kernel my_kernel;
    
    my_kernel.add(&input);
    my_kernel.add(&render);
    my_kernel.add(&update);
    
    // std::thread input_t (&Light_Task::execute, &input);
    // std::thread render_t (&Critical_Task::execute, &render);
    // std::thread update_t (&Light_Task::execute, &update);
    
    std::thread kernel_thread (&Kernel::run, &my_kernel);
    
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    std::cout << RED << "Stopping Kernel" << std::endl;

    Once_Task close ([&my_kernel]() {my_kernel.stop();});
    
    kernel_thread.join();
    
    // my_kernel.stop();
    // input.stop_execution();
    
    //input_t.join();
    //render_t.join();
    //update_t.join();
    
    return 0;
}

/*// constructing atomics
#include <iostream>       // std::cout
#include <atomic>         // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <thread>         // std::thread, std::this_thread::yield
#include <vector>         // std::vector

std::atomic<bool> ready (false);
std::atomic_flag winner = ATOMIC_FLAG_INIT;

void count1m (int id) {
  while (!ready) { std::this_thread::yield(); }      // wait for the ready signal
  for (volatile int i=0; i<1000000; ++i) {}          // go!, count to 1 million
  if (!winner.test_and_set()) { std::cout << "thread #" << id << " won!\n"; }
};

int main ()
{
  std::vector<std::thread> threads;
  std::cout << "spawning 10 threads that count to 1 million...\n";
  for (int i=1; i<=10; ++i) threads.push_back(std::thread(count1m,i));
  ready = true;
  for (auto& th : threads) th.join();

  return 0;
}
*/
