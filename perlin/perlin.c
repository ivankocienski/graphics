
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>

typedef unsigned char BYTE;

SDL_Surface *screen;

BYTE *texture = NULL;
BYTE *perlin_texture = NULL;


void draw() {

  BYTE *screen_pixels;
  BYTE *texture_pixels;
  int x;
  int y;

  SDL_LockSurface( screen );

  screen_pixels  = (BYTE *)screen->pixels;
  texture_pixels = perlin_texture;

  for(y = 0; y < 256; y++) {

    memcpy( screen_pixels, texture_pixels, 256 );

    screen_pixels  += 640; 
    texture_pixels += 256;
  }

  SDL_UnlockSurface( screen );

}

void cleanup() {

  if(texture) free(texture);
  SDL_Quit();

}

BYTE read_tex(int x, int y) {

  x &= 255;
  y &= 255;

  return *(texture + x + y * 256);
}

void init_texture() {

  unsigned char *pixels;
  int x;
  int y;
  int rx;
  int ry;
  int scale;
  int p11;
  int p21;
  int p12;
  int p22;
  float fx;
  float fy;

  float i;
  float j;

  BYTE *p;
  int r;
  float o;

  /* RAM */
  texture = (BYTE*)calloc( 256 * 256, sizeof(BYTE));
  if(!texture) {
    perror("allocating texture space");
    exit(0);
  }

  perlin_texture = (BYTE*)calloc(256 * 256, sizeof(BYTE));
  if(!perlin_texture) {
    perror("allocating texture space 2");
    exit(0);
  }



  /* now perlin texture */

  for( scale = 128; scale > 0; scale >>= 1 ) {\

  //if(1) {

    //scale = 32;

    /* source texture */
    p = texture;
    for(y = 0; y < 256 * 256; y++) {
      *(p++) = rand() % 20;
    }

    pixels = perlin_texture;

    for(y = 0; y < 256; y++) {

      for(x = 0; x < 256; x++) {

        rx = (x + scale / 2) / scale;
        ry = (y + scale / 2) / scale;

        p11 = read_tex( rx,     ry     );
        p21 = read_tex( rx + 1, ry     );
        p12 = read_tex( rx,     ry + 1 );
        p22 = read_tex( rx + 1, ry + 1 );

        fx = (float)((x + scale / 2) % scale) / (float)scale;
        fy = (float)((y + scale / 2) % scale) / (float)scale;

        i = p11 * (1 - fx) + p21 * fx;
        j = p12 * (1 - fx) + p22 * fx;

        o = i * (1 - fy) + j * fy;

        rx = *pixels;
        rx += o;
        if(rx > 255) rx = 255;

        *pixels = rx;

        pixels++;
      }
    }
  }

}

void init() {

  SDL_Color *palette;

  int r;

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

  palette = (SDL_Color *)calloc( 256, sizeof(SDL_Color) );
  if(!palette) {
    perror("allocating palette space");
    exit(0);
  }

  for(r = 0; r < 256; r++) {
    palette[r].r = r;
    palette[r].g = r;
    palette[r].b = r;
  }

  SDL_SetColors( screen, palette, 0, 256 );

  free(palette); 
}

int main(int argc, char ** argv ) {

  SDL_Event ev;

  atexit(cleanup);

  init();
  init_texture();

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

    draw();

    SDL_Flip(screen);
    SDL_Delay(20);
  }


  return 0;
}


