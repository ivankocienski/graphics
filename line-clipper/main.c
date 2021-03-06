
#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <time.h>

#include "clipper.h"

#define XRES 800
#define YRES 600

GLFWwindow* window;

double line_x1 = XRES / 2;
double line_y1 = YRES / 2; 
double line_x2 = 0;
double line_y2 = 0; 

int clip_x1 = 200;
int clip_y1 = 200;
int clip_x2 = 600;
int clip_y2 = 400;

void do_stuff() {

  int x1;
  int y1;
  int x2;
  int y2;

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glColor3f( 0.1, 0.1, 0.1 );
  glBegin(GL_LINE_LOOP);
  glVertex2f( clip_x1, clip_y1 );
  glVertex2f( clip_x2, clip_y1 );
  glVertex2f( clip_x2, clip_y2 );
  glVertex2f( clip_x1, clip_y2 );
  glEnd();


  glBegin(GL_LINES);

  glColor3f( 0.5, 0, 0 );
  glVertex2f( line_x1, line_y1 );
  glVertex2f( line_x2, line_y2 );

  glEnd();

  if( clip_line( line_x1, line_y1, line_x2, line_y2, &x1, &y1, &x2, &y2 ) ) {

    glBegin(GL_LINES);
    glColor3f( 0, 1, 0 );
    glVertex2f( x1, y1 );
    glVertex2f( x2, y2 );
    glEnd();
  }


}

void init_stuff() {

  setup_line_clipper( clip_x1, clip_y1, clip_x2, clip_y2 );

  glViewport(0, 0, XRES, YRES);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0, (float)XRES, (float)YRES, 0.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable( GL_LINE_SMOOTH );
  
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void cleanup() {
  glfwTerminate();
}

int main( int argc, char** argv ) {

  struct timespec ts;

  ts.tv_sec  = 0;
  ts.tv_nsec = 50 * 1000;

  if(!glfwInit()) {
    fprintf( stderr, "could not init glfw\n" );
    return -1;
  }

  atexit( cleanup );

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow( XRES, YRES, "line clipping", NULL, NULL );
  if(!window) {
    fprintf( stderr, "could not open window\n" );
    return -1;
  }

  glfwMakeContextCurrent(window);

  init_stuff();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {

    if( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_1 ) == GLFW_PRESS ) 
      glfwGetCursorPos( window, &line_x1, &line_y1 );

    if( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_2 ) == GLFW_PRESS ) 
      glfwGetCursorPos( window, &line_x2, &line_y2 );
    

    do_stuff();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();

    nanosleep( &ts, NULL ); 
  } 

  init_stuff();

  return 0;
}

