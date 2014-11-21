
#include "point.hh"
#include "util.hh"

Point::Point() {
  xpos = 0;
  ypos = 0;
  xinc = 0;
  yinc = 0;
  xmax = 0;
  ymax = 0;
}

Point::Point( int mx, int my ) {

  xmax = mx;
  ymax = my;

  xinc = 1 - frand() * 2;
  yinc = 1 - frand() * 2;

  xpos = frand() * mx;
  ypos = frand() * my; 
}

void Point::move() {

  xpos += xinc;
  ypos += yinc;

  if( xpos < 1 || xpos > (xmax-1) ) {
    xinc = -xinc;
    yinc = 1 - frand() * 2;
  }

  if( ypos < 1 || ypos > (ymax-1) ) { 
    yinc = -yinc;
    xinc = 1 - frand() * 2; 
  } 
}

int Point::ix() { return (int)xpos; }
int Point::iy() { return (int)ypos; }

