
//Harry Patel
//CSCI 172: Computer Graphics
//Project 4: Skybox

#include <string.h>

#ifdef APPLE
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <SOIL.h>
#include <math.h>

using namespace std;
const bool onOff  = true; //This is the bool type variable to check the condition in keys.
GLuint tex; // Variable to load the texture of type GLuint.


//light settings
GLfloat position[] = { 0.0f, 1.0f, 1.0f, 0.0f };//for my light
const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Declare variables for inputs to functions */

//cube position for glTranslatef function, rotation angle variables to rotate, scale multiplier, camera position
GLfloat cubeX, cubeY, cubeZ,cubeAngleX,cubeAngleY,skyBoxAngleX,skyBoxAngleY,sMulC,sbMulC,camX,camY=0;

//size of cube for glTranslatef function
GLfloat cubeSize=1;

//bool to trigger the wireframe functionality
bool WireFrame = false;

/* GLUT callback Handlers */

//Resize function given in framework.
static void resize(int width, int height)
{
    double Ratio;

    width = (float)width;
    height = (float)height;

    Ratio = (double)width / (double)height;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0f, Ratio, 0.1f, 1500.0f);
}

//load images for cube and skybox
 void loadTexture(char *fileName){
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    int width, height;                 // width & height for the image reader
    unsigned char *image;
    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGB);
    if(!image)  //If there is no image or it cannot find it, then it will print an error
       cout << "Failed to find image" << endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // binding image data
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//Displays the object and the skybox
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //creates viewing matrix derived from an eye point,
    //a reference point indicating the center of the scene, and an UP vector.
    //(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    gluLookAt(camX, camY, 3, camX, camY, 0.0, 0.0, 1.0, 0.0);

    if (WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    glTranslatef(0,0,sMulC);
    glRotatef(skyBoxAngleX,1,0,0);
    glRotatef(skyBoxAngleY,0,1,0);

    //CREATE SKYBOX
    //This is the room or skybox.
    glPushMatrix();
        glScalef(5.0, 5.0, 5.0); //Make it 5 times bigger on all sides
        glDisable(GL_NORMALIZE);    // This will Disable normalize
        glDisable(GL_LIGHTING);     //This will Disable lighting so that everything is visible and bright

        //An Front image.
        loadTexture("images/brick.png"); //This image will be loaded into all the four sides of the wall.(front,back,left,right).
        glBegin(GL_QUADS);  //GL_QUADS allows us to create a 4 sided polygon that we can use. These are the coordinates that we use and their vertexes.
            glTexCoord2f(1.0, 0.0);     //This is the First point (x, y), these also set the current texture's coordinates on a 2D plane.
            glVertex3f(-1.0, 1.0, -1.0);    //These specify the vertexes, basically the points that we are using.

            glTexCoord2f(1.0, 1.0);     //This is the Second point of the front side image.
            glVertex3f(-1.0, -1.0, -1.0);

            glTexCoord2f(0.0, 1.0);     //This is the third point of the front side image.
            glVertex3f(1.0, -1.0, -1.0);

            glTexCoord2f(0.0, 0.0);     //This is the Fourth point of the front side image.
            glVertex3f(1.0, 1.0, -1.0);
        glEnd();

        //Right side image
        glBegin(GL_QUADS);                  //Creates the right side wall of the box.
            glTexCoord2f(1.0, 0.0);         //This is the first point of the right side image.
            glVertex3f(1.0, 1.0, -1.0);

            glTexCoord2f(1.0, 1.0);         //This is the second point of the right side image.
            glVertex3f(1.0, -1.0, -1.0);

            glTexCoord2f(0.0, 1.0);         //This is the third point of the right side image.
            glVertex3f(1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 0.0);         //This is the fourth point of the right side image.
            glVertex3f(1.0, 1.0, 1.0);
        glEnd();

        //Left side image
        glBegin(GL_QUADS);                  //Creates the left side wall of the box.
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-1.0, 1.0, -1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(-1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(-1.0, -1.0, -1.0);
        glEnd();


        //Back side image
        glBegin(GL_QUADS);                  //Creates the back side wall of the box.
            glTexCoord2f(0.0, 0.0);         //All four points are opposite side of the front side of the image.
            glVertex3f(-1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(-1.0, -1.0, 1.0);
        glEnd();

        //Ceiling of the skybox
        loadTexture("images/c.png");
        glBegin(GL_QUADS);             //Creates a ceiling or top of the box.
            glTexCoord2f(0.0, 1.0);
            glVertex3f(1.0, 1.0, -1.0);

            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(-1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0, 1.0, -1.0);
        glEnd();

        //Floor of the skybox
        glBegin(GL_QUADS);              //Creates a floor or bottom of the box (opposite of ceiling).
            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, -1.0, -1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(-1.0, -1.0, -1.0);
        glEnd();

        glEnable(GL_NORMALIZE);
    glPopMatrix();

    //CREATE CUBE
    //This is basically the skybox code but the scale is smaller than the skybox to make the cube.
    glPushMatrix();
    glRotatef(cubeAngleX,1,0,0);
    glRotatef(cubeAngleY,0,1,0);
        glScalef(1.0, 1.0, 1.0); //scale the cube size to 1 which makes it smaller cube inside of a skybox.
        glDisable(GL_NORMALIZE);    //Disable normalize
        glDisable(GL_LIGHTING);     //Disable lighting

        //Front
        loadTexture("images/crate.jpg"); //This image will be loaded onto the cube object.
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 0.0);     //This is the First point (x, y), these also set the current texture's coordinates on a 2D plane
            glVertex3f(-1.0, 1.0, -1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0, -1.0, -1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(1.0, -1.0, -1.0);

            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, 1.0, -1.0);
        glEnd();

        //Right
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 1.0, -1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(1.0, -1.0, -1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, 1.0, 1.0);
        glEnd();

        //Left
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-1.0, 1.0, -1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(-1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(-1.0, -1.0, -1.0);
        glEnd();


        //Back
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(-1.0, -1.0, 1.0);
        glEnd();

        //Top
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(1.0, 1.0, -1.0);

            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(-1.0, 1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0, 1.0, -1.0);
        glEnd();

        //Bottom
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(-1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex3f(1.0, -1.0, 1.0);

            glTexCoord2f(0.0, 0.0);
            glVertex3f(1.0, -1.0, -1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex3f(-1.0, -1.0, -1.0);
        glEnd();

        glEnable(GL_NORMALIZE); //Enable Normalization
    glPopMatrix();
    glutSwapBuffers();
}

//Assign functionality for keys
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        //press ESC or 'q' to quit the program.
    case 27:
    case 'q':
        exit(0);
        break;

        //press 'a' key to rotate left around the Y axis.
    case 'a':
        if (onOff) cout << "ROTATE CUBE LEFT AROUND Y AXIS\n";
        cubeAngleY -= 5; //set the angle to be rotated to 5 degrees
        break;

        //press 'd' key to rotate right around the Y axis.
    case 'd':
        if (onOff)  cout << "ROTATE CUBE RIGHT AROUND Y AXIS\n";
        cubeAngleY += 5; //set the angle to be rotated to 5 degrees
        break;

        //press 'w' key to rotate up around the X axis.
    case 'w':
        if (onOff) cout << "ROTATE CUBE UP AROUND X AXIS\n";
        cubeAngleX += 5; //set the angle to be rotated to 5 degrees
        break;

        //press 's' key to rotate down around the X axis.
    case 's':
        if (onOff) cout << "ROTATE CUBE DOWN AROUND X AXIS\n";
        cubeAngleX -= 5; //set the angle to be rotated to 5 degrees
        break;

        //press 'j' key to rotate camera left around the Y axis
    case 'j':
        if (onOff) cout << "ROTATE CAMERA LEFT AROUND Y AXIS\n";
        camX -= .2;
        break;

        //press 'l' key to rotate camera right around the Y axis
    case 'l':
        if (onOff) cout << "ROTATE CAMERA RIGHT AROUND Y AXIS\n";
        camX += .2;
        break;

        //press 'i' key to rotate camera up around the X axis
    case 'i':
        if (onOff) cout << "ROTATE CAMERA UP AROUND X AXIS\n";
        camY += .2;
        break;

        //press 'k' key to rotate camera down around the X axis
    case 'k':
        if (onOff) cout << "ROTATE CAMERA DOWN AROUND X AXIS\n";
        camY -= .2;
        break;

        //press 'o' key to resets camera to starting position
    case 'o':
        if (onOff) cout << "RESET CAMERA POSITION\n";
        camX = 0;
        camY = 0;
        break;

        //press 'z' key to zoom in
    case 'z':
        if (onOff) cout << "ZOOM IN\n";
        sMulC += 0.2; //increments by .2 in scaling function (above 1.0 stretches)
        sbMulC += 0.2; //increments by .2 in scaling function (above 1.0 stretches)
        break;

        //press 'x' key to zoom out
    case 'x':
        if (onOff) cout << "ZOOM OUT\n";
        sMulC -= 0.2; //decrements by .2 in scaling function (below 1.0 shrinks)
        sbMulC -= 0.2; //decrements by .2 in scaling function (below 1.0 shrinks)
        break;

        //press space bar to reset the entire scene.
    case ' ':
        if (onOff) cout << "RESET SCENE\n";
        //camera position
        camX = 0;
        camY = 0;

        //rotation degree variables for glRotatef
        cubeAngleX = 0, cubeAngleY = 0;
        skyBoxAngleX = 0, skyBoxAngleY = 180;
        break;

        //press 'f' and is able to display the cube in wireframe mode
    case 'f':
        if (onOff) cout << "\nWireframe..\n";
        WireFrame = !WireFrame;//change the wireframe flag back and forth
        break;
    }
}

//Assign functionality for keys UP, DOWN, RIGHT, LEFT, HOME, END
void Specialkeys(int key, int x, int y)
{
    switch (key)
    {
        //user presses key UP and SKYBOX is able to preform a rotation around the X axis
    case GLUT_KEY_UP:
        if (onOff) cout << "ROTATE SKYBOX UP AROUND X AXIS\n";
        skyBoxAngleX -= 5; //set the angle to be rotated to 5 degrees
        break;

        //user presses key DOWN and SKYBOX is able to preform a rotation around the X axis
    case GLUT_KEY_DOWN:
        if (onOff) cout << "ROTATE SKYBOX DOWN AROUND X AXIS\n";
        skyBoxAngleX += 5; //set the angle to be rotated to 5 degrees
        break;

        //user presses key LEFT and SKYBOX is able to preform a left rotation around the Y axis
    case GLUT_KEY_LEFT:
        if (onOff) cout << "ROTATE SKYBOX LEFT AROUND Y AXIS\n";
        skyBoxAngleY -= 5; //set the angle to be rotated to 5 degrees
        break;

        //user presses key RIGHT and SKYBOX is able to preform a right rotation around the Y axis
    case GLUT_KEY_RIGHT:
        if (onOff)  cout << "ROTATE SKYBOX RIGHT AROUND Y AXIS\n";
        skyBoxAngleY += 5; //set the angle to be rotated to 5 degrees
        break;
    }
    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

//Initialize all variables with starting values
static void init(void)
{
    //camera position
    camX = 0;
    camY = 0;

    //cube position for glTranslatef function
    cubeX = 0.0, cubeY = 0.0, cubeZ = -2.0;

    //rotation degree variables for glRotatef
    cubeAngleX = 0, cubeAngleY = 0;
    skyBoxAngleX = 0, skyBoxAngleY = 180;

    //scale multiplier (set to 1 when not in use)
    sMulC = -2;  //for cube
    sbMulC = 6;  //for skyBox

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}

/* Program entry point */

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1000, 900);//starting size of window
    glutInitWindowPosition(0, 0);//start window pos
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project 4: SkyBox");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
