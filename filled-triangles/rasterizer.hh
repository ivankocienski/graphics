
#pragma once


// setup some parameters
void rast_setup( unsigned char *, int, int, int );

// halfspace algorithm
void rast_fill_triangle( int, int, int, int, int, int );

// progressive scan algorithm
void rast_fill_triangle2( int, int, int, int, int, int, unsigned char );

