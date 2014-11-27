
#include <cmath>
#include "point.hh"

Point::Point() {
  set(0, 0);
}

Point::Point( float _x, float _y ) {
  set(_x, _y);
}

void Point::set( float _x, float _y ) {
  x = _x;
  y = _y;
}

void Point::rotate( float th ) {

  float c = cos(th);
  float s = sin(th);

  float nx = c * x + -s * y;
  float ny = s * x +  c * y;

  x = nx;
  y = ny;
}

void Point::translate( Point& p) {
  x += p.x;
  y += p.y;
}

void Point::scale( float m ) {
  x *= m;
  y *= m;
}
