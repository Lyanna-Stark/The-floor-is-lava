#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <time.h>
#include "image.h"

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

#define LAVATEXTURE "lava.bmp"

static GLuint lava_texture;
static int is_safe();
static int is_dead();
static int check_existing_rock(int x, int z, int num_of_rocks);
static void rock_line(int z_coord);
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
static void reset();
static void initializeTexture(void);

static double z_fix;
static char str[15];
static int num_of_rocks;
static int path_x[25];
static int path_z[25];
static int new_level;
static int jump_ongoing;
static int level;
static int left_island;
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
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	
	//inicijalizuje se on display funkcija
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	
	
	
	svetlo();	
	
	glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);    
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);

	initializeTexture();

	level=0;
	reset();
	
	//glavna petlja
	glutMainLoop();
	return 0;
}

void level_text(const char* tekst) {
	//boja teksta		
	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	//pozicija
	glRasterPos3f(10, 10, 10);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, tekst);
	glEnable(GL_LIGHTING);
}

static void reset(){
	//inicijalizujemo promenljive
	new_level=1;
	jump_ongoing=0;
	left_island=0;
	x=0;
	y=0;

	z=-15;
	//printf("%lf, %lf, %lf\n", x, y, z);
	generate_path();
	glutPostRedisplay();
}
static int is_safe(){
	//proverava da li je stigao do drugog ostrva
	if(abs(z-15)<0.1) 
		return 1;
	else 
		return 0;
}

static int is_dead(){
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

static int check_existing_rock(int x, int z, int num_of_rocks){
	int i;
	//proverava da li je kamen koji zelimo da generisemo vec generisan
	for(i=0; i<num_of_rocks; i++){
		if(path_x[i]==x && path_z[i]==z)
			return 1;
	}
	return 0;
}

static void on_reshape(int width, int height) {
	//viewport
	glViewport(0, 0, width, height);

	//perspektiva tj odsecanje
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, (float) width / height, 1, 1500);
}

static void svetlo(){
	
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

static void lava_floor(GLuint lava_texture){
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, lava_texture);
	
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);

	//funkcija koja crta pod od lave
	glPushMatrix();
		glBegin(GL_QUADS);
			 glNormal3f(0, 1, 0);
			//glColor3f(.941, .4, 0);		
			glTexCoord2f(0, 0);	
			glVertex3f(-400, 0, -400); 
			glTexCoord2f(40, 0);
			glVertex3f(400, 0, -400); 
			glTexCoord2f(40,40);
			glVertex3f(400, 0, 400); 
			glTexCoord2f(0, 60);
			glVertex3f(-400, 0, 400);
		glEnd();
	
		glBindTexture(GL_TEXTURE_2D, 0);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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

static void rock_line(int z_coord){
	int next_dir;
	int last_z;
	do{
		next_dir=rand()%5;
		switch(next_dir){
			case LEFT:
				//ako je ispalo da je naredni kamen levo od prethodnog, z koordinata ostaje ista, nastavljamo dalje da generisemo
				if(path_x[num_of_rocks-1]<10 && !check_existing_rock(path_x[num_of_rocks-1]+5, z_coord, num_of_rocks+1)){
						path_x[num_of_rocks]=path_x[num_of_rocks-1]+5;
					path_z[num_of_rocks]=z_coord;
					last_z=z_coord;
					num_of_rocks++;
				}
				break;
			case RIGHT:				
				//ako je ispalo da je naredni kamen desno od prethodnog, z koordinata ostaje ista, nastavljamo dalje da generisemo

				if(path_x[num_of_rocks-1]>-10 && !check_existing_rock(path_x[num_of_rocks-1]-5, z_coord, num_of_rocks+1)){
					path_x[num_of_rocks]=path_x[num_of_rocks-1]-5;
					path_z[num_of_rocks]=z_coord;
					last_z=z_coord;
					num_of_rocks++;
				}
				break;
			case DIAG_LEFT:				
				//ako je ispalo da je naredni kamen dijagonalno levo od prethodnog, z koordinata se povecava

				if(path_x[num_of_rocks-1]<10){
					path_x[num_of_rocks]=path_x[num_of_rocks-1]+5;
					path_z[num_of_rocks]=z_coord+5;
					last_z=z_coord+5;

					num_of_rocks++;
				}
				break;
			case DIAG_RIGHT:				
				//ako je ispalo da je naredni kamen dijagonalno desno od prethodnog, z koordinata se povecava

				if(path_x[num_of_rocks-1]>-10 && !check_existing_rock(path_x[num_of_rocks-1]-5, z_coord, num_of_rocks+1)){
					path_x[num_of_rocks]=path_x[num_of_rocks-1]-5;
					path_z[num_of_rocks]=z_coord+5;
					last_z=z_coord+5;
					
					num_of_rocks++;
				}
				break;	
			case FORWARD:			
				//ako je ispalo da je naredni kamen ispred prethodnog, z koordinata se povecava

				path_x[num_of_rocks]=path_x[num_of_rocks-1];
				path_z[num_of_rocks]=z_coord+5;
				last_z=z_coord+5;

				num_of_rocks++;
		}
	}while(last_z!=z_coord+5);
	//uslov za izlazak iz petlje je da smo presli u naredni red, tj da je generisan kamen koji nije levo ili desno vec dijagonalno ili napred
	
	
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
	
	//za svaki naredni red kamenja pozivamo funkciju koja generise kamenje u tom redu sve dok ne ,,preskoce" u naredni red tj dok random ne ispadne FORWARD ili DIAG_LEFT ili DIAG_RIGHT
	rock_line(-10);
	rock_line(-5);
	rock_line(0);
	rock_line(5);
	
	//resetujemo new_level promenljivu da ne bi pri svakom pokretanju animacije generisao novu stazucd
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

		//printf("%d: %d, %d\n", i, path_x[i], path_z[i]);
	}
}
static void initializeTexture(void)
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
static void on_keyboard(unsigned char key, int x1, int y1){
	
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

static void on_timer(int value)
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
				//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva
				if(is_safe()){
					level++;
					z_fix++;
					
					reset();
				}    
	
				if(is_dead()){
					reset();
					level=0;
					z_fix++;
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
				jump_ongoing=0;		//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva
				if(is_safe()){
					level++;
					z_fix++;
					
					reset();
				}    
				
				if(is_dead()){
					reset();
					level=0;
					z_fix++;
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
				jump_ongoing=0;			//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva
				if(is_safe()){
					level++;
					z_fix++;
					
					reset();
				}    
			
				if(is_dead()){
					reset();
					level=0;
					z_fix++;
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
				jump_ongoing=0;				//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva
				if(is_safe()){
					level++;
					z_fix++;
					
					reset();
				}    
		
				if(is_dead()){
					reset();
					level=0;
					z_fix++;
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
				jump_ongoing=0;					//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva
				if(is_safe()){
					level++;
					z_fix++;
					
					reset();
				}    
				
				if(is_dead()){
					reset();
					level=0;
					z_fix++;
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
				jump_ongoing=0;					//vracamo coveka nazad i generisemo novu putanju ako je dosao do bezbednog ostrva
				if(is_safe()){
					level++;
					z_fix++;
					
					reset();
				}    
	
				if(is_dead()){
					reset();
					level=0;
					z_fix++;
				}    

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
	
	
	//generisemo putanju
	//if(new_level){
	//	generate_path();
	///}
	
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





