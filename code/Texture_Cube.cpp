//
//  Texture_Cube.cpp
//  OpenGL-FinalProject
//
//  Created by Andrés Ragot on 5/1/25.
//

#include "Texture_Cube.hpp"

#include <SOIL2.h>
#include <vector>
#include <iostream>
#include <filesystem>

#include <cassert>

#include <sstream>

namespace Ragot
{
    using namespace std;
    
    void check_gl_error(const std::string &label) { GLenum error; while ((error = glGetError()) != GL_NO_ERROR) { std::cerr << "OpenGL error [" << label << "]: " << error << std::endl; } }
    
    Texture_Cube::Texture_Cube(const string & texture_base_path)
    {
        texture_is_loaded = false;
        
        vector < shared_ptr< Color_Buffer > > texture_sides (6);
        
        for (size_t texture_index = 0; texture_index < 6; texture_index++)
        {
        
            texture_sides[texture_index] = load_image (texture_base_path + char('0' + texture_index) + ".png");
            
            if (!texture_sides [texture_index])
            {
                return;
            }
        }
        
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        
        glEnable        (GL_TEXTURE_CUBE_MAP);
        
        check_gl_error("gl enable");
        
        glGenTextures   (1, &texture_id);
        check_gl_error("gl gen textures");
        
        glActiveTexture (GL_TEXTURE0);
        check_gl_error("gl activate texture");
        glBindTexture   (GL_TEXTURE_CUBE_MAP, texture_id);
        check_gl_error("gl bind texture");
        
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        check_gl_error("gl tex parameteri 1");
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        check_gl_error("gl tex parameteri 2");
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        check_gl_error("gl tex parameteri 3");
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        check_gl_error("gl tex parameteri 4");
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_EDGE);
        check_gl_error("gl tex parameteri 5");
        
        static const GLenum texture_target[] =
        {
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
        };
        
        for (size_t texture_index = 0; texture_index < 6; ++texture_index)
        {
            
            Color_Buffer & texture = *texture_sides[texture_index];
            
            glTexImage2D
            (
                texture_target [texture_index],
                0,
                GL_RGBA,
                texture.get_width  (),
                texture.get_height (),
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                texture.colors ()
            );
            
            ostringstream labelStream;
            labelStream << "gl tex image 2d" << " " << texture_index;
            
            check_gl_error(labelStream.str());
        }
        
        texture_is_loaded = true;
        
    }
    
    shared_ptr< Texture_Cube::Color_Buffer > Texture_Cube::load_image(const std::string &image_path)
    {
    
        // std::cout << "Directorio actual: " << std::filesystem::current_path() << std::endl;
        
        // std::cout << "Ejecutando load_image para: " << image_path << std::endl;
        
        int image_width    = 0;
        int image_height   = 0;
        int image_channels = 0;
        
        uint8_t * loaded_pixels = SOIL_load_image
        (
            image_path.c_str(),
           &image_width,
           &image_height,
           &image_channels,
            SOIL_LOAD_RGBA
        );
        
        if (loaded_pixels)
        {
            auto image = make_shared< Color_Buffer > (image_width, image_height);
            
            copy_n
            (
                loaded_pixels,
                size_t (image_width) * size_t (image_height) * sizeof (Color_Buffer::Color),
                reinterpret_cast< uint8_t * >(image->colors())
            );
            
            SOIL_free_image_data (loaded_pixels);
            
            return image;
        }
        
        return nullptr;
    }

    Texture_Cube::~Texture_Cube()
    {
        if (texture_is_loaded)
        {
            glDeleteTextures (1, &texture_id);
        }
    }
}

