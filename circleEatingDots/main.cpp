#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <stdlib.h>
#include <time.h>


GLint consumed = 0;

GLint incX = 1;
GLint incY = 1;

GLint tempincX = incX;
GLint tempincY = incY;


GLint halfSize = 40;
vec2 center = { 200, 200 };

GLint glutTimerFuncValue = 5;

GLint windowWidth = 400;
GLint windowHeight = 300;

GLint pointX = 0;
GLint pointY = 0;

GLint pointSize = 1;

GLint keyStates[256];

void spawnDot() {

	pointX = rand() % windowWidth;
	pointY = rand() % windowHeight;

	float diffX = center[0] - pointX;
	float diffY = center[1] - pointY;

	while ( 
		(   (pointX < halfSize && pointY < halfSize)
		||  (pointX > windowWidth - halfSize && pointY > windowHeight - halfSize )
		||  (pointX < halfSize && pointY > windowHeight - halfSize)
		||  (pointX > windowWidth - halfSize && pointY < halfSize) )
		||
		halfSize*halfSize > (diffY * diffY) + (diffX *diffX)
		) {

		pointX = rand() % windowWidth;
		pointY = rand() % windowHeight;

		diffX = center[0] - pointX;
		diffY = center[1] - pointY;
	}

}

void spawnDotToCoord(GLint x, GLint y) {

	pointX = x;
	pointY = y;
}

void consumeDot() {

	float diffX = center[0] - pointX;
	float diffY = center[1] - pointY;
	
	if ( halfSize*halfSize >= (diffY * diffY) + (diffX *diffX) )
	{

		if (
			(center[0] + halfSize + 1 + incX < windowWidth ) &&
			(center[0] - halfSize - 1 + incX > 0 ) &&
			(center[1] + halfSize + 1 + incY < windowHeight ) &&
			(center[1] - halfSize - 1 + incY > 0 )
			)
		{
			halfSize++;
			printf("halfSize increased: %d \n", halfSize);

		}
		else {

			printf("halfSize did not increase: %d \n", halfSize);
		}

		consumed++;
		printf("Consumed: %d\n",consumed);
		if (consumed >= 10) {
			printf("Game over.");
			incX = 0;
			incY = 0;
			pointX = -1;
			pointY = -1;
		}
		else {
			spawnDot();
		}

	}

}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = 1;
}

void keyUp(unsigned char key, int x, int y)
{
	keyStates[key] = 0;
}

void keyOperations(int value)
{
	if (keyStates['w'] ) {
		incY *= -1;
		keyStates['w'] = 0;
	}

	if (keyStates['d'] ) {
		incX *= -1;
		keyStates['d'] = 0;
	}
	//stop
	if (keyStates['e'] && incX != 0 && incY != 0) {

		tempincX = incX;
		tempincY = incY;
		incX = 0;
		incY = 0;
		keyStates['e'] = 0;
	}

	//start
	if (keyStates['r'] && incX == 0 && incY == 0) {
		incX = tempincX;
		incY = tempincY;
		keyStates['r'] = 0;
	}

	glutPostRedisplay();
	
	glutTimerFunc(glutTimerFuncValue, keyOperations, 0);
}

void init()
{
	srand(time(NULL));
	glEnable(GL_POINT_SMOOTH);
	glPointSize(pointSize);
	spawnDot();
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);
	glShadeModel(GL_FLAT);
}

void circle(vec2 O, GLdouble r) {
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
	glEnd();
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	circle(center, halfSize);
	consumeDot();
	glBegin(GL_POINTS);
	glVertex2f(pointX, pointY);
	glEnd();
	glutSwapBuffers();
}

void update(int n)
{
	center[0] += incX;
	center[1] += incY;

	if (center[0] + halfSize >= windowWidth || center[0] - halfSize <= 0)
		incX *= -1;

	if (center[1] + halfSize >= windowHeight || center[1] - halfSize <= 0)
		incY *= -1;

	glutPostRedisplay();

	glutTimerFunc(glutTimerFuncValue, update, 0);

}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glutIgnoreKeyRepeat(1);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(glutTimerFuncValue, update, 0);
	glutTimerFunc(glutTimerFuncValue, keyOperations, 0);
	glutMainLoop();
	return 0;
}
