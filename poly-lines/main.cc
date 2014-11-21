
#include <stdio.h>
#include <SDL/SDL.h>

#include "line-loop.hh"

#include "util.hh"
#include "lines.hh"

#define XRES 800
#define YRES 600

#define POLY_POINT_COUNT 5
#define LINE_HISTORY 10

LineLoop leader( POLY_POINT_COUNT, XRES, YRES );


SDL_Surface *screen = NULL;

void do_stuff() {

  leader.move();

  pbyte pixels;

  SDL_LockSurface( screen );
  pixels = (pbyte)screen->pixels; 

  setup_line_drawer( pixels, screen->pitch, 0, 0, XRES, YRES );

  leader.draw();

  SDL_UnlockSurface( screen );
}

void init_stuff() {
}

void cleanup() {

  SDL_Quit();
}

void init() {

  SDL_Color *palette;
  int i;

  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf("failed to init SDL\n");
    exit(-1);
  }

  atexit( cleanup );

  screen = SDL_SetVideoMode( XRES, YRES, 8, SDL_SWSURFACE | SDL_DOUBLEBUF );
  if(!screen) {
    printf("failed to set crappy video mode\n");
    exit(-1);
  }

  palette = (SDL_Color*)calloc(256, sizeof(SDL_Color));
  if(!palette) {
    printf("failed to alloc palette space\n");
    exit(-1);
  }
  for(i = 0; i < 256; i++) {
    palette[i].r = i;
    palette[i].g = i;
    palette[i].b = i;
  }

  SDL_SetPalette( screen, SDL_LOGPAL | SDL_PHYSPAL, palette, 0, 256 );

  free(palette);
}

int main( int argc, char** argv ) {

  SDL_Event event;

  init();
  init_stuff();

  while(1) {

    SDL_PollEvent( &event );
    if( event.type == SDL_QUIT ) {
      printf( "quit\n");
      return 0;
    }

    SDL_FillRect( screen, NULL, 0 );

    do_stuff();

    SDL_Flip( screen ); 
    SDL_Delay( 20 );
  }

  return 0;
}

