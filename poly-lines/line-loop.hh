
#pragma once

#include <vector>

#include "point.hh"

class LineLoop {
private:

  typedef std::vector<Point> point_vector_t;
  point_vector_t m_points;

public:

  LineLoop( int, int, int );

  void move();

  void draw();
};

