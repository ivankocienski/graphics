
#include <stdio.h>
#include <GL/gl.h>
#include <math.h>

enum {
  C_LEFT  = 8, // 1000
  C_RIGHT = 4, // 0100
  C_UP    = 2, // 0010
  C_DOWN  = 1  // 0001
};

static int min_x;
static int min_y;
static int max_x;
static int max_y;

void setup_line_clipper( int minx, int miny, int maxx, int maxy ) {
  min_x = minx;
  min_y = miny;
  max_x = maxx;
  max_y = maxy;
}

//#define dist( a, b, c, d ) sqrt( (c-a * c-a) + (d-b * d-b) )

float dist( float x1, float y1, float x2, float y2 ) {
  float dx = x2 - x1;
  float dy = y2 - y1;

  return sqrt( dx * dx + dy * dy ); 
}

int clip_line( int x1, int y1, int x2, int y2, int *ox1, int *oy1, int *ox2, int *oy2 ) {

  static unsigned char blink = 0;

  char clip_a = 0;
  char clip_b = 0;

  if( x1 < min_x ) clip_a |= C_LEFT;
  if( x2 < min_x ) clip_b |= C_LEFT;

  if( y1 < min_y ) clip_a |= C_UP;
  if( y2 < min_y ) clip_b |= C_UP;

  if( x1 >= max_x ) clip_a |= C_RIGHT;
  if( x2 >= max_x ) clip_b |= C_RIGHT;

  if( y1 >= max_y ) clip_a |= C_DOWN;
  if( y2 >= max_y ) clip_b |= C_DOWN;

  if( (clip_a & clip_b) & C_LEFT  ) return 0;
  if( (clip_a & clip_b) & C_DOWN  ) return 0;
  if( (clip_a & clip_b) & C_UP    ) return 0;
  if( (clip_a & clip_b) & C_RIGHT ) return 0;

  float dx = (float)(x2 - x1);
  float dy = (float)(y2 - y1);
  float s1 = dy / dx; 
  float s2 = dx / dy; 
  int nx1, ny1;
  int nx2, ny2;
  int d1;
  int d2;
  
  blink++;

  switch( clip_a ) {
    case C_LEFT | C_UP: 
      
      //up
      d1 = min_y - y1;
      nx1 = x1 + s2 * d1;
      ny1 = min_y;

      //left
      d2 = min_x - x1;
      nx2 = min_x;
      ny2 = y1 + s1 * d2;

      
      if( dist(nx1, ny1, x2, y2) < dist(nx2, ny2, x2, y2) ) {
        x1 = nx1;
        y1 = ny1;
      } else {
        x1 = nx2;
        y1 = ny2;
      }
      break; 

    case C_LEFT: // okay
      d1 = min_x - x1;
      y1 += s1 * d1;
      x1 = min_x;
      break;

    case C_LEFT | C_DOWN:

      // down
      d1 = max_y - y1;
      nx1 = x1 + s2 * d1;
      ny1 = max_y - 1;

      // left
      d2 = min_x - x1;
      nx2 = min_x;
      ny2 = y1 + s1 * d2;

      glLineWidth(5); 
      glBegin(GL_LINES);

      if( ((blink >> 2) & 63) < 31 ) {

        glColor3f( 1, 0.3, 0.3 );
        glVertex2f(nx2, ny2);
        glVertex2f(x2, y2);

      } else {

        glColor3f( 0.3, 0.3, 1 );
        glVertex2f(nx1, ny1);
        glVertex2f(x2, y2);
      }

      glEnd(); 
      glLineWidth(1);


      dx = nx1 - x2;
      dy = ny1 - y2;


      if( dist(nx1, ny1, x2, y2) < dist(nx2, ny2, x2, y2) ) {
        x1 = nx1;
        y1 = ny1; 

      } else {
        x1 = nx2;
        y1 = ny2;
      }


      break;

    case C_UP: // okay
      d1 = min_y - y1;
      x1 += s2 * d1;
      y1 = min_y - 1; 
      break;

    case C_DOWN: // okay
      d1 = max_y - y1;
      x1 += s2 * d1;
      y1 = max_y - 1;
      break;

    case C_RIGHT | C_UP:

      // up
      d1  = min_y - y1;
      nx1 = x1 + s2 * d1;
      ny1 = min_y; 

      // right
      d2  = x1 - max_x;
      nx2 = max_x - 1;
      ny2 = y1 - s1 * d2;




      if( dist(nx1, ny1, x2, y2) < dist(nx2, ny2, x2, y2) ) {
        x1 = nx1;
        y1 = ny1;


      } else {

        x1 = nx2;
        y1 = ny2;

      }


      break;

    case C_RIGHT:
      d1  = x1 - max_x;
      y1 -= s1 * d1;
      x1  = max_x - 1;
      break;

    case C_RIGHT | C_DOWN:

      //right
      d1  = x1 - max_x;
      nx2 = max_x - 1;
      ny2 = y1 - s1 * d1;

      //down
      d2 = max_y - y1;
      nx1 = x1 + s2 * d2;
      ny1 = max_y - 1;

      if( dist(nx1, ny1, x2, y2) < dist(nx2, ny2, x2, y2) ) {
        x1 = nx1;
        y1 = ny1;
      } else {
        x1 = nx2;
        y1 = ny2;
      }
      break;
  }

  // have we clipped ourselves to invisibiltiy?
 
    if( clip_b & C_UP ) {
      if( y1 < min_y ) return 0;
    } else
      if( clip_b & C_DOWN ) {
	if( y1 >= max_y ) return 0;
      }

    if( clip_b & C_LEFT ) {
      if( x1 < min_x ) return 0;
    } else
      if( clip_b & C_RIGHT ) {
	if( x1 >= max_x ) return 0;
      }
    

  switch( clip_b ) {
    case C_LEFT | C_UP: //okay
      
      //up
      d1 = min_y - y2;
      nx1 = x2 + s2 * d1;
      ny1 = min_y;

      //left
      d2 = min_x - x2;
      nx2 = min_x;
      ny2 = y2 + s1 * d2;

      if( dist(nx1, ny1, x1, y1) < dist(nx2, ny2, x1, y1) ) {
        x2 = nx1;
        y2 = ny1;
      } else {
        x2 = nx2;
        y2 = ny2;
      }
      break; 

    case C_LEFT: // okay
      d1 = min_x - x2;
      y2 += s1 * d1;
      x2 = min_x;
      break;

    case C_LEFT | C_DOWN: // okay

      // dpwn
      d1 = max_y - y2;
      nx1 = x2 + s2 * d1;
      ny1 = max_y - 1;

      // left
      d2 = min_x - x2;
      nx2 = min_x;
      ny2 = y2 + s1 * d2;

      if( dist(nx1, ny1, x1, y1) < dist(nx2, ny2, x1, y1) ) {
        x2 = nx1;
        y2 = ny1;
      } else {
        x2 = nx2;
        y2 = ny2;
      }

      break;

    case C_UP: // okay
      d1 = min_y - y2;
      x2 += s2 * d1;
      y2 = min_y - 1; 
      break;

    case C_DOWN: // okay
      d1 = max_y - y2;
      x2 += s2 * d1;
      y2 = max_y - 1;
      break;

    case C_RIGHT | C_UP:

      // up
      d1  = min_y - y2;
      nx1 = x2 + s2 * d1;
      ny1 = min_y; 

      // right
      d2  = x2 - max_x;
      nx2 = max_x - 1;
      ny2 = y2 - s1 * d2;

      if( dist(nx1, ny1, x1, y1) < dist(nx2, ny2, x1, y1) ) {
        x2 = nx1;
        y2 = ny1;
      } else {
        x2 = nx2;
        y2 = ny2;
      }

      break;

    case C_RIGHT:
      d1  = x2 - max_x;
      y2 -= s1 * d1;
      x2  = max_x - 1;
      break;

    case C_RIGHT | C_DOWN:

      //right
      d1  = x2 - max_x;
      nx2 = max_x - 1;
      ny2 = y2 - s1 * d1;

      //down
      d2 = max_y - y2;
      nx1 = x2 + s2 * d2;
      ny1 = max_y - 1;

      if( dist(nx1, ny1, x1, y1) < dist(nx2, ny2, x1, y1) ) {
        x2 = nx1;
        y2 = ny1;
      } else {
        x2 = nx2;
        y2 = ny2;
      }
      break;

  }

    if( clip_a & C_UP ) {
      if( y2 < min_y ) return 0;
    } else
      if( clip_a & C_DOWN ) {
	if( y2 >= max_y ) return 0;
      }

    if( clip_a & C_LEFT ) {
      if( x2 < min_x ) return 0;
    } else
      if( clip_a & C_RIGHT ) {
	if( x2 >= max_x ) return 0;
      }
 
  *ox1 = x1;
  *oy1 = y1;
  *ox2 = x2;
  *oy2 = y2;

  return 1;
}
