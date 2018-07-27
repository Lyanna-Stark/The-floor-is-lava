#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <time.h>


static void on_reshape(int width, int height);
static void on_display(void);
static void svetlo();
static void lego_man();
static void lava_floor();
static void rock();

int main(int argc, char** argv){
	//inicijalizujemo glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	
	//kreiramo prozor
	glutInitWindowSize(1300, 800);
	glutInitWindowPosition(200, 200);
	glutCreateWindow(argv[0]);
	
	//inicijalizuje se on display funkcija
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	
	glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
	
	svetlo();
	
	//glavna petlja
	glutMainLoop();
	return 0;
}

static void on_reshape(int width, int height) {
	//viewport
	glViewport(0, 0, width, height);

	//perspektiva tj odsecanje
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, (float) width / height, 1, 1700);
}

static void svetlo(){
	
	glEnable(GL_NORMALIZE);

	//namesta se difuzno svetlo
    float diffuse_light[] = {1, 1, 1, 1};    
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	float light[] = {40, 40, 40, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, light);
	
}

void lego_man(){
	//da bi se videlo svetlo
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	/*
	 * 
	 * CRTAMO COVEKA
	 * 
	 */
	
	//trup
	glPushMatrix();    
		glColor3f(1, 0, 0);
		glScalef(1, 1.43, .5);
		glutSolidCube(1);
	glPopMatrix();

	//ruke
	glPushMatrix();
		glColor3f(.8, .65, .45);
		glTranslatef(-.62, .084, 0);
		glScalef(.2, 1.3, .2);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(.62, .084, 0);
		glScalef(.2, 1.3, .2);
		glutSolidCube(1);
	glPopMatrix();
	
	//noge
	glPushMatrix();
		glColor3f(0, 0, .4);
		glTranslatef(-.35, -1.5, 0);
		glScalef(.3, 1.5, .3);
		glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
		glColor3f(0, 0, 0.4);
		glTranslatef(.35, -1.5, 0);
		glScalef(.3, 1.5, .3);
		glutSolidCube(1);
	glPopMatrix();

	//vrat 
	glPushMatrix();  
		glColor3f(.8, .65, .45);
		glTranslatef(0, .86, 0);
		glScalef(.3, .3, .3);
		glutSolidCube(1);
	glPopMatrix();
	
	//glava
	glPushMatrix();
		glColor3f(.8, .65, .45);
		glTranslatef(0, 1.3, 0);
		glScalef(.35, .45, .3);
		glutSolidSphere(1, 20, 20);
	glPopMatrix();

	//oci
	glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(-.17, 1.3, .32);
		glScalef(.04, .04, .04);
		glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(0.17, 1.3, .32);
		glScalef(.04, .04, .04);
		glutSolidSphere(1, 20, 20);
	glPopMatrix();
		
		
}

static void lava_floor(){

	glPushMatrix();
	//glTranslatef(0, 0, 0);
	glBegin(GL_QUADS);
		glColor3f(.941, .4, 0);
		glVertex3f(0, 0,50);
		glVertex3f(100,0,50);
		glVertex3f(100, 0, -50);
		glVertex3f(0, 0, -50);
	glEnd();

	glPopMatrix();

}

static void rock(){
	glPushMatrix();    
		glColor3f(.3, .3, .3);
		glScalef(2, .3, 2);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, .3, 0);
		glColor3f(.3, .3, .3);
		glScalef(1.8, .25, 1.8);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, .55, 0);
		glColor3f(.3, .3, .3);
		glScalef(1.6, .15, 1.6);
		glutSolidCube(1);
	glPopMatrix();

}

static void on_display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	//podesavamo lookat
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	//iz svih uglova
	gluLookAt(-20, 5, 0, 0, 0, 0, 0, 1, 0);
	//front
	//gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	// profil
	//gluLookAt(10, 0, 0, 0, 0, 0, 0, 1, 0);

	//crtamo pod	
	glPushMatrix();
		glTranslatef(-10, 0,0);
		lava_floor();   	
	glPopMatrix();	

	glPushMatrix();
		glTranslatef(0, 0,0);
		rock();
	glPopMatrix();	

	//crtamo coveka
    glPushMatrix();
		glTranslatef(0, 3,0);
		lego_man();
	glPopMatrix();	
	
	//nova slika
	glutSwapBuffers();
}





