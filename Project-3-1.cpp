
//Harry Patel
//CSCI 172: Computer Graphics
//Project 3: Texture loading on Spheres



#include <string.h>
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <unistd.h>
#include <iostream>
#include <SOIL.h>
#define SPACEBAR 32

using namespace std;

bool WireFrame= false; //Wire frame for all objects is off because it is set to false.
GLUquadricObj *sphere = NULL; //Sphere Pointer.

GLfloat cameraAngle = 0; //This will change the angle of camera so, it will rotate the objects either left or right.
GLfloat cameraZoom = 0; //This will move the objects on the y-axis so, it will zoom in and zoom out the entire model.
GLuint tex; //variable for the Texture.
bool spacebar = false; //Bool variable to start the animation or rotation when space bar is pressed.

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (80.0f,1,0.1f, 150.0f);
}

void Specialkeys(int key, int x, int y)
{

    switch (key) {
        case GLUT_KEY_LEFT : //When you press left arrow key it will rotate the entire model to the left.
            cameraAngle -= 0.5;
            break;

        case GLUT_KEY_RIGHT : //When you press right arrow key it will rotate the entire model to the right.
            cameraAngle += 0.5;
            break;

        case GLUT_KEY_UP : //When you press up arrow key it will move the entire model into the screen which makes it look small.
            cameraZoom -= 0.5;
            break;

        case GLUT_KEY_DOWN : //When you press down arrow key it will move the entire model towards the screen which makes it look bigger.
            cameraZoom += 0.5;
            break;

    }
    glutPostRedisplay();
}

//Texture loader function given in class.
void TextureLoader(char* fileName, GLuint tex)
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, tex); //Given in class. Images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    int width, height; //Given in class. width & height for the image reader.
    unsigned char* image;

    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        cout << "Image Not Found" << endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    // binding image data
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

static void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,15,0.1,0.0,0.0,0.0,0.0,1.0,0.0); //lookAt is camera viewpoint or where we are looking from.

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draw Our Mesh In Wire frame Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Toggle WIRE FRAME

    // your code here
    const double speed = glutGet(GLUT_ELAPSED_TIME) / 1000.0; //This is the speed of each objects rotating around its own axis.
    const double earth = speed*20.0; //This is the speed of earth rotation.
    const double moon = speed*50.0; //This is the speed of moon rotation.
    const double mars = speed*40.0; //This is the speed of mars rotation.

    glBindTexture (GL_TEXTURE_2D, tex); //Binding the images to the objects or texture.
    glTranslated(cameraAngle,cameraZoom,0); //Here we are using the translated function and variables to rotate and zoom objects.

    //Earth
    glPushMatrix();
    TextureLoader("image/earth.jpg", tex); //This is the path to texture image to wrap around the object earth.
    if(spacebar){glRotated(earth, 0, 0, 1);}//This will start rotating the earth if space bar is pressed.
    glTranslatef(0.0, 0.0, 0.0); //Initial position of the earth basically center.
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    gluSphere(sphere,3.5, 40, 40); //Displays the sphere with the given size for earth.
    glPopMatrix();

    //Moon
    glPushMatrix();
    TextureLoader("image/moon.jpg", tex); //This is the path to texture image to wrap around the object moon.
    if(spacebar){glRotated(moon, 0, -0.5, 1);} //This will start rotating the moon if space bar is pressed.
    glTranslatef(3.0, -3.25, -5.0); //Initial position of the moon.
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    gluSphere(sphere, 1.0, 20, 20); //Displays the sphere with the given size for moon.
    glPopMatrix();

    //Mars
    glPushMatrix();
    TextureLoader("image/mars.jpg", tex); //This is the path to texture image to wrap around the object mars.
    if(spacebar){glRotated(mars, 0, -2, 1);} //This will start rotating the mars if space bar is pressed.
    glTranslatef(-3.0, 3.5, -7.0); //Initial position of the mars.
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    gluSphere(sphere, 1.5, 25, 25); //Displays the sphere with the given size for mars.
    glPopMatrix();

    glFlush();
    glutSwapBuffers();

}

static void key(unsigned char key, int x, int y)
{

    switch (key)
    {
        case 27 :
            case 'q':   //when q is pressed the program will exit.
                exit(0);
            break;

        case 'w':       //when w is pressed the program will show the wire frame of every object.
            WireFrame =!WireFrame;
        break;

        case ' ':       // when space bar is pressed the program will start rotating the objects.
            spacebar = true;
        break;
     }
}

static void idle(void)
{

    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

    glFlush();

}

/* Program entry point */

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);

    glutInitWindowSize(800,500);
    glutInitWindowPosition(20,20);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project 3");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    glFlush();
    return EXIT_SUCCESS;

}
