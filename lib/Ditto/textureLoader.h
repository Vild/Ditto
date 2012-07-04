#ifndef IMAGELOADER_H
#define IMAGELOADER_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

GLuint loadTexture(const char * file, int alpha);

#endif