#include "ditto.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>

int screen_width = 640;
int screen_height = 480;
const int SCREEN_BPP = 32;

const int UPDATE_PER_SEC = 60;

double fov = 45.0;
float angel = 30.0f;
float cameraAngle = 0.0f;

void ditto_render();
int ditto_update(int physics);

typedef int (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

void setVSync(int interval)
{
  const char *extensions = glGetString( GL_EXTENSIONS );

  if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
    return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
  else
  {
#ifdef wglGetProcAddress
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
#else
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)glXGetProcAddress("glXSwapIntervalEXT");
    
#endif
    if( wglSwapIntervalEXT )
      wglSwapIntervalEXT(interval);
  }
}

int init_GL()
{
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, (double)screen_width/(double)screen_height, 1.0, 200.0);
  //glOrtho(0, screen_width, screen_height, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  if (glGetError() != GL_NO_ERROR)
    return -1;
  
  setVSync(0);
  
  return 0;
}

int ditto_init(char * text, char * icon)
{  
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    return -1;
  if (SDL_SetVideoMode(screen_width, screen_height, SCREEN_BPP, SDL_OPENGL | SDL_RESIZABLE) == NULL)
    return -1;
  if (init_GL() != 0)
    return -1;
  
  SDL_WM_SetCaption(text, icon);
  
  return 0;
}

void ditto_loop()
{
  int quit = 0;
  Uint32 t;
  while (!quit) {
    t = SDL_GetTicks();
    
    if (ditto_update(1))
      quit = 1;
    
    while (1000 / UPDATE_PER_SEC > (SDL_GetTicks() - t)) {
      ditto_render();
      SDL_Delay(1);
      if (1000 / UPDATE_PER_SEC > (SDL_GetTicks() - t))
	if (ditto_update(0))
	  quit = 1;
    }
  }
}

void ditto_render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(-cameraAngle, 0.0f, 1.0f, 0.0f);
  glTranslatef(0.0f, 0.0f, -5.0f);
  
  glPushMatrix();
  glTranslatef(0.0f, -1.0f, 0.0f);
  glRotatef(angel, 0.0f, 0.0f, 1.0f);
  glBegin(GL_QUADS);
    glVertex3f(-0.7f, -0.5f, 0.0f);
    glVertex3f(0.7f, -0.5f, 0.0f);
    glVertex3f(0.4f, 0.5f, 0.0f);
    glVertex3f(-0.4f, 0.5f, 0.0f);
  glEnd();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(1.0f, 1.0f, 0.0f);
  glRotatef(angel, 0.0f, 1.0f, 0.0f);
  glScalef(0.7f, 0.7f, 0.7f);
  glBegin(GL_TRIANGLES);
    //Pentagon
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    
  glEnd();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(-1.0f, 1.0f, 0.0f);
  glRotatef(angel, 1.0f, 2.0f, 3.0f);
  glBegin(GL_TRIANGLES);  
    //Triangle
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
  glEnd();
  glPopMatrix();
  
  SDL_GL_SwapBuffers();
}

int ditto_update(int physics)
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
	return 1;
      else if (event.type == SDL_KEYDOWN) {
	switch (event.key.keysym.sym) {
	  default: break;
	}
      } else if (event.type == SDL_KEYUP) {
	switch (event.key.keysym.sym) {
	  default: break;
	  case SDLK_ESCAPE: return 1;
	}
      } else if (event.type == SDL_VIDEORESIZE) {
	screen_width = event.resize.w;
	screen_height = event.resize.h;
	
	
	SDL_SetVideoMode(screen_width, screen_height, SCREEN_BPP, SDL_OPENGL | SDL_RESIZABLE);
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (double)screen_width/(double)screen_height, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
      }
    }
  
  if (physics) {
    angel += 2.0f;
    if (angel > 360)
      angel -=360;
  }
  return 0;
}

void ditto_destoy()
{
  SDL_Quit();
}