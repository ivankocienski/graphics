
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>

#define NUM_POINTS 5

typedef unsigned char byte;
typedef byte* pbyte;

typedef struct _S_POLY_POINT {
  float x;
  float y;
  float x_inc;
  float y_inc;

  struct _S_POLY_POINT *next;
} T_POLY_POINT, *PT_POLY_POINT;


SDL_Surface *screen = NULL;

PT_POLY_POINT points;

inline double frand() {
  return (double)rand() / (double)RAND_MAX;
}

/*
 * TODO: give this a palette of 2 colors: white and grey
 *
 *   make one of those bouncing lines screen saver type algorithms
 *
 *   alter draw_line to be able to define arbitrary clip box
 *
 *   draw lines with 'no' clipping full screen in grey
 *
 *   draw same lines with box clipping to small rectangle in white
 */

void draw_line( pbyte p, byte c, int x1, int y1, int x2, int y2 ) {

  int dx, dy;
  int sx, sy;
  int err, err2;

#if 0
  if( x1 == x2 ) {
    if( x1 < 0 || x1 > 639 ) return;

    if( y2 < y1 ) {
      y  = y2;
      y2 = y1;
      y1 = y;
    }

    if( y1 < 0) {
      if(y2 < 0) return;
      y1 = 0;
    }
    if(y2 > 479) {
      if(y1 > 479) return;
      y2 = 479;
    }

    p += (y1 * 640) + x1;
    y = y2 - y1;
    while(y) {
      y--;
      *p = c;
    }

    return; 
  }

  if( y1 == y2 ) {
    if( y1 < 0 || y1 > 479 ) return;

    if( x2 < x1 ) {
      x  = x2;
      x2 = x1;
      x1 = x;
    }

    if( x1 < 0 ) {
      if( x2 < 0 ) return;
      x1 = 0;
    }

    if( x2 > 639) {
      if( x1 > 639 ) return;
      x2 = 639;
    }

    /* should use memset */
    p += (y1 * 640) + x1;

    memset( p, x2 - x1, c );

    return; 
  }
#endif

  dx = abs(x2 - x1);
  dy = abs(y2 - y1); 

  sx = (x1 < x2) ? 1 : -1;
  sy = (y1 < y2) ? 1 : -1;

  err = dx - dy;

  while(1) {

    p[ (y1 * 640) + x1 ] = c;

    if( x1 == x2 && y1 == y2) break;

    err2 = err << 1;

    if(err2 > -dy) { err -= dy; x1  += sx; }
    if(err2 <  dx) { err += dx; y1  += sy; }
  } 
}

void move_point( PT_POLY_POINT p ) {


}

void do_stuff() {

  int x;
  int y;
  
  PT_POLY_POINT first = points;
  PT_POLY_POINT p = points;

  pbyte pixels;

  SDL_LockSurface( screen );
  pixels = (pbyte)screen->pixels;

  //draw_line( pixels, 255, 320, 240, x, y );


  SDL_UnlockSurface( screen );


}

PT_POLY_POINT alloc_point() {

  PT_POLY_POINT p = (PT_POLY_POINT)calloc(1, sizeof(T_POLY_POINT));
  if(!p) {
    perror("alloc'ing points");
    exit(-1);
  }

  return p;
}

void init_stuff() {

  int i;
  PT_POLY_POINT p;

  points = p = alloc_point();

  for(i = 0; i < NUM_POINTS; i++) {
    
    p->x = rand() % 640;
    p->y = rand() % 480;
    p->x_inc = 2 - 4 * frand();
    p->y_inc = 2 - 4 * frand();

    p->next = alloc_point();
    p = p->next;
  } 
}

void cleanup() {
  PT_POLY_POINT  p = points;
  PT_POLY_POINT pp;

  SDL_Quit();

  while(p) {
    pp = p;
    p = p->next;
    free(pp); 
  }
}

void init() {

  SDL_Color *palette;
  int i;

  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf("failed to init SDL\n");
    exit(-1);
  }

  atexit( cleanup );

  screen = SDL_SetVideoMode( 640, 480, 8, SDL_SWSURFACE | SDL_DOUBLEBUF );
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

  printf("int is %d bits\n", sizeof(int) * 8 );

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

    SDL_Delay( 50 );
  }

  return 0;
}

