
#pragma once

class Point {
public:

  float x;
  float y;

  Point();
  Point(float, float);

  void set(float, float);

  void rotate( float );
  void translate( Point& );
  void scale( float );
};

