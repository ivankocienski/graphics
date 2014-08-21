
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>

typedef unsigned char BYTE;

SDL_Surface *screen;

void do_frame() { 

}

void render_pattern() { 

  BYTE *p;
  int x;
  int y;
  int hx;
  int hy;
  float d;
  float ang;

  /* draw pattern into screen buffer */
  SDL_LockSurface( screen );
  p = screen->pixels;
  
  for(y = 0; y < 480; y++)
    for(x = 0; x < 640; x++) {

      hx = x - 320;
      hy = y - 240;

      d = sqrt(hx*hx + hy*hy);
      if( d > 100 && d < 200) {

        ang = 0.25 + 0.25 * (hy / d);
        if(hx < 0) ang = 1.0 - ang; 

        *p = ang * 255;
      }

      p++;
    }
  
  SDL_UnlockSurface( screen );
}

void cleanup() {

  SDL_Quit();
}

void init() {

  SDL_Color palette[256];

  int i; 

  srand(time(NULL));

  if( SDL_Init( SDL_INIT_VIDEO ) == -1) {

    fprintf(stderr, "could not init SDL\n");
    exit(-1); 
  }

  screen = SDL_SetVideoMode( 640, 480, 8, SDL_SWSURFACE | SDL_DOUBLEBUF );
  if(!screen) {
    fprintf(stderr, "could not set video mode\n");
    exit(-1);
  }

  SDL_WM_SetCaption( "teapot", NULL );

  for(i = 0; i < 256; i++) {
    palette[i].r = i;
    palette[i].g = i;
    palette[i].b = i;
  }

  SDL_SetColors( screen, palette, 1, 255 ); 
}

int main(int argc, char ** argv ) {

  SDL_Event ev;

  atexit(cleanup);

  init();

  render_pattern();

  while(1) {

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
      }
    }

    do_frame();

    SDL_Flip(screen); // shouldn't really need to do this
    SDL_Delay(20);
  }


  return 0;
}


