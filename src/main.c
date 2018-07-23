#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <time.h>


static void on_display(void);

int main(int argc, char** argv){
	//inicijalizujemo glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	
	//kreiramo prozor
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(200, 200);
	glutCreateWindow(argv[0]);
	
	//inicijalizuje se on display funkcija
	glutDisplayFunc(on_display);
	glClearColor(0.75, 0.75, 0.75, 0);
	
	//glavna petlja
	glutMainLoop();
	return 0;
}

static void on_display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//podesavamo perspektivu
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	
	
	
	//nova slika
	glutSwapBuffers();
}
