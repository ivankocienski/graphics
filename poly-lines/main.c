
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>

#define NUM_LINES 20

typedef unsigned char byte;
typedef byte* pbyte;

typedef struct _S_POINT {
  float x;
  float y;
  float x_inc;
  float y_inc;
} T_POINT, *PT_POINT;

typedef struct _S_LINE {
  T_POINT p1;
  T_POINT p2;
} T_LINE, *PT_LINE;

PT_LINE lines = NULL;
SDL_Surface *screen = NULL;

inline double frand() {
  return (double)rand() / (double)RAND_MAX;
}

void randomize_inc( PT_POINT p ) {

  p->x_inc = 1 - 2.0 * frand();
  p->y_inc = 1 - 2.0 * frand();
}

void randomize_pos( PT_POINT p ) {
  
  p->x = frand() * 640;
  p->y = frand() * 480; 
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

void draw_line( pbyte p, byte c, PT_LINE l, int minx, int miny, int maxx, int maxy ) {

  int x1 = l->p1.x;
  int x2 = l->p2.x;
  int y1 = l->p1.y;
  int y2 = l->p2.y;
  int dx, dy;
  int sx, sy;
  int err, err2;

  float r;

  if( x1 == x2 ) {
    if( x1 < minx || x1 > maxx ) return;

    if( y2 < y1 ) {
      dy = y2;
      y2 = y1;
      y1 = dy;
    }

    if( y1 < miny) {
      if(y2 < miny) return;
      y1 = miny;
    }
    if(y2 > maxy) {
      if(y1 > maxy) return;
      y2 = maxy;
    }

    p += (y1 * 640) + x1;
    dy = y2 - y1;
    while(dy) {
      dy--;
      *p = c;
    }

    return; 
  }

  if( y1 == y2 ) {
    if( y1 < miny || y1 > maxy ) return;

    if( x2 < x1 ) {
      dx = x2;
      x2 = x1;
      x1 = dx;
    }

    if( x1 < minx ) {
      if( x2 < minx ) return;
      x1 = minx;
    }

    if( x2 > maxx) {
      if( x1 > maxx ) return;
      x2 = maxx;
    }

    /* should use memset */
    p += (y1 * 640) + x1;

    memset( p, x2 - x1, c );

    return; 
  }

  //TODO... edge boundary clipping...

  dx = abs(x2 - x1);
  dy = abs(y2 - y1); 

  r = (float)dx / (float)dy;

  printf( "preclip\n");
  printf( "  x1=%d\n", x1 );
  printf( "  y1=%d\n", y1 );
  printf( "  x2=%d\n", x2 );
  printf( "  y2=%d\n", y2 );
  printf( "   r=%f\n", r );
  

  if( x1 < minx ) {
    dx  = minx - x1;
    y1 += r * dx;
    x1  = minx;
  }

  if( x2 > maxx ) {
    dx  = x2 - maxx;
    y2 -= r * dx;
    x2  = maxx;
  }

/*   if( y1 < miny ) {
 *     dy  = miny - y1;
 *     x1 += r * dy;
 *     y1  = miny;
 *   }
 * 
 *   if( y2 > maxy ) {
 *     dy  = y2 - maxy;
 *     x2 -= r * dy;
 *     y2  = maxy;
 *   }
 */

  printf( "postclip\n" );
  printf( "  x1=%d\n", x1 );
  printf( "  y1=%d\n", y1 );
  printf( "  x2=%d\n", x2 );
  printf( "  y2=%d\n", y2 );


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

void move_point( PT_POINT p ) {

  p->x += p->x_inc;
  p->y += p->y_inc;

  if(p->x < 1 || p->x > 639 ) {
    p -> x_inc = -(p -> x_inc);
    p -> y_inc = frand();
  }

  if( p->y < 0 || p->y > 479 ) { 
    p -> y_inc = -(p -> y_inc);
    p -> x_inc = frand();
  }

}

void move_stuff() { 

  int i;

  for(i = 0; i < NUM_LINES; i++) {

    move_point( &lines[i].p1 );
    move_point( &lines[i].p2 ); 
  }
}

void do_stuff() {

  int i;
  PT_LINE l;
  pbyte pixels;

  SDL_LockSurface( screen );
  pixels = (pbyte)screen->pixels; 

  for(i = 0; i < NUM_LINES; i++) {

    l = &lines[i];

    draw_line( pixels, 100, l,   0,   0, 639, 479 );
    draw_line( pixels, 255, l, 160, 120, 479, 359 );
  }

  SDL_UnlockSurface( screen );
}

void init_stuff() {

  int i;

  lines = (PT_LINE)calloc( NUM_LINES, sizeof( T_LINE ));
  if(!lines) {
    perror("could not allocate lines");
    exit(-1);
  }

  for(i = 0; i < NUM_LINES; i++) {

    randomize_pos( &lines[i].p1 );
    randomize_inc( &lines[i].p1 );

    randomize_pos( &lines[i].p2 );
    randomize_inc( &lines[i].p2 );
  } 
}

void cleanup() {

  SDL_Quit();

  if(lines) free(lines);
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

    move_stuff();

    do_stuff();

    SDL_Flip( screen );

    SDL_Delay( 20 );
  }

  return 0;
}

