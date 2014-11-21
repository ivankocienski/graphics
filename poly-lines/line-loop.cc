
#include "line-loop.hh"
#include "lines.hh"

LineLoop::LineLoop( int c, int mx, int my ) {

  m_points.resize( c );

  point_vector_t::iterator it;

  for(it = m_points.begin(); it != m_points.end(); it++)
    (*it) = Point( mx, my );
}

void LineLoop::move() {

  point_vector_t::iterator it;

  for(it = m_points.begin(); it != m_points.end(); it++)
    it->move(); 
}

void LineLoop::draw() { 

  int start_x = m_points[0].ix();
  int start_y = m_points[0].iy();
  int pre_x   = start_x;
  int pre_y   = start_y;

  for( int i = 1; i < m_points.size(); i++ ) {

    draw_line( pre_x, pre_y, m_points[i].ix(), m_points[i].iy(), 255 );

    pre_x = m_points[i].ix();
    pre_y = m_points[i].iy(); 
  }

  draw_line( pre_x, pre_y, start_x, start_y, 255 );
  
}
