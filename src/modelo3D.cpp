#include "modelo3D.h"
#include <gl/gl.h>
#include <stdint.h>

//Copyright (C) <2017>  <Eliseo Rivera Silva> curso.tareas@gmail.com


modelo3D::modelo3D()
{
    ntriangles=0;
    ux={1,0,0};uy={0,1,0};uz={0,0,1};  //local and global axis are coincident

    BFRx.identity(3);
    BFRy.identity(3);
    BFRz.identity(3);
    LARx.identity(3);
    LARy.identity(3);
    LARz.identity(3);
    R.identity(3);  //accumulated rotations
    Rz.identity(3);
    Rx.identity(3);
    Ry.identity(3);
    ///***************
  //IRx.identity(3);
    //IRy.identity(3);
    //IRz.identity(3);
    ///***************
    O={0,0,0};
}

modelo3D::~modelo3D()
{
 delete triangulos;

}
modelo3D::modelo3D(int ntriangulos){
triangulos=new Triangle3D[ntriangulos];
ntriangles=ntriangulos;
}
void modelo3D::leer(string nombre){

char head[80] = "";
///************
	archivo.open(nombre.c_str(), std::ios::in | std::ios::binary);
	if(archivo){
	archivo.read(head, 80);

	int32_t size ;
	archivo.read(reinterpret_cast<char *> (&size), sizeof(int32_t));
	ntriangles=size;
	triangulos=new Triangle3D[size];
	Triangle3D triangle;
	vector3d P0, P1, P2;
	vector3d normal;
	float p0[3], p1[3], p2[3], n[3];
	char attribute[2] = "0";
for (int i=0;i<size;i++){

		archivo.read(reinterpret_cast<char *> (&n[0]), 4);
		archivo.read(reinterpret_cast<char *> (&n[1]), 4);
		archivo.read(reinterpret_cast<char *> (&n[2]), 4);
    //  cout<<n[0]<<" , "<<n[1]<<" , "<<n[2]<<endl;
       triangulos[i].N={n[0],n[1],n[2]};
		archivo.read(reinterpret_cast<char *> (&p0[0]), 4);
		archivo.read(reinterpret_cast<char *> (&p0[1]), 4);
		archivo.read(reinterpret_cast<char *> (&p0[2]), 4);
	//	   cout<<p0[0]<<" , "<<p0[1]<<" , "<<p0[2]<<endl;
          triangulos[i].vertices[0]={p0[0],p0[1],p0[2]};


		archivo.read(reinterpret_cast<char *> (&p1[0]), 4);
		archivo.read(reinterpret_cast<char *> (&p1[1]), 4);
		archivo.read(reinterpret_cast<char *> (&p1[2]), 4);
	//	   cout<<p1[0]<<" , "<<p1[1]<<" , "<<p1[2]<<endl;
          triangulos[i].vertices[1]={p1[0],p1[1],p1[2]};

		archivo.read(reinterpret_cast<char *> (&p2[0]), 4);
		archivo.read(reinterpret_cast<char *> (&p2[1]), 4);
		archivo.read(reinterpret_cast<char *> (&p2[2]), 4);
	//	   cout<<p2[0]<<" , "<<p2[1]<<" , "<<p2[2]<<endl;

        triangulos[i].vertices[2]={p2[0],p2[1],p2[2]};
	    archivo.read(attribute, 2);
	}


	archivo.close();
}
else{
        ntriangles=0;
        cout<<"el archivo no se encuentra"<<endl;
}
}
void modelo3D::definirRx(float dtheta){


Rx.aij[0][0]=1;
Rx.aij[0][1]=0;
Rx.aij[0][2]=0;

Rx.aij[1][0]=0;
Rx.aij[1][1]=cos(dtheta);
Rx.aij[1][2]=-sin(dtheta);

Rx.aij[2][0]=0;
Rx.aij[2][1]=sin(dtheta);
Rx.aij[2][2]=cos(dtheta);

}
void modelo3D::definirRy(float dtheta){


Ry.aij[0][0]=cos(dtheta);
Ry.aij[0][1]=0;
Ry.aij[0][2]=sin(dtheta);

Ry.aij[1][0]=0;
Ry.aij[1][1]=1;
Ry.aij[1][2]=0;

Ry.aij[2][0]=-sin(dtheta);
Ry.aij[2][1]=0;
Ry.aij[2][2]=cos(dtheta);



}
void modelo3D::definirRz(float dtheta){

Rz.aij[0][0]=cos(dtheta);
Rz.aij[0][1]=-sin(dtheta);
Rz.aij[0][2]=0;

Rz.aij[1][0]=sin(dtheta);
Rz.aij[1][1]=cos(dtheta);
Rz.aij[1][2]=0;

Rz.aij[2][0]=0;
Rz.aij[2][1]=0;
Rz.aij[2][2]=1;

}

void modelo3D::dibujar(){
    glBegin(GL_TRIANGLES);
    glFrontFace(GL_FRONT_AND_BACK);

    for (int i=0;i<ntriangles;i++){

vector3d v1=triangulos[i].vertices[0];   //locales son locales
vector3d v2=triangulos[i].vertices[1];
vector3d v3=triangulos[i].vertices[2];
vector3d V1,V2, V3;
V1=v1.x*ux+v1.y*uy+v1.z*uz;  // se dibuja en base local en origen de modelo local
V2=v2.x*ux+v2.y*uy+v2.z*uz;
V3=v3.x*ux+v3.y*uy+v3.z*uz;

V1=O+V1; V2=O+V2; V3=O+V3;

vector3d d1,d2,n;
d1=V2-V1;
d2=V3-V1;
n=d1*d2;  ///devuelve el producto vectorial
n.normalize();

glNormal3f(n.x,n.y,n.z);
        glVertex3f(V1.x,V1.y,V1.z);
        glVertex3f(V2.x,V2.y,V2.z);
        glVertex3f(V3.x,V3.y,V3.z);
    }

glEnd();
};

void modelo3D::rotarZ(){  //global rotation of only model
    for(int i=0;i<ntriangles;i++){
        vector3d v1,v2,v3;
        v1=triangulos[i].vertices[0];
        v2=triangulos[i].vertices[1];
        v3=triangulos[i].vertices[2];
v1=Rz*v1;
v2=Rz*v2;
v3=Rz*v3;

        triangulos[i].vertices[0]=v1;
        triangulos[i].vertices[1]=v2;
        triangulos[i].vertices[2]=v3;
    }
    R=Rz*R;
}
void modelo3D::rotarY(){ //global rotation  of only model
    for(int i=0;i<ntriangles;i++){
        vector3d v1,v2,v3;
        v1=triangulos[i].vertices[0];
        v2=triangulos[i].vertices[1];
        v3=triangulos[i].vertices[2];
v1=Ry*v1;
v2=Ry*v2;
v3=Ry*v3;

        triangulos[i].vertices[0]=v1;
        triangulos[i].vertices[1]=v2;
        triangulos[i].vertices[2]=v3;
    }
    R=Ry*R;
}
void modelo3D::rotarX(){//global rotation  of only model
    for(int i=0;i<ntriangles;i++){
        vector3d v1,v2,v3;
        v1=triangulos[i].vertices[0];
        v2=triangulos[i].vertices[1];
        v3=triangulos[i].vertices[2];
v1=Rx*v1;
v2=Rx*v2;
v3=Rx*v3;

        triangulos[i].vertices[0]=v1;
        triangulos[i].vertices[1]=v2;
        triangulos[i].vertices[2]=v3;
    }
    R=Rx*R;
}

void modelo3D::trasladar(vector3d A){  //local traslation
 for(int i=0;i<ntriangles;i++){
        vector3d v1,v2,v3;
        v1=triangulos[i].vertices[0];
        v2=triangulos[i].vertices[1];
        v3=triangulos[i].vertices[2];


        triangulos[i].vertices[0]=v1+A;
        triangulos[i].vertices[1]=v2+A;
        triangulos[i].vertices[2]=v3+A;
    }


}

void modelo3D::BodyFramedefinirRx(float dtheta){


BFRx.aij[0][0]=1;
BFRx.aij[0][1]=0;
BFRx.aij[0][2]=0;

BFRx.aij[1][0]=0;
BFRx.aij[1][1]=cos(dtheta);
BFRx.aij[1][2]=-sin(dtheta);

BFRx.aij[2][0]=0;
BFRx.aij[2][1]=sin(dtheta);
BFRx.aij[2][2]=cos(dtheta);


}
void modelo3D::BodyFramedefinirRy(float dtheta){

BFRy.aij[0][0]=cos(dtheta);
BFRy.aij[0][1]=0;
BFRy.aij[0][2]=sin(dtheta);

BFRy.aij[1][0]=0;
BFRy.aij[1][1]=1;
BFRy.aij[1][2]=0;

BFRy.aij[2][0]=-sin(dtheta);
BFRy.aij[2][1]=0;
BFRy.aij[2][2]=cos(dtheta);


}
void modelo3D::BodyFramedefinirRz(float dtheta){

BFRz.aij[0][0]=cos(dtheta);
BFRz.aij[0][1]=-sin(dtheta);
BFRz.aij[0][2]=0;

BFRz.aij[1][0]=sin(dtheta);
BFRz.aij[1][1]=cos(dtheta);
BFRz.aij[1][2]=0;

BFRz.aij[2][0]=0;
BFRz.aij[2][1]=0;
BFRz.aij[2][2]=1;

}

void modelo3D::definir_x_LocalAxisRotation(float dtheta){
LARx.aij[0][0]=1;
LARx.aij[0][1]=0;
LARx.aij[0][2]=0;

LARx.aij[1][0]=0;
LARx.aij[1][1]=cos(dtheta);
LARx.aij[1][2]=sin(dtheta);

LARx.aij[2][0]=0;
LARx.aij[2][1]=-sin(dtheta);
LARx.aij[2][2]=cos(dtheta);
}
void modelo3D::definir_y_LocalAxisRotation(float dtheta){


LARy.aij[0][0]=cos(dtheta);
LARy.aij[0][1]=0;
LARy.aij[0][2]=sin(dtheta);

LARy.aij[1][0]=0;
LARy.aij[1][1]=1;
LARy.aij[1][2]=0;

LARy.aij[2][0]=-sin(dtheta);
LARy.aij[2][1]=0;
LARy.aij[2][2]=cos(dtheta);



}
void modelo3D::definir_z_LocalAxisRotation(float dtheta){

LARz.aij[0][0]=cos(dtheta);
LARz.aij[0][1]=-sin(dtheta);
LARz.aij[0][2]=0;

LARz.aij[1][0]=sin(dtheta);
LARz.aij[1][1]=cos(dtheta);
LARz.aij[1][2]=0;

LARz.aij[2][0]=0;
LARz.aij[2][1]=0;
LARz.aij[2][2]=1;

}


void  modelo3D::x_LocalAxisRotation(){
R=LARx*R;
vector3d Lx(1,0,0),Ly(0,1,0),Lz(0,0,1);///definir vectores
Lx=LARx*Lx;
Ly=LARx*Ly;
Lz=LARx*Lz;
ux=Lx.x*ux+Lx.y*uy+Lx.z*uz;
uy=Ly.x*ux+Ly.y*uy+Ly.z*uz;
uz=Lz.x*ux+Lz.y*uy+Lz.z*uz;
}
void  modelo3D::y_LocalAxisRotation(){
R=LARy*R;
vector3d Lx(1,0,0),Ly(0,1,0),Lz(0,0,1);///definir vectores
Lx=LARy*Lx;
Ly=LARy*Ly;
Lz=LARy*Lz;
ux=Lx.x*ux+Lx.y*uy+Lx.z*uz;
uy=Ly.x*ux+Ly.y*uy+Ly.z*uz;
uz=Lz.x*ux+Lz.y*uy+Lz.z*uz;
}
void  modelo3D::z_LocalAxisRotation(){
R=LARz*R;
vector3d Lx(1,0,0),Ly(0,1,0),Lz(0,0,1);///definir vectores
Lx=LARz*Lx;
Ly=LARz*Ly;
Lz=LARz*Lz;
ux=Lx.x*ux+Lx.y*uy+Lx.z*uz;
uy=Ly.x*ux+Ly.y*uy+Ly.z*uz;
uz=Lz.x*ux+Lz.y*uy+Lz.z*uz;
}

void modelo3D::BodyFramerotarX(){
R=BFRx*R;
ux=BFRx*ux;
uy=BFRx*uy;
uz=BFRx*uz;

};
void modelo3D::BodyFramerotarY(){
R=BFRy*R;
ux=BFRy*ux;
uy=BFRy*uy;
uz=BFRy*uz;
};
void modelo3D::BodyFramerotarZ(){
R=BFRz*R;
ux=BFRz*ux;
uy=BFRz*uy;
uz=BFRz*uz;
};
/*
void modelo3D::IRx(){
    Matrix::inversa(IRx());

}*/

void modelo3D::BodyFrametrasladar(vector3d A){
O=A;

};
vector3d modelo3D::LocalMassCenter()const
{
 vector3d center;
 for(int i=0;i<ntriangles;i++){
        vector3d v1,v2,v3;
        v1=triangulos[i].vertices[0];
        v2=triangulos[i].vertices[1];
        v3=triangulos[i].vertices[2];
center=center+(1.0/3.0)*(v1+v2+v3);

    }
    center=(1.0/ntriangles)*center;
    cout<<" Local center mass is :"<<endl;
    center.mostrar();
    return center;

}
vector3d modelo3D::GlobalCenterMass()const{
vector3d L=LocalMassCenter();



        vector3d C=O+L.x*ux+L.y*uy+L.z*uz;
    cout<<" Global center mass is: "<<endl;
        C.mostrar();
        return C;

}
