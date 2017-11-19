#include "Robot.h"
#define PI 3.14159265
///Copyright (C) <2017>  <Eliseo Rivera> curso.tareas@gmail.com
Robot::Robot()
{
   theta1=0;theta2=0;theta3=0; theta4=0; theta5=0; theta6=0,theta7=0;
   THx.identity(4);
   THy.identity(4);
   THz.identity(4);
int d=4;
  TH.identity(4);
    THbase.identity(4);
  d1={0,0,2.5};
  d2={0,0,6};
  d3={12,0,6};
  d4={12,0,1};
  d5={0,0,6};
  d6={0,0,6};
  d7={0,0,0.5};
    //ctor
}

Robot::~Robot()
{

delete base;
delete b1;
delete b2;
delete b3;
delete b4;
delete b5;
delete b6;
delete gripe;
    //dtor
}

void Robot::inicializar(){
base=new modelo3D();
b1=new modelo3D();
b2=new modelo3D();
b3=new modelo3D();
b4=new modelo3D();
b5=new modelo3D();
b6=new modelo3D();
gripe=new modelo3D();


base->leer("base.STL");
b1->leer("b1.STL");
b2->leer("b2.STL");
b3->leer("b3.STL");
b4->leer("b4.STL");
b5->leer("b2.STL");
b6->leer("b2.STL");
gripe->leer("gripe.STL");

b1->definirRx(-PI/2.0);
b1->rotarX();

b2->definirRx(-PI/2.0);
b2->rotarX();
b3->definirRz(-PI/2);
b3->rotarZ();
b4->definirRz(-PI/2);
b4->rotarZ();


b5->definirRz(-3.14159265);
b5->rotarZ();

b6->definirRz(-3.14159265);
b6->rotarZ();




/*
gripe->definirRx(-3.14159265/2);
gripe->rotarX();
*/
modelos.push_back(base);
modelos.push_back(b1);

modelos.push_back(b2);

modelos.push_back(b3);

modelos.push_back(b4);

modelos.push_back(b5);
modelos.push_back(b6);

modelos.push_back(gripe);

}
void Robot::configurarTH(){
AplicarTHz(0,{0,0,0}); //base
THList.push_back(THz);
AplicarTHx(0,{0,0,0}); //base
THList.push_back(THx);



AplicarTHz(0,{0,0,0}); //b1
THList.push_back(THz);
AplicarTHx(-90,d1); //b1
THList.push_back(THx);



AplicarTHz(0,{0,0,0}); //b2
THList.push_back(THz);
AplicarTHx(-90,d2); //b2
THList.push_back(THx);


AplicarTHz(0,{0,0,0}); //b3
THList.push_back(THz);
AplicarTHx(0,d3); //b3
THList.push_back(THx);


AplicarTHz(0,{0,0,0}); //b4
THList.push_back(THz);
AplicarTHz(0,d4); //b4
THList.push_back(THz);

AplicarTHz(0,{0,0,0}); //b5
THList.push_back(THz);
AplicarTHx(-90,d5); //b5
THList.push_back(THx);


AplicarTHz(0,{0,0,0}); //b6
THList.push_back(THz);
AplicarTHx(-90,d6); //b6
THList.push_back(THx);



AplicarTHz(0,{0,0,0}); //gripe
THList.push_back(THz);
AplicarTHx(0,d7); //gripe
THList.push_back(THx);

}
void Robot::renderizar(){


TH.resetIdentity();
THbase.resetIdentity();
modelo3D *model;

for (int m=0;m<modelos.size();m++){
model=modelos[m];

 TH=TH*THList[2*m+0]*THList[2*m+1];


vector3d ux,uy,uz,O;
ux={1,0,0};
uy={0,1,0};
uz={0,0,1};

Matrix ux4(ux,1),uy4(uy,1),uz4(uz,1),O4(O,1);


ux4=TH*ux4-TH*O4;
uy4=TH*uy4-TH*O4;
uz4=TH*uz4-TH*O4;
O4=TH*O4;


ux={ux4.aij[0][0],ux4.aij[1][0],ux4.aij[2][0]};
uy={uy4.aij[0][0],uy4.aij[1][0],uy4.aij[2][0]};
uz={uz4.aij[0][0],uz4.aij[1][0],uz4.aij[2][0]};
O={O4.aij[0][0],O4.aij[1][0],O4.aij[2][0]};


         Drawarrow3D(O,O+4*ux,{1,0.1,0.2},0.03,0.1);
         Drawarrow3D(O,O+4*uy,{.1,1,0.2},0.03,0.1);
         Drawarrow3D(O,O+4*uz,{0.1,0.2,1},0.03,0.1);
         glColor4f(fabs(cos(m*PI/modelos.size())),fabs(sin(20*(m-5)*PI/modelos.size())),0.2,0.5);
 // if (m==1){
glEnable(GL_BLEND);
 glBegin(GL_TRIANGLES);


  glFrontFace(GL_FRONT_AND_BACK);
    for (int i=0;i<model->ntriangles;i++){

vector3d v1=model->triangulos[i].vertices[0];   //posiciones locales
vector3d v2=model->triangulos[i].vertices[1];
vector3d v3=model->triangulos[i].vertices[2];
Matrix v14(v1,1),v24(v2,1),v34(v3,1);

v14=TH*v14;
v24=TH*v24;
v34=TH*v34;
v1={v14.entry(0,0),v14.entry(1,0),v14.entry(2,0)};
v2={v24.entry(0,0),v24.entry(1,0),v24.entry(2,0)};
v3={v34.entry(0,0),v34.entry(1,0),v34.entry(2,0)};



Matrix N(4,1),d14(4,1),d24(4,1);
d14=v24-v14;
d24=v34-v14;
vector3d d1,d2,n;
d1={d14.entry(0,0),d14.entry(1,0),d14.entry(2,0)};
d2={d24.entry(0,0),d24.entry(1,0),d24.entry(2,0)};
n=d1*d2;  ///devuelve el producto vectorial
n.normalize();



        glNormal3f(n.x,n.y,n.z);
        glVertex3f(v1.x,v1.y,v1.z);
        glVertex3f(v2.x,v2.y,v2.z);
        glVertex3f(v3.x,v3.y,v3.z);
    }
glEnd();

 glDisable(GL_BLEND);


///DIBUJAR EJES






}


}




void Robot::DefinirTHx(float dtheta, vector3d d){


THx.aij[0][0]=1;
THx.aij[0][1]=0;
THx.aij[0][2]=0;

THx.aij[1][0]=0;
THx.aij[1][1]=cos(dtheta);
THx.aij[1][2]=-sin(dtheta);

THx.aij[2][0]=0;
THx.aij[2][1]=sin(dtheta);
THx.aij[2][2]=cos(dtheta);

THx.aij[3][0]=0;
THx.aij[3][1]=0;
THx.aij[3][2]=0;
THx.aij[3][3]=1;

THx.aij[0][3]=d.x;
THx.aij[1][3]=d.y;
THx.aij[2][3]=d.z;
}
void Robot::DefinirTHy(float dtheta, vector3d d){


THy.aij[0][0]=cos(dtheta);
THy.aij[0][1]=0;
THy.aij[0][2]=sin(dtheta);

THy.aij[1][0]=0;
THy.aij[1][1]=1;
THy.aij[1][2]=0;

THy.aij[2][0]=-sin(dtheta);
THy.aij[2][1]=0;
THy.aij[2][2]=cos(dtheta);

THy.aij[3][0]=0;
THy.aij[3][1]=0;
THy.aij[3][2]=0;
THy.aij[3][3]=1;

THy.aij[0][3]=d.x;
THy.aij[1][3]=d.y;
THy.aij[2][3]=d.z;
}
void Robot::DefinirTHz(float dtheta, vector3d d){

THz.aij[0][0]=cos(dtheta);
THz.aij[0][1]=-sin(dtheta);
THz.aij[0][2]=0;

THz.aij[1][0]=sin(dtheta);
THz.aij[1][1]=cos(dtheta);
THz.aij[1][2]=0;

THz.aij[2][0]=0;
THz.aij[2][1]=0;
THz.aij[2][2]=1;

THz.aij[3][0]=0;
THz.aij[3][1]=0;
THz.aij[3][2]=0;
THz.aij[3][3]=1;

THz.aij[0][3]=d.x;
THz.aij[1][3]=d.y;
THz.aij[2][3]=d.z;
}

void Robot::AplicarTHx(float theta, vector3d d){
theta=theta*PI/180.0;

DefinirTHx(theta,d);

}
void Robot::AplicarTHy(float theta, vector3d d){
theta=theta*PI/180.0;
DefinirTHy(theta,d);

}
void Robot::AplicarTHz(float theta, vector3d d){
theta=theta*PI/180.0;
DefinirTHz(theta,d);

}
void Robot:: ConfigurarOrigenes(){

    /*
modelo3D *modelo;
Matrix TH;
TH.identity(4);
for (int m=0;m<modelos.size();m++){
modelo=modelos[m];
TH=TH*THList[m];

vector3d ux,uy,uz,O;
ux=modelo->ux;
uy=modelo->uy;
uz=modelo->uz;
O=modelo->O;

Matrix ux4(ux,1),uy4(uy,1),uz4(uz,1),O4(O,1);

ux4=TH*ux4-TH*O4;
uy4=TH*uy4-TH*O4;
uz4=TH*uz4-TH*O4;
O4=TH*O4;
modelo->ux={ux4.aij[0][0],ux4.aij[1][0],ux4.aij[2][0]};
modelo->uy={uy4.aij[0][0],uy4.aij[1][0],uy4.aij[2][0]};
modelo->uz={uz4.aij[0][0],uz4.aij[1][0],uz4.aij[2][0]};
modelo->O={O4.aij[0][0],O4.aij[1][0],O4.aij[2][0]};


}*/
}

void Robot::Drawarrow3D( vector3d A,  vector3d B, vector3d color, double cota1,double R)
{

double color1,color2,color3,a,b,c,d,e;



color1=color.x;//abs(color1/255);
color2=color.y;//abs(color2/255);
color3=color.z;//abs(color3/255);

glColor3f( color1,color2, color3);

vector3d n=B-A,np,vertex[10],normallight;
n.normalize();
if(n.z!=0)np={1,1,(-1/n.z)*(n.x+n.y)};
else if(n.y!=0)np={1,(-1/n.y)*(n.x+n.z),1};
else np={(-1/n.x)*(n.y+n.z),1,1};

np.normalize();
vertex[0]=R*np;
vertex[2]=R*(n*np).normalize();
vertex[1]=R*((0.5)*(vertex[2]-vertex[0])+vertex[0]).normalize();
vertex[4]=R*(n*vertex[2]).normalize();
vertex[3]=R*((0.5)*(vertex[4]-vertex[2])+vertex[2]).normalize();
vertex[6]=R*(n*vertex[4]).normalize();
vertex[5]=R*((0.5)*(vertex[6]-vertex[4])+vertex[4]).normalize();
vertex[7]=R*((0.5)*(vertex[0]-vertex[6])+vertex[6]).normalize();
vertex[8]=vertex[0];
vertex[9]=vertex[1];
int nx=8;
double d_thetha,fraccion=0.1,radioflecha=R+.7*R;
d_thetha=2.0f*PI/nx;


  ///tubos
 glBegin( GL_TRIANGLE_STRIP );

         for(int i=0;i<9;i++)
               {

normallight=n*(vertex[i-1]-vertex[i+1]);
normallight.normalize();
glNormal3f(normallight.x, normallight.y, normallight.z);
                 glVertex3f(vertex[i].x+A.x,vertex[i].y+A.y,vertex[i].z+A.z);

glVertex3f(vertex[i].x+B.x-fraccion*(B.x-A.x),vertex[i].y+B.y-fraccion*(B.y-A.y),vertex[i].z+B.z-fraccion*(B.z-A.z));

    // top face

                }

glEnd();



//flecha
vertex[0]=radioflecha*np;
vertex[2]=radioflecha*(n*np).normalize();
vertex[1]=radioflecha*((0.5)*(vertex[2]-vertex[0])+vertex[0]).normalize();
vertex[4]=radioflecha*(n*vertex[2]).normalize();
vertex[3]=radioflecha*((0.5)*(vertex[4]-vertex[2])+vertex[2]).normalize();
vertex[6]=radioflecha*(n*vertex[4]).normalize();
vertex[5]=radioflecha*((0.5)*(vertex[6]-vertex[4])+vertex[4]).normalize();
vertex[7]=radioflecha*((0.5)*(vertex[0]-vertex[6])+vertex[6]).normalize();
vertex[8]=vertex[0];
vertex[9]=vertex[1];
vector3d Ap(B-fraccion*(B-A));



 glBegin( GL_TRIANGLE_STRIP );  //flecha

         for(int i=0;i<9;i++)
               {

normallight=n*(vertex[i-1]-vertex[i+1]);
normallight.normalize();
glNormal3f(normallight.x, normallight.y, normallight.z);
                 glVertex3f(vertex[i].x+Ap.x,vertex[i].y+Ap.y,vertex[i].z+Ap.z);


glNormal3f(n.x, n.y, n.z);
glVertex3f(Ap.x+fraccion*(B-A).x,Ap.y+fraccion*(B-A).y,Ap.z+fraccion*(B-A).z);

    // top face

                }

glEnd();


}
