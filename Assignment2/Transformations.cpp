#include<bits/stdc++.h>

#define pi (2*acos(0.0))

using namespace std;

double view[3][3];              /// 1st row eye , 2nd row look , 3rd row up vectors
double fovY;
double aspectRatio;
double near;
double far;
ifstream inputFile("scene.txt");
ofstream outputFile("stage1.txt");
ofstream outputFile2("stage2.txt");
ofstream outputFile3("stage3.txt");

struct point
{
	double x,y,z;
};

struct matrix{
    double mat[4][4];
};

struct smallStk{
    stack<matrix> S;
};

struct matrix mat;
stack<smallStk> bigStack;
smallStk smallStack;

point crossProduct(struct point vec1,struct point vec2)
{
    struct point newVec;
    newVec.x = vec2.y*vec1.z - vec2.z*vec1.y;
    newVec.y = vec2.z*vec1.x - vec2.x*vec1.z;
    newVec.z = vec2.x*vec1.y - vec2.y*vec1.x;

    return newVec;
}

double dotProduct(point vec1, point vec2)
{
    double product;
    product = (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
    return product;
}

point scalarProduct(point vec1, double x)
{
    point newVec;

    newVec.x = vec1.x * x;
    newVec.y = vec1.y * x;
    newVec.z = vec1.z * x;

    return newVec;
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

double inputPoint()
{
    string in;
    inputFile>>in;
    if(in == "") return 9999.0;
    double din = stod(in);
    return din;
}

matrix productMatrix(double a[4][4] , double b[4][4])
{
    matrix product;


//    for(int i =0 ; i < 4 ;i++){
//        for(int j =0 ; j< 4 ; j++){
//            cout<<a[i][j] << " "<<b[i][j]<<" ";
//        }
//        cout<<endl;
//    }

    for(int i =0 ; i < 4 ;i++){
        for(int j =0 ; j< 4 ; j++){
            product.mat[i][j] = 0;
        }
    }

    for(int i =0 ; i< 4; i++){
        for(int j =0 ; j<4 ; j++){
            for(int k =0 ; k<4 ; k++){
                product.mat[i][j] += a[i][k]*b[k][j];
            }
        }
    }
    return product;
}

point transformPoint(matrix m, point p)
{
    double product[4][1];
    double arr[4][1];
    arr[0][0] = p.x;
    arr[1][0] = p.y;
    arr[2][0] = p.z;
    arr[3][0] = 1.0;

    for(int i =0 ; i< 4; i++){
        product[i][0] = 0;
    }

    for(int i =0 ; i< 4; i++){
        for(int j =0 ; j<4 ; j++){
            product[i][0] += m.mat[i][j]*arr[j][0];
        }
    }

    point q;
    q.x = product[0][0] / product[3][0]; ///Scaling the product by making the w 0
    q.y = product[1][0] / product[3][0];
    q.z = product[2][0] / product[3][0];

    return q;
}



point Rodrigeus(point x, point a, double angle)
{
    point R,ang,scalDot,scalDotAng,cross,crossAng;
    double kone,dot,newKone,sinKone;
    if(angle == 90) kone = 0;
    else kone = cos(pi*angle/180);

    ang = scalarProduct(x,kone); ///

    newKone = 1-kone;
    dot = dotProduct(a,x);
    scalDot = scalarProduct(a,dot);
    scalDotAng = scalarProduct(scalDot,newKone); ///

    sinKone = sin(pi*angle/180);
    cross = crossProduct(x,a);
    crossAng = scalarProduct(cross,sinKone); ///

    R.x = ang.x + scalDotAng.x + crossAng.x;
    R.y = ang.y + scalDotAng.y + crossAng.y;
    R.z = ang.z + scalDotAng.z + crossAng.z;

    return R;

}

void ModelingTransform(string command)
{
    //stack<matrix> S;


    if(command == "triangle"){
        point t[3];
        point P;
        ///Input of three points
        for(int i =0 ; i<3 ; i++){
            t[i].x = inputPoint();
            t[i].y = inputPoint();
            t[i].z = inputPoint();
        }

        for(int i =0 ; i<3 ; i++){
            P = transformPoint(bigStack.top().S.top(),t[i]);
            cout.precision(7);
            outputFile<<fixed<<P.x <<" " <<P.y << " " << P.z<<"\n";
        }
        outputFile<<"\n";
    }
    else if(command == "translate")
    {
        double trans1,trans2,trans3;

        trans1 = inputPoint();
        trans2 = inputPoint();
        trans3 = inputPoint();

        double transMat[4][4];
        for(int i =0 ; i <4 ; i++){
            for(int j =0 ; j<4 ; j++){
                if(i == j) transMat[i][j] = 1;
                else if(i == 0 && j == 3) transMat[i][j] = trans1;
                else if(i == 1 && j == 3) transMat[i][j] = trans2;
                else if(i == 2 && j == 3) transMat[i][j] = trans3;
                else transMat[i][j] = 0;
            }
        }

        matrix T = productMatrix(bigStack.top().S.top().mat,transMat);

        bigStack.top().S.push(T);
    }

    else if(command == "scale")
    {
        double scale[3];
        for(int i =0 ; i<3 ; i++)
        {
            scale[i] = inputPoint();
        }
        double scaleMat[4][4];

        for(int i =0 ; i <4 ; i++){
            for(int j =0 ; j<4 ; j++){
                if(i == 3 && j == 3) scaleMat[i][j]=1;
                else if(i == j) scaleMat[i][j] = scale[i];
                else scaleMat[i][j] = 0;
            }
        }

        matrix Sc = productMatrix(bigStack.top().S.top().mat,scaleMat);

        bigStack.top().S.push(Sc);
    }

    else if(command == "rotate")
    {
        double angle;
        point a;

        angle = inputPoint();
        a.x = inputPoint();
        a.y = inputPoint();
        a.z = inputPoint();

        point C[3],unit[3];
        unit[0].x=1;unit[0].y=0;unit[0].z=0;
        unit[1].x=0;unit[1].y=1;unit[1].z=0;
        unit[2].x=1;unit[2].y=0;unit[2].z=1;

        a = normalize(a);

        for(int i =0 ; i<3 ; i++)
        {
            C[i] = Rodrigeus(unit[i],a,angle);
        }

        double rotMat[4][4];
        for(int i = 0 ; i< 4 ; i++)
        {
            for(int j =0 ; j<4 ; j++)
            {
                if(i == 3 && j == 3) rotMat[i][j]=1;
                else if(j == 3) rotMat[i][j] = 0;
                else if(i == 0) rotMat[i][j] = C[j].x;
                else if(i == 1) rotMat[i][j] = C[j].y;
                else if(i == 2) rotMat[i][j] = C[j].z;
            }
        }
        matrix Rt = productMatrix(bigStack.top().S.top().mat,rotMat);

        bigStack.top().S.push(Rt);

    }

    else if(command == "push")
    {
        smallStk a ;
        a.S.push(bigStack.top().S.top());
        bigStack.push(a);
    }
    else if(command == "pop")
    {
        bigStack.pop();
    }

}

matrix viewTransform(point eye,point look , point up)
{
    point l,r,u;
    l.x = look.x - eye.x;
    l.y = look.y - eye.y;
    l.z = look.z - eye.z;

    l = normalize(l);

    r = crossProduct(up,l);
    r = normalize(r);

    u = crossProduct(l,r);

    double T[4][4];
    for(int i =0 ; i<4 ; i++){
        for(int j =0 ; j<4 ; j++){
            if(i == j) T[i][j] = 1;
            else if(i == 0 && j == 3) T[i][j] = -(eye.x);
            else if(i == 1 && j == 3) T[i][j] = -(eye.y);
            else if(i == 2 && j == 3) T[i][j] = -(eye.z);
            else T[i][j]=0;
        }
    }

    double R[4][4];
    for(int i =0 ; i<4 ; i++){
        for(int j =0 ; j<4 ; j++){
            if(i == 0 && j ==0) R[i][j] = r.x;
            else if(i == 0 && j ==1) R[i][j] = r.y;
            else if(i == 0 && j ==2) R[i][j] = r.z;
            else if(i == 1 && j ==0) R[i][j] = u.x;
            else if(i == 1 && j ==1) R[i][j] = u.y;
            else if(i == 1 && j ==2) R[i][j] = u.z;
            else if(i == 2 && j ==0) R[i][j] = -(l.x);
            else if(i == 2 && j ==1) R[i][j] = -(l.y);
            else if(i == 2 && j ==2) R[i][j] = -(l.z);

            else if(i == 3 && j == 3) R[i][j] =1;
            else R[i][j]=0;
        }
    }

    matrix V = productMatrix(R,T);
    return V;
}

matrix projectionTransform()
{
    double fovX,t,r;

    fovX = fovY * aspectRatio;
    t = near * tan((pi*fovY)/360);
    r = near * tan((pi*fovX)/360);

    matrix P;
    double z1 = -(far+near)/(far-near);
    double z2 = -(2*far*near)/(far-near);
    for(int i =0 ; i<4 ; i++){
        for(int j =0 ; j<4 ; j++){
            if(i == 0 && j ==0) P.mat[i][j] = near/r;
            else if(i == 1 && j == 1) P.mat[i][j] = near/t;
            else if(i == 2 && j == 2) P.mat[i][j] = z1;
            else if(i == 2 && j == 3) P.mat[i][j] = z2;
            else if(i == 3 && j == 2) P.mat[i][j] = -1;
            else P.mat[i][j] = 0;
        }
    }

    return P;
}


int main(int argc, char **argv){

    point eye,look,up;

    eye.x = inputPoint();eye.y = inputPoint();eye.z = inputPoint();
    look.x = inputPoint();look.y = inputPoint();look.z = inputPoint();
    up.x = inputPoint();up.y = inputPoint();up.z = inputPoint();

    string fov,aspect,nearStr,farStr;

    inputFile>>fov;
    fovY = stod(fov);
    inputFile>>aspect;
    aspectRatio = stod(aspect);
    inputFile>>nearStr;
    near = stod(nearStr);
    inputFile>>farStr;
    far = stod(farStr);

    matrix identity;
    double ident[4][4]; /// Identity matrix;
    for(int i =0 ; i< 4 ; i++){
        for(int j =0 ; j< 4 ; j++){
            if(i == j) identity.mat[i][j] =1;
            else identity.mat[i][j] = 0;
        }
    }
    smallStack.S.push(identity);
    bigStack.push(smallStack);
    while(1){
        string com;
        inputFile >> com;
        if(com == "end") break;
        ModelingTransform(com);
    }

    inputFile.close();
    outputFile.close();
    inputFile.open("stage1.txt");

    ///ViewTransformation
    matrix V = viewTransform(eye,look,up);
    while(1){

        point p[3],r;
        int flag = 0;
        for(int i =0 ; i<3 ; i++){
            p[i].x = inputPoint();
            if(p[i].x == 9999.0){
                flag = 1;
                break;
            }
            p[i].y = inputPoint();
            p[i].z = inputPoint();
        }
        if(flag == 1) break;

        for(int i =0 ; i<3 ; i++){
            r = transformPoint(V,p[i]);
            cout.precision(7);
            outputFile2<<fixed<<r.x<<" "<<r.y<<" "<<r.z<<"\n";
        }
        outputFile2<<"\n";
    }

    inputFile.close();
    outputFile2.close();
    inputFile.open("stage2.txt");

    ///Projection Transform

    matrix P = projectionTransform();
    while(1){
        point p[3],r;
        int flag = 0;
        for(int i =0 ; i<3 ; i++){
            p[i].x = inputPoint();
            if(p[i].x == 9999.0){
                flag = 1;
                break;
            }
            p[i].y = inputPoint();
            p[i].z = inputPoint();
        }
        if(flag == 1) break;

        for(int i =0 ; i<3 ; i++){
            r = transformPoint(P,p[i]);
            cout.precision(7);
            outputFile3<<fixed<<r.x<<" "<<r.y<<" "<<r.z<<"\n";
        }
        outputFile3<<"\n";
    }

	return 0;
}

