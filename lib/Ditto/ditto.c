#include "ditto.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int SQUARE_WIDTH = 20;
const int SQUARE_HEIGHT = 20;

const int UPDATE_PER_SEC = 60;

int x, y;
int xVel, yVel;

void ditto_render();
void ditto_update();

int init_GL()
{
  glClearColor(0, 0, 0, 0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
  
  glMatrixMode(GL_MODELVIEW);
  
  if (glGetError() != GL_NO_ERROR)
    return -1;
  return 0;
}

int ditto_init()
{
  x = 0;
  y = 0;
  xVel = 0;
  yVel = 0;
  
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    return -1;
  if (SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_OPENGL) == NULL)
    return -1;
  if (init_GL() != 0)
    return -1;
  return 0;
}

void ditto_setTitle(char* text)
{
  SDL_WM_SetCaption(text, NULL);
}


void ditto_loop()
{
  SDL_Event event;
  int quit = 0;
  Uint32 t;
  while (!quit) {
    t = SDL_GetTicks();
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
	quit = 1;
      else if (event.type == SDL_KEYDOWN) {
	switch (event.key.keysym.sym) {
	  case SDLK_UP: yVel -= SQUARE_HEIGHT / 2; break;
	  case SDLK_DOWN: yVel += SQUARE_HEIGHT / 2; break;
	  case SDLK_LEFT: xVel -= SQUARE_WIDTH / 2; break;
	  case SDLK_RIGHT: xVel += SQUARE_WIDTH / 2; break;
	}
      } else if (event.type == SDL_KEYUP) {
	switch (event.key.keysym.sym) {
	  case SDLK_UP: yVel += SQUARE_HEIGHT / 2; break;
	  case SDLK_DOWN: yVel -= SQUARE_HEIGHT / 2; break;
	  case SDLK_LEFT: xVel += SQUARE_WIDTH / 2; break;
	  case SDLK_RIGHT: xVel -= SQUARE_WIDTH / 2; break;
	  case SDLK_ESCAPE: quit = 1;
	}
      }
    }
    
    ditto_update();
    
    while (1000 / UPDATE_PER_SEC > (SDL_GetTicks() - t)) {
      ditto_render();
    }
  }
}

void ditto_render()
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  glTranslatef(x, y, 0);
  glBegin(GL_QUADS);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(SQUARE_WIDTH, 0, 0);
    glVertex3f(SQUARE_WIDTH, SQUARE_HEIGHT, 0);
    glVertex3f(0, SQUARE_HEIGHT, 0);
  glEnd();
  glLoadIdentity();
  
  SDL_GL_SwapBuffers();
}

void ditto_update()
{
  x += xVel;
  
  if ((x < 0) || (x + SQUARE_WIDTH > SCREEN_WIDTH))
    x -= xVel;
  
  y += yVel;
  if ((y < 0) || (y + SQUARE_HEIGHT > SCREEN_HEIGHT))
    y -= yVel;
}

void ditto_destoy()
{
  SDL_Quit();
}