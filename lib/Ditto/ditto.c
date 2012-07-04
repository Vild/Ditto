#include "ditto.h"
#include "textureLoader.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>

int screen_width = 640;
int screen_height = 480;
const int SCREEN_BPP = 32;

const int UPDATE_PER_SEC = 60;

double fov = 45.0;
const float BOX_SIZE = 7.0f;
float angle = 0;
GLuint texture;

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
#ifdef _WIN32 || _WIN64
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
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_ALPHA_TEST);
  
  glAlphaFunc(GL_GREATER, 0.5);
  //glShadeModel(GL_SMOOTH);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, (double)screen_width/(double)screen_height, 1.0, 200.0);
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
  
  texture = loadTexture("tex.png", 0);

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
  
  glTranslatef(0.0f, 0.0f, -20.0f);
  
  GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
  
  GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
  GLfloat lightPos[] = {-2 * BOX_SIZE, BOX_SIZE, 4 * BOX_SIZE, 1.0f};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  
  glRotatef(-angle, 1.0f, 1.0f, 1.0f);
  
  glBegin(GL_QUADS);
    //Top face
    glColor3f(1.0f, 1.0f, 0.0f);
    glNormal3f(0.0, 1.0f, 0.0f);
    glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
    glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
    glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
    glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
    
    //Bottom face
    glColor3f(1.0f, 0.0f, 1.0f);
    glNormal3f(0.0, -1.0f, 0.0f);
    glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
    glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
    glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
    glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
    
    //Left face
    glNormal3f(-1.0, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
    glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
    glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
    
    //Right face
    glNormal3f(1.0, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
    glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
    glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
  glEnd();
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
    //Front face
    glNormal3f(0.0, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
    
    //Back face
    glNormal3f(0.0, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  
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
	  case SDLK_f: (fov == 45.0f) ? fov = 80.0f : fov == 45.0f; break;
	  case SDLK_ESCAPE: return 1;
	  default: break;
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
    angle += 1.5f;
    if (angle > 360)
      angle -= 360;
  }
  return 0;
}

void ditto_destoy()
{
  glDeleteTextures(1, &texture);
  SDL_Quit();
}
