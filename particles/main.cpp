
#include <time.h>
#include <iostream>

#include <stdlib.h>




#include "particle_system.h"
#include "SOIL.h"
#include "constants.h"
#include "cloud.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>

using namespace std;

particle_system p(NUMBER_OF_PARTICLES);
float win_width = 400, win_height = 400;
GLuint tex;

//Called when a key is pressed
void handle_keypress(unsigned char key, int x, int y)
{
    switch(key)
    {
	case 'a':
	case 'A':
		p.add_particles(10);
		break;
	
	case 'd':
	case 'D':
		p.delete_particles(10);
		break;


    case 27:
        exit(0);
		break;
    }
    glutPostRedisplay();
}



//Initializes 3D rendering
void init()
{
	// Make big points and wide lines
	glPointSize(1);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

uint initTex(char* c) {
	int width = 280;
	int height = 280;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	unsigned char* image = SOIL_load_image("fire.jpeg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

}


//Called when the window is resized
void handle_resize(int w, int h)
{
	//setup windows width and height
	win_width = (w==0) ? 1 : w;
	win_height = (h==0) ? 1 : h;

    //Tell OpenGL how to convert from coordinates to pixel values
    glViewport(0, 0, win_width, win_height);

	//Switch to setting the camera perspective
    glMatrixMode(GL_PROJECTION);

    //Set the camera perspective
    glLoadIdentity(); //Reset the camera
	glOrtho(-LENGTH, LENGTH, -LENGTH, LENGTH, -LENGTH, LENGTH);
	
	glMatrixMode(GL_MODELVIEW);
}


//Draws the 3D scene
void draw()
{
	glLoadIdentity();
	
	//Draw particles
	glPushMatrix();
		p.advance(DELTA);
		p.delete_particle();
		p.draw(tex);
	glPopMatrix();

	//Draw overlaying quad for trail
	glColor4f(0, 0, 0, 0.1);
	glBegin(GL_QUADS);
		glVertex3f(-LENGTH, -LENGTH, 100);
		glVertex3f(LENGTH, -LENGTH, 100);
		glVertex3f(LENGTH, LENGTH, 100);
		glVertex3f(-LENGTH, LENGTH, 100);
	glEnd();

	glutSwapBuffers();
	glutPostRedisplay();
}


//Handle mouse movement
void mouse_movement(int x, int y ){
	float ww_ratio = float(x)/win_width;
	float wh_ratio = float(y)/win_height;

	p.set_gravity(vec3d((2*ww_ratio-1)*LENGTH, (1-2*wh_ratio)*LENGTH, 0));
}



int main(int argc, char** argv)
{
	srand(time(0));
    p.set_gravity();

	//Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height); //Set the window size

    //Create the window and initialize OpenGL
    glutCreateWindow("Particle System");
    init();

    tex = initTex("do");

    //Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(draw);
    glutKeyboardFunc(handle_keypress);
    glutReshapeFunc(handle_resize);
	glutPassiveMotionFunc(mouse_movement);


    
	glutMainLoop();
    return 0; //This line is never reached
}