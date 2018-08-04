#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <time.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20
#define JUMP_LEN 5
#define JUMP_HEIGHT 2
#define FORWARD 0
#define LEFT 1
#define RIGHT 2
#define DIAG_LEFT 3
#define DIAG_RIGHT 4
#define BACK 5

static void draw_path();
static void generate_path();
static void draw_islands();
static void island();
static void on_reshape(int width, int height);
static void on_display(void);
static void svetlo();
static void lego_man();
static void lava_floor();
static void rock();
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);


static int num_of_rocks;
static int path_x[25];
static int path_z[25];
static int new_level;
static int jump_ongoing;
static double x;
static double y;
static double z;
static double jumped;

static int direction;

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
	glutKeyboardFunc(on_keyboard);
	
	glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
	
	new_level=1;
	jump_ongoing=0;
	x=0;
	y=0;
	z=0;
	

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
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	float light[] = {100, 40, 40, 0};
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
	
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);

	//funkcija koja crta pod od lave
	glPushMatrix();
		glBegin(GL_QUADS);
			glColor3f(.941, .4, 0);			
			glVertex3f(-200, 0, -100); 
			glVertex3f(200, 0, -100); 
			glVertex3f(200, 0, 100); 
			glVertex3f(-200, 0, 100);
		glEnd();
	glPopMatrix();
	
}


static void island(){
	
	//funkcija koja crta ostrvo, slicna onoj za kamen, crta prvo bazu
	
	glPushMatrix();    
		glColor3f(.3, .3, .3);
		glScalef(2, .3, 2);
		glutSolidCube(1);
	glPopMatrix();
	
	//sredina
	glPushMatrix();
		glTranslatef(0, .3, 0);
		glColor3f(.3, .3, .3);
		glScalef(1.93, .25, 1.93);
		glutSolidCube(1);
	glPopMatrix();
	
	//vrh
	glPushMatrix();
		glTranslatef(0, .55, 0);
		glColor3f(.3, .3, .3);
		glScalef(1.86, .15, 1.86);
		glutSolidCube(1);
	glPopMatrix();

}

static void rock(){
	
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);

	//funkcija koja crta kamen, prvo bazu
	
	glPushMatrix();    
		glColor3f(.3, .3, .3);
		glScalef(2, .3, 2);
		glutSolidCube(1);
	glPopMatrix();
	
	//sredina
	glPushMatrix();
		glTranslatef(0, .3, 0);
		glColor3f(.3, .3, .3);
		glScalef(1.8, .25, 1.8);
		glutSolidCube(1);
	glPopMatrix();
	
	//vrh
	glPushMatrix();
		glTranslatef(0, .55, 0);
		glColor3f(.3, .3, .3);
		glScalef(1.6, .15, 1.6);
		glutSolidCube(1);
	glPopMatrix();

}

static void draw_islands(){
	
	//crtamo prvo ostrvo
	glPushMatrix();
		glTranslatef(0, 0,-29);
		glScalef(15, 1, 15);
		island();
	glPopMatrix();	
	
	//crtamo drugo ostrvo
	glPushMatrix();
		glTranslatef(0, 0, 29);		
		glScalef(15, 1, 15);
		island();
	glPopMatrix();	
}
static void generate_path(){
	//funkcija koja random generise stazu od ostrva do ostrva i upisuje x i y koordinate svakog kamena u niz path_x i path_z i broj kamenja u num_of_rocks
	time_t t;
	//broj kamenja
	num_of_rocks=0;
	
	//random seed
	srand((unsigned) time(&t));
	
	//x koordinata prvog kamena, moguce vrednosti -10, -5, 0, 5, 10
	int rand_pos=-2+rand()%5;
	path_x[0]=rand_pos*5;
	path_z[0]=-10;
	num_of_rocks=1;
	int next_dir;
	do{
		next_dir=rand()%5;
		switch(next_dir){
			case LEFT:
				if(path_x[num_of_rocks-1]!=10){
					path_x[num_of_rocks]=path_x[num_of_rocks-1]+5;
					path_z[num_of_rocks]=-10;
					
					num_of_rocks++;
				}
				break;
			case RIGHT:
				if(path_x[num_of_rocks-1]!=-10){
					path_x[num_of_rocks]=path_x[num_of_rocks-1]-5;
					path_z[num_of_rocks]=-10;
					num_of_rocks++;
				}
				break;
			case DIAG_LEFT:
				if(path_x[num_of_rocks-1]!=10){
					path_x[num_of_rocks]=path_x[num_of_rocks-1]+5;
					path_z[num_of_rocks]=-5;
					num_of_rocks++;
				}
				break;
			case DIAG_RIGHT:
				if(path_x[num_of_rocks]!=-10){
					path_x[num_of_rocks]=path_x[num_of_rocks-1]-5;
					path_z[num_of_rocks]=-5;
					num_of_rocks++;
				}
				break;	
			case FORWARD:
				path_x[num_of_rocks]=path_x[num_of_rocks-1];
				path_z[num_of_rocks]=-5;
				num_of_rocks++;
		}
	}while(next_dir==LEFT || next_dir==RIGHT);


	new_level=0;
}
static void draw_path(){
	//funkcija koja iscrtava vec generisanu stazu pomocu x i z koordinata upisanih u dva niza
	int i=0;
	for(i=0; i<num_of_rocks; i++){
		glPushMatrix();
			glTranslatef(path_x[i], 0, path_z[i]);
			rock();   	
		glPopMatrix();	

		printf("%d: %d, %d\n", i, path_x[i], path_z[i]);
	}
}
static void on_keyboard(unsigned char key, int x, int y){
	switch (key) {
	
	//ako je pritisnut esc izlazimo iz programa
	case 27:
		exit(EXIT_SUCCESS);
		break;
		
	case '5':
	//skace napred, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=FORWARD;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;
	
	case '7':
	//skace levo, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=LEFT;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;
	
	case '1':
	//skace desno, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=RIGHT;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;
	
	case '4':
	//skace nazad, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=BACK;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;
	
	case'2':
	//skace dijagonalno desno, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=DIAG_RIGHT;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;

	
	case '8':
	//skace dijagonalno levo, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=DIAG_LEFT;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;

  }
}


static void on_timer(int value)
{
	//proverava se da li callback dolazi od odgovarajuceg tajmera
	if (value != TIMER_ID)
        return;
	switch(direction){
		case FORWARD:
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			z+=.2;
			
			/*
			* ovo je formula koju sam izvela direktno iz y=a*z*z+b*z+c
			* ako su z1=0 i z2=MAX_LEN i y(z1)=y(z2)=0
			* 	a z3=JUMP_LEN y(z3)=JUMP_HEIGHT tj globalni maksimum 
			* 	onda je a=-4*JUMP_HEIGHT/(JUMP_LEN*JUMP_LEN)
			* 	b=4*JUMP_HEIGHT/JUMP_LEN
			*	a z je jumped jer se pri svakom skoku jumped resetuje na nulu dok z nastavlja da se povecava
			
			*/
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
			if(jumped<JUMP_LEN){
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}
			else{
				jump_ongoing=0;
			}
			break;
		case LEFT:
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			x+=.2;
			
			//formula kao gore
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
			if(jumped<JUMP_LEN){
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}			
			else{
				jump_ongoing=0;
			}

			break;
		case RIGHT:
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			x-=.2;
			
			//formula kao gore
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
			if(jumped<JUMP_LEN){
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}			
			else{
				jump_ongoing=0;
			}

			break;

		case DIAG_LEFT:
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			x+=.2;
			z+=.2;
			//formula kao gore
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
			if(jumped<JUMP_LEN){
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}			
			else{
				jump_ongoing=0;
			}

			break;
		case DIAG_RIGHT:
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			x-=.2;
			z+=.2;
			
			//formula kao gore
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
			if(jumped<JUMP_LEN){
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}			
			else{
				jump_ongoing=0;
			}

			break;
		case BACK:
			//skacemo po 0.1 odjednom, da ne bi seckala animacija
			jumped+=.1;
			z-=0.1;
			
			
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
			if(jumped<JUMP_LEN){
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}
			else{
				jump_ongoing=0;
			}
			break;
	}

}

static void on_display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//podesavamo lookat
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	//iz svih uglova
	gluLookAt(-25, 17, 0, 0, 0, 0, 0, 1, 0);
	//front
	//gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	// profil
	//gluLookAt(10, 0, 0, 0, 0, 0, 0, 1, 0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//crtamo pod	
	glPushMatrix();
		glTranslatef(-10, 0,0);
		lava_floor();   	
	glPopMatrix();	
	
	//crtamo coveka
    glPushMatrix();
		glTranslatef(x, y+3, z-15);
		lego_man();
	glPopMatrix();	
	
	//crtamo bezbedna ostrva
	draw_islands();
	
	//generisemo putanju
	if(new_level){
		generate_path();
	}
	
	draw_path();
	//nova slika
	glutSwapBuffers();
}





