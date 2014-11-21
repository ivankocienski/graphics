
#pragma once

class Point {
public:

  float xmax;
  float xpos;
  float xinc;

  float ymax; 
  float ypos;
  float yinc;


  Point();
  Point(int, int);

  void move();

  int ix();
  int iy();

};

