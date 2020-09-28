// myImageLoader.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <assert.h>
#include <SDL.h>
#include <iostream>

#include "ImageException.h"
#include "IMetaImage.h"
#include "ImageFactory.h"
#include "Logger.h"

using namespace std;

void SDLDrawImage(IMetaImage *image) {
    bool quit = false;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        image->GetWidth(), image->GetHeight(),
        SDL_WINDOW_OPENGL);

    SDL_Surface *screen = SDL_GetWindowSurface(window);

    SDL_Surface *imageSurface = SDL_CreateRGBSurfaceFrom(
        image->GetPixelPtr(),
        image->GetWidth(),
        image->GetHeight(),
        image->GetPitch(),
        image->GetWidth() * (image->GetPitch() / 8),
        image->GetRedMask(),
        image->GetGreenMask(),
        image->GetBlueMask(),
        image->GetAlphaMask());

    //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0, 0));
    SDL_BlitSurface(imageSurface, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);

    while (!quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        }
    }

    image->Destroy();
    SDL_FreeSurface(screen);
    SDL_FreeSurface(imageSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    try {
        //LOG_INFO("Loading image");
        Logger::GetInstance() << LoggerLevel::INFO << __FUNCDNAME__ <<  "Loading image";

        IMetaImage *image = ImageFactory::Get()->Detector("d:\\image_test\\test.bmp");
        image->Load();
        SDLDrawImage(image);
    }
    catch (ImageException &e) {
        cerr << endl << "Image loader SDK exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
