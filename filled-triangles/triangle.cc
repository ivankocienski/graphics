
#include "common.hh"
#include "triangle.hh"
#include "rasterizer.hh"

#include <stdio.h>

#include <stdlib.h>
#include <math.h>

float frand() {
  return (float)rand() / (float)RAND_MAX;
}

float sfrand() {
  return 1.0 - 2.0 * frand();
}

Triangle::Triangle() { 

  m_scale = 0;
  
  randomize(); 
}

void Triangle::randomize() {
  
  m_hold  =  15 + rand() % 45;
  m_life  = 100 + rand() % 300;
  m_color =   1 + rand() % 4;

  m_angle = sfrand() * M_PI;
  m_angle_inc = sfrand() * 0.05;

  
  m_p1.set( sfrand() * (float)(g_xres / 2), sfrand() * (float)(g_yres / 2) );
  m_p2.set( sfrand() * (float)(g_xres / 2), sfrand() * (float)(g_yres / 2) );
  m_p3.set( sfrand() * (float)(g_xres / 2), sfrand() * (float)(g_yres / 2) );

  m_position.set( rand() % g_xres, rand() % g_yres );
  
  m_pos_inc.x = sfrand() * 2.0;
  m_pos_inc.y = sfrand() * 2.0;
}

void Triangle::move() {

  if( m_hold ) {
    m_hold--;
    return;
  }

  if( m_life ) {

    if( m_scale < 1 ) m_scale += 0.05;

    m_life--;

  } else {
    
    m_scale -= 0.05;
    if( m_scale <= 0 ) {
      m_scale = 0;
      randomize();
      return;
    } 
  }

  m_position.translate( m_pos_inc );

  if( m_position.x < 0 || m_position.x >= g_xres ) {

    m_angle_inc = sfrand() * 0.05;

    m_pos_inc.x = -m_pos_inc.x;
    m_pos_inc.y = sfrand() * 2.0;
  }

  if( m_position.y < 0 || m_position.y >= g_yres ) {

    m_angle_inc = sfrand() * 0.05;

    m_pos_inc.y = -m_pos_inc.y;
    m_pos_inc.x = sfrand() * 2.0;
  }

  m_angle += m_angle_inc;
}

void Triangle::draw() {

  if( m_hold ) return;

  Point tp1 = m_p1;
  Point tp2 = m_p2;
  Point tp3 = m_p3;

  tp1.rotate( m_angle );
  tp2.rotate( m_angle );
  tp3.rotate( m_angle );

  tp1.scale( m_scale );
  tp2.scale( m_scale );
  tp3.scale( m_scale );

  tp1.translate( m_position );
  tp2.translate( m_position );
  tp3.translate( m_position );

  rast_fill_triangle2( tp1.x, tp1.y, tp2.x, tp2.y, tp3.x, tp3.y, m_color );
}

void Triangle::set_pos( int x, int y ) {
  m_position.set( x, y );
}

void Triangle::adjust_angle( int d ) {
  m_angle += (float)d * 0.1;
}
