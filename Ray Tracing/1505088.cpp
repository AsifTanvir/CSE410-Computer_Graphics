#include<bits/stdc++.h>


#include <GL/glut.h>
#include "bitmap_image.hpp"

using namespace std;

#define pi (2*acos(0.0))

int drawgrid;
int drawaxes;
double angle;
double recursion;
double pixelNumber;
double objNum;
double windwHeight;
int flag=0;
double lightSourceNum;


ifstream inputFile("description.txt");


class point
{
public:
	double x,y,z;
	point(double x,double y,double z)
	{
	    this->x = x;
	    this->y = y;
	    this->z = z;
	}
	point()
	{

	}

    point operator+(point p)
    {
        return point(this->x + p.x, this->y + p.y, this->z + p.z);
    }

    point operator-(point p)
    {
        return point(this->x - p.x, this->y - p.y, this->z - p.z);
    }
};

class Triangle
{
public:
    point point1;
    point point2;
    point point3;

    Triangle(point point1,point point2,point point3)
    {
        this->point1 = point1;
        this->point2 = point2;
        this->point3 = point3;
    }
};

struct objectType
{
    string name;
    point centre,color;
    double ambient, diffuse, specular, reflection , exponent , rad, height, width;
    vector<Triangle> triangles;
};

class Ray
{
public:
    point start;
    point dir;

    Ray(point start,point dir)
    {
        this->start = start;
        this->dir = dir;
    }
};

point pos,l,r,u;
point centre,color;
vector<objectType> objectVector;
vector<vector<point> > midPoints;
double assignedColor[3];
vector<Triangle> triangle_vector;
vector<point> lightSources;

point addPoints(point p1,point p2)
{
    point p;
    p.x = p1.x + p2.x;
    p.y = p1.y + p2.y;
    p.z = p1.z + p2.z;

    return p;
}

point subtractPoints(point p1,point p2)
{
    point p;
    p.x = p1.x - p2.x;
    p.y = p1.y - p2.y;
    p.z = p1.z - p2.z;

    return p;
}

point crossProduct(struct point vec1,struct point vec2)
{
    struct point newVec;
    newVec.x = vec2.y*vec1.z - vec2.z*vec1.y;
    newVec.y = vec2.z*vec1.x - vec2.x*vec1.z;
    newVec.z = vec2.x*vec1.y - vec2.y*vec1.x;

    return newVec;
}

point scalarProduct(point vec1, double x)
{
    point newVec;

    newVec.x = vec1.x * x;
    newVec.y = vec1.y * x;
    newVec.z = vec1.z * x;

    return newVec;
}
double dotProduct(point v1,point v2)
{
    double p;
    p = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    return p;
}

point normalize(point p)
{
    point newP;
    double val,add;
    add = p.x*p.x + p.y*p.y + p.z*p.z;
    val = sqrt(add);

    newP.x = p.x/val;
    newP.y = p.y/val;
    newP.z = p.z/val;

    return newP;
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




double inputPoint()
{
    string in;
    inputFile>>in;
    if(in == "") return 9999.0;
    double din = stod(in);
    return din;
}

void drawSquare(point a,double width)
{
	glBegin(GL_QUADS);{
		glVertex3f( a.x, a.y,0);
		glVertex3f( a.x,a.y+width,0);
		glVertex3f(a.x+width,a.y+width,0);
		glVertex3f(a.x+width, a.y,0);
	}glEnd();
}

void drawGrid(double width)
{
	if(drawgrid==1)
	{
	    double c =1;
		for (int i = -50; i <= 50; i++) {
			for (int j = -50; j <= 50; j++) {
				glPushMatrix(); {
					glColor3f(c, c, c);
					//glTranslatef(j*checker_board_width, i*checker_board_width, 0);
					point p;
					p.x = -width*(i + 0.5);
					p.y = -width*(j + 0.5);
					p.z =  0;
					//repeat -= 1;
					drawSquare(p , width);
					c = 1 - c;
				}glPopMatrix();
			}
		}
	}
}



void drawSphere(double radius,int slices,int stacks)
{
	point points[100][100];
	int i, j;
	double h, r;
	//generate points
	for (i = 0; i <= stacks; i++)
	{
		h = radius * sin(((double)i / (double)stacks)*(pi / 2));
		r = radius * cos(((double)i / (double)stacks)*(pi / 2));
		for (j = 0; j <= slices; j++)
		{
			points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
			points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0; i < stacks; i++)
	{
		//glColor3f((double)i / (double)stacks, (double)i / (double)stacks, (double)i / (double)stacks);
		for (j = 0; j < slices; j++)
		{
			glBegin(GL_QUADS); {
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				//lower hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
			}glEnd();
		}
	}
}



void drawPyramid(point base,double height,double width,point colorP)
{
    glPushMatrix();
    {
        glColor3f(colorP.x,colorP.y,colorP.z);
        drawSquare(base,width);

        point top;
        double r = width/2;
        top.x = base.x + r;
        top.y = base.y + r;
        top.z = base.z + height;

        glColor3f(colorP.x,colorP.y,colorP.z);
//        point n;
//        n.x = base.x+width;n.y = base.y;n.z = base.z;
//        Triangle t1(top,base,n);
//        triangle_vector.push_back(t1);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(top.x,top.y,top.z);
            glVertex3f(base.x,base.y,base.z);
            glVertex3f(base.x+width,base.y,base.z);
        }
        glEnd();

//        glColor3f(colorP.x,colorP.y,colorP.z);
//        n.x = base.x;n.y = base.y+width;n.z = base.z;
//        Triangle t2(top,base,n);
//        triangle_vector.push_back(t2);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(top.x,top.y,top.z);
            glVertex3f(base.x,base.y,base.z);
            glVertex3f(base.x,base.y+width,base.z);
        }
        glEnd();

        glColor3f(colorP.x,colorP.y,colorP.z);
//        point m;
//        m.x = base.x+width;m.y = base.y+width;m.z = base.z;
//        n.x = base.x+width;n.y = base.y;n.z = base.z;
//        Triangle t3(top,m,n);
//        triangle_vector.push_back(t3);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(top.x,top.y,top.z);
            glVertex3f(base.x+width,base.y+width,base.z);
            glVertex3f(base.x+width,base.y,base.z);
        }
        glEnd();

        glColor3f(colorP.x,colorP.y,colorP.z);
//        m.x = base.x+width;m.y = base.y+width;m.z = base.z;
//        n.x = base.x;n.y = base.y+width;n.z = base.z;
//        Triangle t4(top,m,n);
//        triangle_vector.push_back(t4);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(top.x,top.y,top.z);
            glVertex3f(base.x+width,base.y+width,base.z);
            glVertex3f(base.x,base.y+width,base.z);
        }
        glEnd();
    }glPopMatrix();
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

//void createPixelArray()
//{
//    cout<<"midpoint hoise"<<endl;
//    double height = 2;
//    point midPoint = addPoints(pos,l); /// pos + l*nearP // nearP = 1
//    double pixelHeight = height/pixelNumber; ///assuming height of the square is 2
//    double halfHeight = height/2;
//
////    point** midPoints;
////    midPoints = new point* [pixelNumber];
////    for(int i=0;i<pixelNumber ; i++){
////        midPoints[i] = new point[pixelNumber];
////    }
//
//
//    for(int i =0 ; i< pixelNumber ; i++){
//        cout<<"midpoint hoinai"<<endl;
//        for(int j =0 ; j<pixelNumber ; j++){
//            cout<<"midpoint hoinai"<<endl;
//            point p;
//            p.x = midPoint.x + (i-halfHeight)*pixelHeight*u.x + (j-halfHeight)*pixelHeight*r.x;
//            p.y = midPoint.y + (i-halfHeight)*pixelHeight*u.y + (j-halfHeight)*pixelHeight*r.y;
//            p.x = midPoint.z + (i-halfHeight)*pixelHeight*u.z + (j-halfHeight)*pixelHeight*r.z;
//            cout<<"midpoint blamar"<<endl;
//
//            midPoints[pixelNumber-i-1][j].x = p.x;
//            midPoints[pixelNumber-i-1][j].y = p.y;
//            midPoints[pixelNumber-i-1][j].z = p.z;
//            cout<<"midpoint hoise"<<endl;
//        }
//    }
//
//}
point getNormalSphere(point p,point centre)
{
    point newP;
    newP = p-centre;
    return normalize(newP);
}

point getNormalTriangle(Triangle t)
{
    point newP;

    point AB = t.point2 - t.point1;
    point AC = t.point3 - t.point1;

    newP = crossProduct(AB,AC);
    return normalize(newP);
}

point getNormalTiles()
{
    return point(0,0,1);
}

point getReflection(Ray ray,point intersect,point normal)
{
    double r;
    point p,q;
    r = 2 * dotProduct(ray.dir,normal);
    p = scalarProduct(normal,r);
    q = p - ray.dir;

    return normalize(q);
}

double trim(double a,double mx,double mn)
{
    if(a>mx) return mx;
    if(a<mn) return mn;
}

double solution(double a,double b,double c,int sign)
{
    double t,det;
    double calc = (b*b) - 4 *a *c;
    if(calc<0) return -9999;
    det = sqrt(calc);
    //cout<<"Det "<<det<<endl;
    t = (-b + (sign *det))/2*a;
    //cout<<t<<" t"<<endl;
    return t;
}

double interSectSphere(Ray ray,int i)
{
    double t,t1,t2;
    point P;

    double a,b,c;
    point Ro = ray.start;
    point Rd = normalize(ray.dir);

    objectType obj = objectVector.at(i);
    point centure = obj.centre;
    double r = obj.rad;
    point dif = subtractPoints(Ro,centure);

    b = dotProduct(Rd,dif);
    b = 2*b;
    //cout<<b<<" b"<<endl;
    a = dotProduct(Rd,Rd);

    c = dotProduct(dif,dif);
    c = c - r*r;

    t1 = solution(a,b,c,1);
    t2 = solution(a,b,c,-1);

    if(t1 == -9999 || t2 == -9999) return -9999;
    t = min(t1,t2);
    return t;
}
///Möller–Trumbore intersection algorithm
double interSectTriangle(Ray ray,Triangle t)
{
    double tt;
    point Ro = ray.start;
    point Rd = normalize(ray.dir);

    double epsilon = 0.0000001;
    point p0 = t.point1;
    point p1 = t.point2;
    point p2 = t.point3;

    point edge1, edge2, h, s, q;
    edge1 = p1 - p0;
    edge2 = p2 - p0;
    double a,f,u,v;
    h = crossProduct(Rd,edge2);
    a = dotProduct(edge1,h);

    if(a>-epsilon && a<epsilon)
        return -1;

    f = 1.0/a;
    s = Ro - p0;
    u = dotProduct(s,h)*f;
    if (u < 0.0 || u > 1.0)
        return -1;

    q = crossProduct(s,edge1);
    v = dotProduct(Rd,q)*f;
    if (v < 0.0 || u + v > 1.0)
        return -1;

    tt = dotProduct(edge2,q)*f;
    return tt;
}

double intersectTiles(Ray ray)
{
    double t;
    point Ro = ray.start;
    point Rd = normalize(ray.dir);

    if(Rd.z == 0) return -1;

    t = -(Ro.z/Rd.z);

    return t;
}


void IlluminationSphere(objectType obj,double t,Ray ray)
{
    double lambert,phong;
    point point_intersect = ray.start + scalarProduct(ray.dir,t);
    point N = getNormalSphere(point_intersect,obj.centre);

    point R = getReflection(ray,point_intersect,N);

    for(int i =0 ;i < lightSourceNum ;i++)
    {
        point direction,lightSource,start;
        lightSource = lightSources.at(i);
        direction = lightSource - point_intersect;
        direction = normalize(direction);

        start = point_intersect + scalarProduct(direction,1);
        Ray L(start,direction);
        point V = normalize(scalarProduct(point_intersect,-1.0));

        bool flagIllum = false;
        double t;
        double tri[4];
        for(int k =0 ; k<objectVector.size() ; k++){
            if(objectVector.at(k).name == "Sphere"){
                t = interSectSphere( L , k);
                if(t>0){
                    flagIllum = true;
                    break;
                }
            }
            else if(objectVector.at(k).name == "Pyramid"){
                for(int l =0 ; l<4 ; l++){
                    tri[l] = interSectTriangle(L,objectVector.at(k).triangles.at(l));
                    if(tri[l] > 0){
                        flagIllum = true;
                        break;
                    }
                }
            }
            else{
                t = intersectTiles(L);
                if(t>0){
                    flagIllum = true;
                    break;
                }
            }
        }

        if(!flagIllum)
        {
            lambert = obj.diffuse * dotProduct(L.dir,N);
            phong = obj.specular * pow(dotProduct(R,V),obj.exponent);
        }

    }
    trim(lambert,1.0,0.0);
    trim(phong,1.0,0.0);

    cout<<lambert<<" "<<phong<<endl;
    assignedColor[0] = (lambert+ phong+ obj.ambient) * (obj.color.x);
    assignedColor[1] = (lambert+ phong+ obj.ambient) * (obj.color.y);
    assignedColor[2] = (lambert+ phong+ obj.ambient) * (obj.color.z);
}

void IlluminationTriangle(objectType obj,Triangle triangle,Ray ray,double t)
{
    double lambert,phong;
    point point_intersect = ray.start + scalarProduct(ray.dir,t);
    point N = getNormalTriangle(triangle);

    point R = getReflection(ray,point_intersect,N);

    for(int i =0 ;i < lightSourceNum ;i++)
    {
        point direction,lightSource,start;
        lightSource = lightSources.at(i);
        direction = lightSource - point_intersect;
        direction = normalize(direction);

        start = point_intersect + scalarProduct(direction,1);
        Ray L(start,direction);
        point V = normalize(scalarProduct(point_intersect,-1.0));

        bool flagIllum = false;
        double t;
        double tri[4];
        for(int k =0 ; k<objectVector.size() ; k++){
            if(objectVector.at(k).name == "Sphere"){
                t = interSectSphere( L , k);
                if(t>0){
                    flagIllum = true;
                    break;
                }
            }
            else if(objectVector.at(k).name == "Pyramid"){
                for(int l =0 ; l<4 ; l++){
                    tri[l] = interSectTriangle(L,objectVector.at(k).triangles.at(l));
                    if(tri[l] > 0){
                        flagIllum = true;
                        break;
                    }
                }
            }
            else{
                t = intersectTiles(L);
                if(t>0){
                    flagIllum = true;
                    break;
                }
            }
        }

        if(!flagIllum)
        {
            lambert = obj.diffuse * dotProduct(L.dir,N);
            phong = obj.specular * pow(dotProduct(R,V),obj.exponent);
        }

    }
    cout<<lambert<<" "<<phong<<endl;
    assignedColor[0] = (lambert+ phong+ obj.ambient) * (obj.color.x);
    assignedColor[1] = (lambert+ phong+ obj.ambient) * (obj.color.y);
    assignedColor[2] = (lambert+ phong+ obj.ambient) * (obj.color.z);

}

void createImage()
{
    bitmap_image image(pixelNumber,pixelNumber);
//    for (int x = 0; x < pixelNumber; x++) {
//        for (int y = 0; y < pixelNumber; y++) {
//            image.set_pixel(x, y, 0.0, 0.0,0.0);
//        }
//    }

    double planeDis = (windwHeight/2)/tan(angle*pi/360);
    point topLeft;
    topLeft.x = pos.x + (l.x * planeDis - r.x * (windwHeight/2) + u.x * (windwHeight/2));
    topLeft.y = pos.y + (l.y * planeDis - r.y * (windwHeight/2) + u.y * (windwHeight/2));
    topLeft.z = pos.z + (l.z * planeDis - r.z * (windwHeight/2) + u.z * (windwHeight/2));

    cout<<planeDis<<" "<<topLeft.x<<" "<<topLeft.y<<" "<<topLeft.z<<endl;

    double d = (windwHeight*1.0)/pixelNumber;
    cout<<d<<" "<<flag<<endl;
    for(int i =0 ; i<pixelNumber ; i++){
        for(int j =0 ; j<pixelNumber ; j++){
            point corner;
            corner.x = topLeft.x + r.x*i*d - u.x*j*d;
            corner.y = topLeft.y + r.y*i*d - u.y*j*d;
            corner.z = topLeft.z + r.z*i*d - u.z*j*d;

            point Rd = subtractPoints(corner,pos);
            Ray ray(pos,Rd);
            double nearest = -1;
            double t;
            double tri[4];
            assignedColor[0] = 0.0;
            assignedColor[1] = 0.0;
            assignedColor[2] = 0.0;

            double min_t = 9999.0;
            for(int k =0 ; k<objectVector.size() ; k++){
                if(objectVector.at(k).name == "Sphere"){
                    t = interSectSphere( ray , k);
                }
                else if(objectVector.at(k).name == "Pyramid"){
                    for(int l =0 ; l<4 ; l++){
                        tri[l] = interSectTriangle(ray,objectVector.at(k).triangles.at(l));
                        if(tri[l] <= 0) continue;
                        if(tri[l]<min_t) {
                            min_t = tri[l];
                            Triangle t = objectVector.at(k).triangles.at(l);
                            IlluminationTriangle(objectVector.at(k),t,ray,tri[l]);
//                            assignedColor[0] = objectVector.at(k).color.x;
//                            assignedColor[1] = objectVector.at(k).color.y;
//                            assignedColor[2] = objectVector.at(k).color.z;
                            //cout<<"color "<<assignedColor[0]<<" "<<assignedColor[1]<<" "<<assignedColor[2]<<endl;
                        }
                    }
                }
                else{
                    t = intersectTiles(ray);
                    double colorTile;
                    double width = 100*100;
                    if(t<=0) continue;
                    if(t<min_t){
                        min_t = t;
                        point intersection = ray.start + scalarProduct(ray.dir,t);
                        int a = floor(intersection.x/width);
                        int b = floor(intersection.y/width);
                        if((abs(a)+abs(b))%2 == 1) colorTile = 1.0;
                        else colorTile = 0.0;

                        assignedColor[0] = colorTile;
                        assignedColor[1] = colorTile;
                        assignedColor[2] = colorTile;
                    }
                }

                if(t<=0) continue;
                if(t<min_t) {
                    min_t = t;
                    IlluminationSphere(objectVector.at(k),t,ray);
//                    assignedColor[0] = objectVector.at(k).color.x;
//                    assignedColor[1] = objectVector.at(k).color.y;
//                    assignedColor[2] = objectVector.at(k).color.z;
                    //cout<<"color "<<assignedColor[0]<<" "<<assignedColor[1]<<" "<<assignedColor[2]<<endl;
                }
            }
            //cout<<"color "<<i<<" "<<j<<" "<<assignedColor[0]<<" "<<assignedColor[1]<<" "<<assignedColor[2]<<endl;
            image.set_pixel(i, j,assignedColor[0]*255, assignedColor[1]*255, assignedColor[2]*255);
            }
    }
    image.save_image("out.bmp");
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
        case '0':
            createImage();
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


	for(int i =0 ; i<objectVector.size() ; i++)
    {
        objectType com = objectVector.at(i);
        if(com.name == "Sphere"){
            glPushMatrix();
            {
                glTranslatef(com.centre.x,com.centre.y,com.centre.z);
                glColor3f(com.color.x,com.color.y,com.color.z);
                drawSphere(com.rad,24,20);
            }
            glPopMatrix();
        }
        else if(com.name == "Pyramid"){
            drawPyramid(com.centre,com.height,com.width,com.color);
        }
        else{
            drawGrid(com.width);
        }
    }


	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	pos = {200.0,150.0,100.0};
	u = {0.0,0.0,1.0};
	l = {-1/sqrt(2),-1/sqrt(2),0};
	r = {-1/sqrt(2),1/sqrt(2),0};

	drawgrid=1;
	drawaxes=0;
	angle=90;
	windwHeight=500;
	assignedColor[0] = 0.0;
    assignedColor[1] = 0.0;
    assignedColor[2] = 0.0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();
	recursion = inputPoint();
	pixelNumber = inputPoint();
	objNum = inputPoint();
	cout<<recursion<<" "<<pixelNumber<<" "<<objNum<<endl;

	for(int i =0 ; i< objNum ;i++)
    {
        string command;
        inputFile>>command;

        cout<<command<<endl;
        if(command == "sphere"){
            //point centre,color;
            //double ambient, diffuse, specular, reflection , exponent;
            objectType sphere;
            sphere.name = "Sphere";
            sphere.centre.x = inputPoint();
            sphere.centre.y = inputPoint();
            sphere.centre.z = inputPoint();
            sphere.rad = inputPoint();

            sphere.color.x = inputPoint();
            sphere.color.y = inputPoint();
            sphere.color.z = inputPoint();

            sphere.ambient = inputPoint();
            sphere.diffuse = inputPoint();
            sphere.specular = inputPoint();
            sphere.reflection = inputPoint();
            sphere.exponent = inputPoint();
            sphere.height = 0.0;
            sphere.width = 0.0;

            objectVector.push_back(sphere);

            cout<<command<<endl;
        }
        else if(command == "pyramid"){
            //point centre,color;
            //double ambient, diffuse, specular, reflection , exponent;
            objectType pyramid;
            pyramid.name = "Pyramid";

            pyramid.centre.x = inputPoint();
            pyramid.centre.y = inputPoint();
            pyramid.centre.z = inputPoint();
            pyramid.width = inputPoint();
            pyramid.height = inputPoint();
            pyramid.rad = 0.0;

            pyramid.color.x = inputPoint();
            pyramid.color.y = inputPoint();
            pyramid.color.z = inputPoint();

            pyramid.ambient = inputPoint();
            pyramid.diffuse = inputPoint();
            pyramid.specular = inputPoint();
            pyramid.reflection = inputPoint();
            pyramid.exponent = inputPoint();

            double r = pyramid.width/2;
            point base = pyramid.centre;
            double width = pyramid.width;
            point top(base.x + r,base.y + r,base.z + pyramid.height);
            point n(base.x+width,base.y,base.z);
            Triangle t1(top,base,n);
            triangle_vector.push_back(t1);

            n.x = base.x;n.y = base.y+width;n.z = base.z;
            Triangle t2(top,base,n);
            triangle_vector.push_back(t2);

            point m;
            m.x = base.x+width;m.y = base.y+width;m.z = base.z;
            n.x = base.x+width;n.y = base.y;n.z = base.z;
            Triangle t3(top,m,n);
            triangle_vector.push_back(t3);

            m.x = base.x+width;m.y = base.y+width;m.z = base.z;
            n.x = base.x;n.y = base.y+width;n.z = base.z;
            Triangle t4(top,m,n);
            triangle_vector.push_back(t4);

            pyramid.triangles = triangle_vector;
            objectVector.push_back(pyramid);
            triangle_vector.clear();
        }
    }
    objectType floor;
    floor.name = "Floor";
    floor.width = 50.0;
    objectVector.push_back(floor);

    lightSourceNum = inputPoint();
    for(int i =0 ; i<lightSourceNum ; i++)
    {
        point l;
        l.x = inputPoint();
        l.y = inputPoint();
        l.z = inputPoint();
        lightSources.push_back(l);
    }

	//give PERSPECTIVE parameters
	gluPerspective(angle,	1,	1,	1000.0);
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
