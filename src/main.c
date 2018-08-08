#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <time.h>
#include "image.h"
#include "figure.h"

static int is_safe();
static int is_dead();

static void on_reshape(int width, int height);
static void on_display(void);
static void svetlo();
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void reset();
static void initializeTexture(void);

int main(int argc, char** argv){
	//inicijalizujemo glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	
	//kreiramo prozor
	glutInitWindowSize(1300, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	
	//inicijalizuje se on display funkcija
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	
	//ukljucujemo svetlo
	svetlo();	
	
	glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);    
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);

	//ukljucujemo teksture
	initializeTexture();

	level=0;
	reset();
	
	//glavna petlja
	glutMainLoop();
	return 0;
}

void reset(){
	//inicijalizujemo promenljive
	new_level=1;
	jump_ongoing=0;
	left_island=0;
	x=0;
	y=0;
	z=-15;
	//printf("%lf, %lf, %lf\n", x, y, z);
	
	//generise se nova putanja
	generate_path();
	
	//novi prikaz na ekaranu
	glutPostRedisplay();
}
int is_safe(){
	//proverava da li je stigao do drugog ostrva
	if(abs(z-15)<0.1) 
		return 1;
	else 
		return 0;
}

int is_dead(){
	//ako nije ni na jedom kamenu ali je na jednom od ostrva nije mrtav
	if(!left_island || is_safe())
		return 0;
	int i;
	//prolazi kroz niz koordinata kamenja i proverava da li se covek nalazi na kamenu ili je upao u lavu
	for(i=0; i<num_of_rocks; i++){
		if((path_x[i]+2>x && path_x[i]-2<x) && (path_z[i]+2>z && path_z[i]-2<z))
			return 0;
	}
//	printf("dead");
	return 1;
}


void on_reshape(int width, int height) {
	//viewport
	glViewport(0, 0, width, height);

	//perspektiva tj odsecanje
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, (float) width / height, 1, 1500);
}

void svetlo(){
	
	//glEnable(GL_NORMALIZE);

	//namesta se difuzno svetlo
    float diffuse_light[] = {1, 1, 1, 1};    
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	float light[] = {100, 40, 40, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, light);
}

void initializeTexture(void)
{
	//kod sa casa
	    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	/* Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz fajla */
    Image *image = image_init(0, 0);


    /* Kreira se tekstura */
    image_read(image, LAVATEXTURE);

    /* Generisu se identifikatori teksture i inicijalizuje tekstura*/
    glGenTextures(1, &lava_texture);

    glBindTexture(GL_TEXTURE_2D, lava_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
}
void on_keyboard(unsigned char key, int x1, int y1){
	
	switch (key) {
	
	//ako je pritisnut esc izlazimo iz programa
	case 27:
		exit(EXIT_SUCCESS);
		break;
		
	case '6':
	//skace napred, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=FORWARD;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;
	
	case '8':
	//skace levo, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=LEFT;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;
	
	case '2':
	//skace desno, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=RIGHT;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;
	case'3':
	//skace dijagonalno desno, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=DIAG_RIGHT;
			jumped=0;
			jump_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
		}
    break;

	
	case '9':
	//skace dijagonalno levo, jumped se resetuje na pocetku svakog skoka
		if(jump_ongoing==0){
			direction=DIAG_LEFT;
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
  }
}

void on_timer(int value)
{
	//proverava se da li callback dolazi od odgovarajuceg tajmera
	if (value != TIMER_ID)
        return;
	switch(direction){
		case FORWARD:
			left_island=1;
			//ako je presao dovoljno prestaje da skace
			if(jumped<JUMP_LEN){
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

				
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}
			else{
				jump_ongoing=0;
				//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva ili ako je poginuo
				if(is_safe()){
					level++;
					reset();
				}    
	
				if(is_dead()){
					reset();
					level=0;
				}    
			}
			break;
		case LEFT:
			
			if(jumped<JUMP_LEN){
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			x+=.2;
			
			//formula kao gore
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}			
			else{
				jump_ongoing=0;		
				//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva ili ako je poginuo
				
				if(is_safe()){
					level++;
					reset();
				}    
				
				if(is_dead()){
					reset();
					level=0;
				}    

			}

			break;
		case RIGHT:
			if(jumped<JUMP_LEN){
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			x-=.2;
			
			//formula kao gore
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}			
			else{
				jump_ongoing=0;							
				//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva ili ako je poginuo

				if(is_safe()){
					level++;
					reset();
				}    
			
				if(is_dead()){
					reset();
					level=0;
				}    

			}

			break;

		case DIAG_LEFT:
			left_island=1;
			if(jumped<JUMP_LEN){
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			x+=.2;
			z+=.2;
			//formula kao gore
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}			
			else{
				jump_ongoing=0;				
				//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva ili ako je poginuo

				if(is_safe()){
					level++;
					reset();
				}    
		
				if(is_dead()){
					reset();
					level=0;
				}    

			}

			break;
		case DIAG_RIGHT:
			left_island=1;
			//ako je presao dovoljno prestaje da skace
			if(jumped<JUMP_LEN){
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			x-=.2;
			z+=.2;
			
			//formula kao gore
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}			
			else{
				jump_ongoing=0;							
				//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva ili ako je poginuo

				if(is_safe()){
					level++;
					reset();
				}    
				
				if(is_dead()){
					reset();
					level=0;
				}    

			}

			break;
			
		case BACK:
			if(jumped<JUMP_LEN){
			//skacemo po 0.2 odjednom, da ne bi seckala animacija
			jumped+=.2;
			z-=.2;
			
			//formula kao gore
			y=(-4*JUMP_HEIGHT*jumped*jumped)/(JUMP_LEN*JUMP_LEN)+4*JUMP_HEIGHT*jumped/JUMP_LEN;
			
			//ponovo se iscrtava prozor	
			glutPostRedisplay();

			//ako je presao dovoljno prestaje da skace
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
			}			
			else{
				jump_ongoing=0;					
				
				//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva ili ako je poginuo
				if(is_safe()){
					level++;
					
					reset();
				}    
	
				if(is_dead()){
					reset();
					level=0;
				}    

			}

			break;
	}

}

void on_display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//podesavamo lookat
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	//iz svih uglova
	gluLookAt(-30, 17, 0, 0, 0, 0, 0, 1, 0);
	//front
	//gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	// profil
	//gluLookAt(10, 0, 0, 0, 0, 0, 0, 1, 0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//crtamo pod	
	glPushMatrix();
		lava_floor(lava_texture);   	
	glPopMatrix();	
	
	
	//crtamo bezbedna ostrva
	glPushMatrix();
		draw_islands();
	glPopMatrix();
	
	//crtamo putanju
	draw_path();

	//crtamo coveka
    glPushMatrix();
 		//glTranslatef(x, y+3, z);
 		glTranslatef(x, y+3, z);
		lego_man();
	glPopMatrix();	


	//ispisujemo nivo
	sprintf(str, "level: %d", level);
	level_text(str);

	//nova slika
	glutSwapBuffers();
}





