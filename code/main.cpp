//
//  main.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 31/12/24.
//

/*#include <iostream>
#include <thread>
#include "Task.hpp"
#include "MyKernel.hpp"

using namespace Ragot;

// Colores para debuggear.
const char*    RED = "\033[1;31m";
const char* YELLOW = "\033[1;33m";
const char*  RESET = "\033[0m";
const char*  GREEN = "\033[1;32m";

int main (int , char * [])
{
    /// TODO: Desarrollar el resto de Componentes:
    ///  * Mesh
    ///  * Camera
    ///  * Transform
    ///
    /// TODO: Luego hacer todos los controllers.
    /// 
    
    return 0;
}*/

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

#include <iostream>
#include <SDL.h>
#include <glad/glad.h>
#include "Entity.hpp"
#include "MySystem.hpp"
#include "Component.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Texture_Cube.hpp"
#include "Mesh.hpp"
#include <SOIL2.h>
#include <memory>
#include "Color_Buffer.hpp"

using namespace Ragot;
using namespace std;

int main(int argc, char* argv[])
{
    System my_system;
    
    auto entity = make_shared < Entity >();

    auto skybox_component = std::make_shared < Skybox_Component > ("fotos/sky-cube-map-");
    Camera camera;
    skybox_component->set_camera(camera);
    
    entity->add_components(skybox_component, "Skybox");
        
    my_system.add_entities( entity, "Skybox" );
    
    my_system.run();
    
    SDL_Quit();
    return 0;
}
