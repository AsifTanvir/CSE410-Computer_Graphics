#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#include <GL/glut.h>

#define pi (2*acos(0.0))

int drawgrid;
int drawaxes;
double arm ;
double max_val;
double angle;

double maxim(double a, double b){
    if(a>b) return a;
    else return b;
}

double mini(double a, double b){
    if(a<b) return a;
    else return b;
}

struct point
{
	double x,y,z;
};

struct point pos,l,r,u;

point crossProduct(struct point vec1,struct point vec2)
{
    struct point newVec;
    newVec.x = vec2.y*vec1.z - vec2.z*vec1.y;
    newVec.y = vec2.z*vec1.x - vec2.x*vec1.z;
    newVec.z = vec2.x*vec1.y - vec2.y*vec1.x;

    return newVec;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    glColor3f(1,1,1);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawAllSquares()
{
    glPushMatrix();
    {
        glTranslated(0,0,max_val);
        drawSquare(arm);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0,0,-max_val);
        drawSquare(arm);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotated(90,1,0,0);
        glTranslated(0,0,max_val);

        drawSquare(arm);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotated(-90,1,0,0);
        glTranslated(0,0,max_val);

        drawSquare(arm);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotated(90,0,1,0);
        glTranslated(0,0,max_val);

        drawSquare(arm);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotated(-90,0,1,0);
        glTranslated(0,0,max_val);

        drawSquare(arm);
    }
    glPopMatrix();
}

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*(pi/2));
			points[i][j].y=r*sin(((double)j/(double)slices)*(pi/2));
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        glColor3f(1,0,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawAllSpheres()
{
    double a = arm;
    double r = max_val - arm;

    glPushMatrix();
    {
        glTranslated(a,a,a);
        drawSphere(r,24,20);    ///Slices 24, stacks 20
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(a,-a,a);
        glRotated(-90,0,0,1);
        drawSphere(r,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(a,a,-a);
        glRotated(180,1,1,0);
        drawSphere(r,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-a,a,a);
        glRotated(90,0,0,1);
        drawSphere(r,24,20);
    }
    glPopMatrix();

    glPushMatrix();{
        glTranslated(-a,-a,a);
        glRotated(180,0,0,1);
        drawSphere(r,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-a,a,-a);
        glRotated(90,0,0,1);
        glRotated(180,1,1,0);
        drawSphere(r,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(a,-a,-a);
        glRotated(-90,0,0,1);
        glRotated(180,1,1,0);
        drawSphere(r,24,20);
    }
    glPopMatrix();

    glPushMatrix();{
        glTranslated(-a,-a,-a);
        glRotated(180,0,0,1);
        glRotated(180,1,1,0);
        drawSphere(r,24,20);
    }
    glPopMatrix();
}

void drawCylinder(double radius,double height,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=height*sin(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*(pi/2));
			points[i][j].y=r*sin(((double)j/(double)slices)*(pi/2));
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        glColor3f(0,1,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

				//lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawAllCylinders()
{
    double r = max_val-arm;
    double a = arm;

    glPushMatrix();
    {
        glTranslated(a,a,0);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(a,-a,0);
        glRotated(-90,0,0,1);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-a,a,0);
        glRotated(90,0,0,1);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-a,-a,0);
        glRotated(180,0,0,1);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0,a,a);
        glRotated(-90,0,1,0);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0,a,-a);
        glRotated(90,0,1,0);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0,-a,a);
        glRotated(-90,0,1,0);
        glRotated(-90,0,0,1);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0,-a,-a);
        glRotated(90,0,1,0);
        glRotated(-90,0,0,1);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(a,0,a);
        glRotated(-90,1,0,0);
        glRotated(-90,0,0,1);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(a,0,-a);
        glRotated(-90,1,0,0);
        //glRotated(-90,0,0,1);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-a,0,a);
        glRotated(90,1,0,0);
        glRotated(90,0,0,1);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-a,0,-a);
        glRotated(90,1,0,0);
        glRotated(180,0,0,1);
        drawCylinder(r,arm,24,20);
    }
    glPopMatrix();
}


point rotateFunc(struct point vec1, struct point vec2, double angle)
{
    struct point newVec;

    ///Cross product
    newVec = crossProduct(vec1,vec2);

    newVec.x = vec1.x * cos(angle) + newVec.x * sin(angle);
    newVec.y = vec1.y * cos(angle) + newVec.y * sin(angle);
    newVec.z = vec1.z * cos(angle) + newVec.z * sin(angle);

    return newVec;
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			l = rotateFunc(l,u,.1);
			r = rotateFunc(r,u,.1);
			break;

        case '2':
			l = rotateFunc(l,u,-.1);
			r = rotateFunc(r,u,-.1);
			break;

        case '3':
			u = rotateFunc(u,r,.1);
			l = rotateFunc(l,r,.1);
			break;

        case '4':
			u = rotateFunc(u,r,-.1);
			l = rotateFunc(l,r,-.1);
			break;

        case '5':
			u = rotateFunc(u,l,.1);
			r = rotateFunc(r,l,.1);
			break;

        case '6':
			u = rotateFunc(u,l,-.1);
			r = rotateFunc(r,l,-.1);
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			pos.x = pos.x - l.x;
			pos.y = pos.y - l.y;
			pos.z = pos.z - l.z;

			break;
		case GLUT_KEY_UP:		// up arrow key
			pos.x = pos.x + l.x;
			pos.y = pos.y + l.y;
			pos.z = pos.z + l.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.x = pos.x + r.x;
			pos.y = pos.y + r.y;
			pos.z = pos.z + r.z;
			break;
		case GLUT_KEY_LEFT:
			pos.x = pos.x - r.x;
			pos.y = pos.y - r.y;
			pos.z = pos.z - r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x = pos.x + u.x;
			pos.y = pos.y + u.y;
			pos.z = pos.z + u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.x = pos.x - u.x;
			pos.y = pos.y - u.y;
			pos.z = pos.z - u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    arm = arm - .5;
		    arm = maxim(arm,0);
			break;
		case GLUT_KEY_END:
		    arm = arm + .5;
		    arm = mini(arm,max_val);
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	gluLookAt(pos.x, pos.y, pos.z,  pos.x + l.x, pos.y + l.y, pos.z + l.z,  u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	drawAllSpheres();
	drawAllSquares();
	drawAllCylinders();

	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	pos = {100.0,100.0,0.0};
	u = {0.0,0.0,1.0};
	l = {-1/sqrt(2),-1/sqrt(2),0};
	r = {-1/sqrt(2),1/sqrt(2),0};

	drawgrid=0;
	drawaxes=1;
	angle=0;
	arm = 12;
	max_val = 20;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

///g++ 1.cpp -o 1.out -lglut -lGLU -lGL
