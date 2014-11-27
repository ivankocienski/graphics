
#include <vector>

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>

#include "common.hh"
#include "rasterizer.hh"
#include "triangle.hh"

const int g_xres = 800; 
const int g_yres = 600;

static SDL_Surface *screen;

static SDL_Color palette[] = {
  {   0,   0,   0 },
  { 255,   0,   0 },
  { 255, 255,   0 },
  {   0, 255,   0 },
  {   0,   0, 255 } 
};

static const int num_triangles = 50;
typedef vector<Triangle> triangle_vector_t; 
static triangle_vector_t triangles;

int mouse_x = g_xres / 2;
int mouse_y = g_yres / 2;

void draw() {

  triangle_vector_t::iterator it;

  SDL_LockSurface( screen );

  rast_setup( (BYTE *)screen->pixels, screen->pitch, screen->w, screen->h ); 
  
  for( it = triangles.begin(); it != triangles.end(); it++ ) {
    //it->set_pos( mouse_x, mouse_y );
    it->move();
    it->draw();
  } 

  SDL_UnlockSurface( screen ); 
}

void do_mouse_wheel( int d ) {
  //triangles[0].adjust_angle( d );
}

void cleanup() {

  SDL_Quit();

}

void init() {

  srand(time(NULL));

  if( SDL_Init( SDL_INIT_VIDEO ) == -1) { 
    fprintf(stderr, "could not init SDL\n");
    exit(-1); 
  }

  screen = SDL_SetVideoMode( g_xres, g_yres, 8, SDL_SWSURFACE | SDL_DOUBLEBUF );
  if(!screen) {
    fprintf(stderr, "could not set video mode\n");
    exit(-1);
  } 

  SDL_SetColors( screen, palette, 0, 5 ); 
  SDL_SetPalette( screen, SDL_LOGPAL | SDL_PHYSPAL, palette, 0, 5 ); 

  triangles.resize( num_triangles );

  triangle_vector_t::iterator it;

  for( it = triangles.begin(); it != triangles.end(); it++ )
    it->randomize();
}

int main(int argc, char ** argv ) {

  char buffer[200];

  float previous;

  SDL_Event ev;

  atexit(cleanup);

  init();

  previous = SDL_GetTicks();

  while(1) {

    float now = SDL_GetTicks();
    float fps = (1.0 / (now - previous)) * 1000;
    previous = now;

    snprintf( buffer, 200, "filled triangle demo (%.2f fps)", fps );
    SDL_WM_SetCaption( buffer, NULL );
    
    /* Check for events */
    while(SDL_PollEvent(&ev)) {  

      switch(ev.type) { 
        case SDL_QUIT:
          exit(0);
          break;

        case SDL_KEYUP:
          if(ev.key.keysym.sym == SDLK_F12) 
            return 0;
          break;

        case SDL_MOUSEMOTION:
          mouse_x = ev.motion.x;
          mouse_y = ev.motion.y;
          break;

        case SDL_MOUSEBUTTONDOWN:
          if( ev.button.button == 5 ) { do_mouse_wheel(-1); }
          if( ev.button.button == 4 ) { do_mouse_wheel( 1); }
          break;
      }
    }

    SDL_FillRect( screen, NULL, 0 );

    draw();

    SDL_Flip(screen);
    SDL_Delay(20);
  }


  return 0;
}


