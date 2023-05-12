#include <string.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
/* GLUT callback Handlers */

float a,b,c,d,e,f;  //declare variables
float x,y,xo,yo;    //declare variables
int it;     //declare iterative variable for loop

void resize(int width, int height)
{
    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
    xo=1;
    yo=1;
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport

}

void display(void)
{

  glClear (GL_COLOR_BUFFER_BIT);    // clear display screen
  // your code here
  //number of iterations = 200000.
  for(int i=0; i<200000; i++)   //loop through the iterations
  {

      it = rand()%100;     //generates a random number between 0-99.
      if (it<=7)    //7% with f4.
      {
          a= -0.15, b =0.26, c =0.28, d =0.24, e =0, f=0.44;
      }
       else if (it<=14)     //7% with f3
       {
           a= 0.2, b =0.23, c =-0.26, d =0.22, e =0, f=1.6;
       }
       else if (it<=99)     //85% with f2
       {
           a= 0.85, b = -0.04, c =0.04, d =0.85, e =0, f=1.6;
       }
       else {       //remaining 1% with f1.
        a= 0, b =0, c =0,d =0.16, e =0, f=0;
       }
       /*Two equations x&y will plot the dots on the cartesian coordinate
       to graph the Barnsley's fractal fern.*/
       x= a*xo+c*yo+e;
       y= b*xo+d*yo+f;
        glColor3f(0.0,4.0,4.0);
        glBegin(GL_POINTS);     //To begin draw points.
        glVertex3f(x, y,0);     //maps the calculated pixels onto the screen.
        glEnd();        //close the gl.
        xo=x;       //Save the current x value from our equation above.
        yo=y;       //Save the current y value from our equation above.
  }
  glFlush ();                           // clear buffer
}


void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (650, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Program1");                //program title
   init();
   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutMainLoop();                               //loop

    return EXIT_SUCCESS;
}
