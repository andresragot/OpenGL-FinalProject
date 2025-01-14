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
 
#include "Entity.hpp"
#include "MySystem.hpp"
#include "Component.hpp"
#include <memory>

using namespace Ragot;
using namespace std;

int main(int argc, char* argv[])
{
    System my_system;
    
    
    auto mesh_entity = make_shared < Entity > ();

    auto mesh_component = make_shared < Model_Component > ("Intergalactic_Spaceship-(Wavefront).obj", "fotos/Intergalactic Spaceship_color_4.jpg");
    mesh_entity->add_components(mesh_component, "Mesh");
    mesh_entity->transform.set_position({0.f, 0.f, -10.f});
    my_system.add_entities(mesh_entity, "Mesh");
    
    auto container_entity = make_shared < Entity > ();
    auto container_mesh_component = make_shared < Model_Component > ("Container.obj", "fotos/Container_DiffuseMap.jpg");
    container_mesh_component->set_transparency(true);
    container_entity->add_components(container_mesh_component, "Mesh");
    my_system.add_entities(container_entity, "Container");
    
    container_entity->transform.set_position({0.f, -0.f, 0.f});
    container_entity->transform.set_scale({0.01f, 0.01f, 0.01f});

    my_system.run();
    
    return 0;
}
