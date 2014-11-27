

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#include "rasterizer.hh"

// using http://forum.devmaster.net/t/advanced-rasterization/6145

static unsigned char *vbuff;
static int vpitch;
//static int vwidth;
//static int vheight;
static int vmaxx;
static int vmaxy;

void rast_setup( unsigned char *vb, int vp, int maxx, int maxy ) {
  
  vbuff   = vb;
  vpitch  = vp;
  //vwidth  = maxx;
  //vheight = maxy;
  vmaxx   = maxx - 1;
  vmaxy   = maxy - 1;
}

void rast_fill_triangle( int x1, int y1, int x2, int y2, int x3, int y3 ) {

  int x;
  int y;

  int min_x = x1;
  if( x2 < min_x ) min_x = x2;
  if( x3 < min_x ) min_x = x3;

  int min_y = y1;
  if( y2 < min_y ) min_y = y2;
  if( y3 < min_y ) min_y = y3;

  int max_x = x1;
  if( x2 > max_x ) max_x = x2;
  if( x3 > max_x ) max_x = x3;

  int max_y = y1;
  if( y2 > max_y ) max_y = y2;
  if( y3 > max_y ) max_y = y3;

  int dx12 = x1 - x2;
  int dx23 = x2 - x3;
  int dx31 = x3 - x1;

  int dy12 = y1 - y2;
  int dy23 = y2 - y3;
  int dy31 = y3 - y1;

  int c1 = dy12 * x1 - dx12 * y1;
  int c2 = dy23 * x2 - dx23 * y2;
  int c3 = dy31 * x3 - dx31 * y3;

  int cy1 = c1 + dx12 * min_y - dy12 * min_x;
  int cy2 = c2 + dx23 * min_y - dy23 * min_x;
  int cy3 = c3 + dx31 * min_y - dy31 * min_x;

  unsigned char *p = vbuff + (min_y * vpitch) + min_x;
  int pad = vpitch - (max_x - min_x);

  for( y = min_y; y < max_y; y++ ) {

    int cx1 = cy1;
    int cx2 = cy2;
    int cx3 = cy3;

    for( x = min_x; x < max_x; x++ ) {

      if( cx1 > 0 && cx2 > 0 && cx3 > 0 ) *p = 255;

      cx1 -= dy12;
      cx2 -= dy23;
      cx3 -= dy31;

      p++;
    }

    cy1 += dx12;
    cy2 += dx23;
    cy3 += dx31;

    p += pad;
  }

}


static void draw_line( int x1, int x2, int y, unsigned char c ) {

  if( x1 < 0 ) x1 = 0;
  if( x2 > vmaxx) x2 = vmaxx;

  int l = x2 - x1;
  if( l < 1 ) return;

  memset( vbuff + ( y * vpitch ) + x1, c, l );
}

static void draw_flat_tri( int xl, int yl, int xm, int ym, int xr, unsigned char c ) {

  if( xl > xr ) {
    int t = xr;
    xr = xl;
    xl = t;
  }

  float dy;
  float slope_l = 0;
  float slope_r = 0;

  float c_x;
  float c_y;
  float e_x;
  
  if( ym < yl ) {

    // scanning from point to base

    if( yl < 0 || ym > vmaxy ) return;

    dy = yl - ym;
    if( dy != 0 ) {
      slope_l = (float)(xl - xm) / dy;
      slope_r = (float)(xr - xm) / dy;
    }

    c_x = xm;
    c_y = ym;
    e_x = xm;

  } else {

    // scanning from base to point

    if( ym < 0 || yl > vmaxy ) return;

    dy = ym - yl;
    if( dy != 0 ) {
      slope_l = (float)(xm - xl) / dy;
      slope_r = (float)(xm - xr) / dy;
    }

    c_x = xl;
    c_y = yl;
    e_x = xr; 
  }

  // do we start to the left off screen?
  if( e_x < 0 ) {

    float steps_l = 0;
    if( slope_r != 0 ) steps_l = (0 - e_x) / slope_r;

    c_y += (int)steps_l;
    e_x  = 0;
    c_x += steps_l * slope_l;

  } else {

    // do we start to the right of the screen?
    if( c_x > vmaxx ) {

      float steps_r = 0;
      if( slope_l != 0 ) steps_r = (vmaxx - c_x) / slope_l;

      c_y += (int)steps_r;
      c_x  = vmaxx;
      e_x += steps_r * slope_r;
    }
  }

  // if we are above the screen then adjust the start
  if( c_y < 0 ) {

    //printf("c_y<0\n");
    float diff = 0 - c_y;

    //printf("  c_y=%f\n", c_y );
    //printf("  diff=%f\n", diff );
    //printf("  slope_r=%f\n", slope_r );
    //printf("  slope_l=%f\n", slope_l );

    c_x += slope_l * diff;
    if( c_x > vmaxx) return;

    e_x += slope_r * diff;

    c_y = 0;

    //printf("  c_x=%f e_x=%f\n", c_x, e_x );
  }

  if( ym < yl ) {

    // render it!
    while( c_y < yl && c_y < vmaxy ) {

      draw_line( c_x, e_x, c_y, c );

      c_x += slope_l;
      e_x += slope_r;

      if( e_x < 0 || c_x > vmaxx) break;

      c_y++;
    }

  } else { 

    while( c_y < vmaxy ) {

      draw_line( c_x, e_x, c_y, c );

      c_x += slope_l;
      e_x += slope_r;

      if( c_x > e_x ) break;
      if( c_x > vmaxx) break;
      if( e_x < 0) break;

      c_y++;
    }
  }

}

void rast_fill_triangle2( int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c ) {

  int s1 = abs(y1 - y2);
  int s2 = abs(y2 - y3);
  int s3 = abs(y3 - y1);

  if( s1 == 0 ) {
    draw_flat_tri( x1, y1, x3, y3, x2, c ); 
    return;
  }

  if( s2 == 0 ) {
    draw_flat_tri( x2, y2, x1, y1, x3, c ); 
    return;
  }

  if( s3 == 0 ) {
    draw_flat_tri( x1, y1, x2, y2, x3, c ); 
    return;
  }

  if( s1 > s2 && s1 > s3 ) { 
    // s1 is largest

    if( y1 < y2 ) {

      float slope = (float)(x2 - x1) / (float)(y2 - y1);

      int ppy = y3;
      int ppx = x1 + (float)(y3 - y1) * slope;

      draw_flat_tri( ppx, ppy, x1, y1, x3, c );
      draw_flat_tri( ppx, ppy, x2, y2, x3, c );
      
    } else {

      float slope = (float)(x1 - x2) / (float)(y1 - y2);

      int ppy = y3;
      int ppx = x2 + (float)(y3 - y2) * slope;

      draw_flat_tri( ppx, ppy, x1, y1, x3, c ); 
      draw_flat_tri( ppx, ppy, x2, y2, x3, c );
    }

    return;
  }

  if(s2 > s1 && s2 > s3) {
    // s2 is the largest

    if( y2 < y3 ) {

      float slope = (float)(x3 - x2) / (float)(y3 - y2);

      int ppy = y1;
      int ppx = x2 + (float)(y1 - y2) * slope;

      draw_flat_tri( ppx, ppy, x2, y2, x1, c );
      draw_flat_tri( ppx, ppy, x3, y3, x1, c );
      
    } else {

      float slope = (float)(x2 - x3) / (float)(y2 - y3);

      int ppy = y1;
      int ppx = x3 + (float)(y1 - y3) * slope;

      draw_flat_tri( ppx, ppy, x2, y2, x1, c );
      draw_flat_tri( ppx, ppy, x3, y3, x1, c );
    }

    return;
  }

  // s3 must be largest

  if( y3 < y1 ) {

    float slope = (float)(x3 - x1) / (float)(y3 - y1);

    int ppy = y2;
    int ppx = x3 + (float)(y2 - y3) * slope;

    draw_flat_tri( ppx, ppy, x3, y3, x2, c );
    draw_flat_tri( ppx, ppy, x1, y1, x2, c );

  } else {

    float slope = (float)(x1 - x3) / (float)(y1 - y3);

    int ppy = y2;
    int ppx = x1 + (float)(y2 - y1) * slope;

    draw_flat_tri( ppx, ppy, x1, y1, x2, c );
    draw_flat_tri( ppx, ppy, x3, y3, x2, c );
  } 
}
