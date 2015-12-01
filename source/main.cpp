/*
 By: Justin Meiners
 
 Copyright (c) 2015 Justin Meiners
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


#include <iostream>
#include "SDL2/SDL.h"

#include "chem.hpp"
#include "renderer.hpp"


int main(int argc, const char * argv[])
{
    Chemistry<> chem;
    Renderer gl;
    MoleculeParser<> parser(chem);

    /* read from either stdin or file from argument */
    Molecule molecule = (argc == 2) ? parser.ParseMolecule(argv[1]) : parser.ParseMolecule(std::cin);
    
    
    View<> view(-20, -20, 10, 0, 0, 0);
    
    
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("failed to init SDL\n");
        return 1;
    }
    
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_RendererInfo rendererInfo;
    int width = 800;
    int height = 600;
    
    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer);

    if (!window || !renderer)
    {
        printf("failed to create window and renderer\n");
        return 1;
    }
    
    SDL_GetRendererInfo(renderer, &rendererInfo);
    if ((rendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0)
    {
        SDL_Quit();
        return 1;
    }
    
    SDL_SetWindowTitle(window, "Molecule Viewer");
    SDL_GL_SetSwapInterval(1);
    gl.Reshape(width, height);

    bool dragging = false;
    int lastX = 0;
    int lastY = 0;
    
    while(1)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                {
                    return 0;
                }
                case SDL_MOUSEMOTION:
                {
                    if (dragging)
                    {
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        
                        int deltaX = x - lastX;
                        int deltaY = y - lastY;
                        
                        view.xAngle += deltaX;
                        view.yAngle += deltaY;
                        
                        lastX = x;
                        lastY = y;
                    }
                    
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    
                    lastX = x;
                    lastY = y;
                    
                    dragging = true;
                    break;
                }
                case SDL_MOUSEBUTTONUP:
                {
                    dragging = false;
                    break;
                }
                    
                case SDL_KEYUP:
                {
                    if (e.key.keysym.sym == SDLK_h)
                    {
                        view.showBonds = !view.showBonds;
                    }
                }
            }
        }
        

        gl.Render(molecule, chem, view);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(17);
    }
    
    return 0;
}

