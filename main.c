#include <GL/glut.h>

static void on_display(void);

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(200, 200);
	glutCreateWindow(argv[0]);
	
	glutDisplayFunc(on_display);
	glClearColor(0.75, 0.75, 0.75, 0);
	glutMainLoop();
	return 0;
}

static void on_display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	glutSwapBuffers();
}
