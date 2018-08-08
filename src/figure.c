#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "image.h"
#include "figure.h"

GLuint lava_texture;
char str[15];
int num_of_rocks;
int path_x[25];
int path_z[25];
int new_level;
int jump_ongoing;
int level;
int left_island;
double x;
double y;
double z;
double jumped;
int direction;

void lego_man(){
	//da bi se videlo svetlo
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
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

void lava_floor(GLuint lava_texture){
	
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
void island(){
	
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

void rock(){
	
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

void draw_islands(){
	
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

void rock_line(int z_coord){
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
void generate_path(){
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
	//nema potrebe za poslednjim redom jer se poslednji kamen generise u petlji za pretposlednji red
	
	//resetujemo new_level promenljivu da ne bi pri svakom pokretanju animacije generisao novu stazucd
	new_level=0;
}
void draw_path(){
	//funkcija koja iscrtava vec generisanu stazu pomocu x i z koordinata upisanih u dva niza
	int i=0;
	for(i=0; i<num_of_rocks; i++){
		glPushMatrix();
			glTranslatef(path_x[i], 0, path_z[i]);
			rock();   	
		glPopMatrix();	

	}
}

int check_existing_rock(int x, int z, int num_of_rocks){
	int i;
	//proverava da li je kamen koji zelimo da generisemo vec generisan
	for(i=0; i<num_of_rocks; i++){
		if(path_x[i]==x && path_z[i]==z)
			return 1;
	}
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

