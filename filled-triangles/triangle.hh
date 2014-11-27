
#pragma once

#include "point.hh"

class Triangle {
private:

  Point m_p1;
  Point m_p2;
  Point m_p3;

  Point m_position;
  Point m_pos_inc;

  float m_scale;

  int m_color;
  
  int m_hold;
  int m_life;

  float m_angle;
  float m_angle_inc;
  

public:

  Triangle();

  void randomize();

  void set_pos( int, int );
  void adjust_angle( int );

  void move();
  void draw(); 
};
