#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <SDL.h>

#include <math.h>
#include <stdio.h>

#include "eschr.h"
#include "eschrObject.h"

using namespace std;

// tickvars for improvised framerate
// f_rame c_ount
unsigned int fcStart,fcEnd;
unsigned int frameCounter = 0;


// ###########
// INIT VALUES
// ###########

// math const
const float SQRT_3 = sqrt(3.0f);

// RESOLUTION
//	int width = 1400, height = 1050;
	int width = 900, height = 900;

// textures
//Texture * cubeTexture;

// switches
bool changePerspective = false;
bool viewPersp = true;
bool viewOrtho = false;


bool cubeStretchAdd = false;
bool cubeStretchSub = false;
bool cubeStretchReset = false;

bool moveLeft = false;
bool moveRight = false;
bool resetCamera = false;

bool eschrColorUp  = false;
bool eschrColorDown = false;
bool eschrColorReset = false;

// eschr movementswitches
bool eschrMoveUp = false;
bool eschrMoveDown = false;
bool eschrMoveReset = false;

bool eschrRotateXpos = false;
bool eschrRotateXneg = false;
bool eschrRotateYpos = false;
bool eschrRotateYneg = false;
bool eschrRotateZpos = false;
bool eschrRotateZneg = false;

bool eschrRotateReset = false;

// values
GLfloat eschrMoveFactor = 0;
GLfloat eschrColorFactor = 0;
GLfloat cubeStretchFactor = 1;
GLfloat eschrRotateX = 0;
GLfloat eschrRotateY = 0;
GLfloat eschrRotateZ = 0;

GLfloat cameraSpeed = 0.5;
GLfloat dX = 0, dY = 0, dZ = 0;

// colorGroups moved from eschr::colorTransform
int colorGroup1[5] = {7,14,16,17,19};
int colorGroup1Base[5] = {0,3,4,5,6};

int colorGroup2[5] = {8,9,11,18,20};
int colorGroup2Base[5] = {0,1,2,5,6};

int colorGroup3[5] = {10,12,13,15,21};
int colorGroup3Base[5] = {1,2,3,4,6};
	


// cube parts
enum {
	cubeFront= 1, cubeFrontTop=   2, cubeFrontBottom=   4,
	cubeRight= 8, cubeRightTop=  16, cubeRightBottom=  32,
	cubeTop=  64, cubeTopLeft=  128, cubeTopRight=    256
};

void printArray(GLfloat *myArray, int mySize){
	for(int i=0; i<mySize;i++){
		printf("%f,",myArray[i]);
	}
}

GLvoid calcNV(	GLfloat vertA[], GLfloat vertB[], GLfloat vertC[], GLfloat *fNormalX, GLfloat *fNormalY, GLfloat *fNormalZ)
{
	GLfloat aX,aY,aZ,bX,bY,bZ;
	
	// calculate vector a=B-A
	aX = vertB[0]-vertA[0];
	aY = vertB[1]-vertA[1];
	aZ = vertB[2]-vertA[2];

	// calculate vector b=C-A
	bX = vertC[0]-vertA[0];
	bY = vertC[1]-vertA[1];
	bZ = vertC[2]-vertA[2];
	
	// calculate kreuzprodukt
	// a x b = 
	*fNormalX = aY*bZ - aZ*bY;
	*fNormalY = aZ*bX - aX*bZ;
	*fNormalZ = aX*bY - aY*bX;
}

GLvoid drawCubePlane (GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat v4[3])
{
	// define normal vectors
	GLfloat normX,normY,normZ;
	
	calcNV(v1,v2,v3,&normX,&normY,&normZ);
	glNormal3f(normX, normY, normZ);
	
	glBegin(GL_QUADS);
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
		glVertex3fv(v4);
	glEnd();
}

GLvoid drawCubeTriPlanePart(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat vx1[3], GLfloat color1[4], GLfloat color2[4] )
{
	// define normal vectors
	GLfloat normX, normY, normZ;
	
	calcNV(v1,v2,v3,&normX,&normY,&normZ);
	glNormal3f(normX, normY, normZ);
	
	glBegin(GL_TRIANGLES);

//	glColor4fv(color1);
	glMaterialfv(GL_FRONT,GL_SPECULAR,color1);
	glMaterialfv(GL_FRONT,GL_AMBIENT,color1);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,color1);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color1);
	glMaterialf(GL_FRONT,GL_SHININESS,50.0f);

	glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(vx1);
	
//	glColor4fv(color2);
	glMaterialfv(GL_FRONT,GL_SPECULAR,color2);
	glMaterialfv(GL_FRONT,GL_AMBIENT,color2);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,color2);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color2);
	glMaterialf(GL_FRONT,GL_SHININESS,50.0f);

	glVertex3fv(v1);
	glVertex3fv(vx1);
	glVertex3fv(v3);

	glEnd();
}


/*
// colors contains colorinfo for the 12 triangles, 4 per side
// order: front, right, top
front:
	ll,lr,rl,rr
right:
	bb,bt,tb,tt
top:
	rr,rl,lr,ll
*/
GLvoid drawCubeParts (GLfloat posX, GLfloat posY, GLfloat posZ, int parts, GLfloat **colors, GLfloat length=1, bool cube=true)
{
	// define vertices
	GLfloat len = length;
	
	// define normal-vector for plane
//GLfloat nv[]={1,1,1};

	// vertices for cube
	GLfloat v[14][3] = {
		{ posX-len, posY-len, posZ+len }, // v[0] == v1
		{ posX+len, posY-len, posZ+len }, // v[1] == v2
		{ posX+len, posY+len, posZ+len },
		{ posX-len, posY+len, posZ+len },
		{ posX+len, posY-len, posZ-len },
		{ posX-len, posY-len, posZ-len },
		{ posX-len, posY+len, posZ-len },
		{ posX+len, posY+len, posZ-len }, //  v[7] == v8
		{ posX+0,   posY-len, posZ+len }, //  v[8] == vx1
		{ posX+0,   posY+len, posZ+len }, //  v[9] == vx2
		{ posX-len, posY+len, posZ+0   }, //  v[10] == vx3
		{ posX+len, posY+len, posZ+0   }, //  v[11] == vx4
		{ posX+len, posY+0,   posZ-len }, //  v[12] == vx5
		{ posX+len, posY+0,   posZ+len }  //  v[13] == vx6
	};
	
	// transform cube
	if(cube == true){
		if(cubeStretchFactor>0){
			GLfloat dist;
			for(int i=0; i<14; i++){
				
				//calculate distance from plane
				dist = (v[i][0]+v[i][1]+v[i][2])/SQRT_3;

				v[i][0]=v[i][0]-dist*cubeStretchFactor*(1.0f/SQRT_3);
				v[i][1]=v[i][1]-dist*cubeStretchFactor*(1.0f/SQRT_3);
				v[i][2]=v[i][2]-dist*cubeStretchFactor*(1.0f/SQRT_3);
			}
		}
			for(int i=0; i<14; i++){
				v[i][0]=v[i][0]+eschrMoveFactor;
				v[i][1]=v[i][1]+eschrMoveFactor;
				v[i][2]=v[i][2]+eschrMoveFactor;
			}
	} else if(cube == false){
		GLfloat dist;
		for(int i=0; i<14; i++){
			
			//calculate distance from plane
			dist = (v[i][0]+v[i][1]+v[i][2])/(SQRT_3);

			v[i][0]=v[i][0]-dist*1*(1.0f/SQRT_3);
			v[i][1]=v[i][1]-dist*1*(1.0f/SQRT_3);
			v[i][2]=v[i][2]-dist*1*(1.0f/SQRT_3);
		}
		
	}
	
	
	// save matrix
	glPushMatrix();
	
	// begin drawing
	
	if(cube == true){
		// back
		//glColor3f(0.5,0.5,0.0);
		GLfloat backColor[]={0.5,0.5,0.0,1.0};
		glMaterialfv(GL_FRONT,GL_SPECULAR,backColor);
		glMaterialfv(GL_FRONT,GL_AMBIENT,backColor);
		glMaterialfv(GL_FRONT,GL_DIFFUSE,backColor);
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,backColor);

		drawCubePlane(v[4],v[5],v[6],v[7]);

		// left
		drawCubePlane(v[5],v[0],v[3],v[6]);

		// bottom
		drawCubePlane(v[0],v[5],v[4],v[1]);
	} else { // cube is false, so we need back side of plane-components
		glDisable(GL_CULL_FACE);
	}
	
	
	//front
	if (parts&cubeFront){
		if(       parts&cubeFrontTop ){
			drawCubeTriPlanePart(v[0],v[2],v[3],v[9],colors[1],colors[0]);
		}else if( parts&cubeFrontBottom ){
			drawCubeTriPlanePart(v[2],v[0],v[1],v[8],colors[2],colors[3]);
		}else{
			drawCubeTriPlanePart(v[0],v[2],v[3],v[9],colors[1],colors[0]);
			drawCubeTriPlanePart(v[2],v[0],v[1],v[8],colors[2],colors[3]);
		}
	}
	
	//right
	if (parts&cubeRight){
		if(       parts&cubeRightTop){
			drawCubeTriPlanePart(v[2],v[4],v[7],v[12],colors[6],colors[7]);
		}else if( parts&cubeRightBottom){
			drawCubeTriPlanePart(v[4],v[2],v[1],v[13],colors[5],colors[4]);
		}else{
			drawCubeTriPlanePart(v[4],v[2],v[1],v[13],colors[5],colors[4]);
			drawCubeTriPlanePart(v[2],v[4],v[7],v[12],colors[6],colors[7]);
		}
	
	}
	
	//top
	if(parts&cubeTop){
		if(       parts&cubeTopLeft){
			drawCubeTriPlanePart(v[2],v[6],v[3],v[10],colors[10],colors[11]);
		}else if( parts&cubeTopRight){
			drawCubeTriPlanePart(v[6],v[2],v[7],v[11],colors[8],colors[9]);
		}else{
			drawCubeTriPlanePart(v[6],v[2],v[7],v[11],colors[9],colors[8]);
			drawCubeTriPlanePart(v[2],v[6],v[3],v[10],colors[10],colors[11]);
		}
	}
	
	if(cube==false){
		glEnable(GL_CULL_FACE);
	}
	// restore matrix
	glPopMatrix();	
	
}




/*
// the eschr-plane
// draws the eschr-pieces
*/
///*
class eschrPlane
{
//	eschr *elements[3][3];
	eschr *elements[13];
	public:
		eschrPlane();
		~eschrPlane();
		void draw();
	private:
};

eschrPlane::eschrPlane()
{
//	int i, j;
//	for (i=0; i<10; i++)
//	for (j=0; j<10; j++)
//		elements[i][j] = new eschr(0, 0, 0, 0);
	
	elements[0]=new eschr(-12,6,6,0);
	elements[1]=new eschr(-6,6,0,1);
	elements[2]=new eschr(0,6,-6,2);
	elements[3]=new eschr(6,6,-12,0);
	
	elements[4]=new eschr(-12,0,12,1);
	
	elements[5]=new eschr(-6,0,6,2);
	elements[6]=new eschr(0,0,0,0);
	elements[7]=new eschr(6,0,-6,1);
	
	elements[8]=new eschr(12,0,-12,2);
	
	elements[9]=new eschr(-6,-6,12,0);
	elements[10]=new eschr(0,-6,6,1);
	elements[11]=new eschr(6,-6,0,2);
	elements[12]=new eschr(12,-6,-6,0);

}

eschrPlane::~eschrPlane()
{
/*	int i, j;
	for (i=0; i<10; i++)
	for (j=0; j<10; j++)
		delete elements[i][j];
*/
	for(int i=0; i<13; i++)
		delete elements[i];
}

void eschrPlane::draw()
{
/*	int i, j;
	for (i=0; i<10; i++)
	for (j=0; j<10; j++)
		elements[i][j]->draw();
*/
	for(int i=0; i<13;i++)
		elements[i]->draw();
}

/*void setPerspective(){ // false normal, if true ortho
	changePerspective = false;
	printf("foo\n");

	if(viewOrtho){
printf("  ortho\n");
		glOrtho(-10,10, -10,10,0,20);
		viewOrtho = false;
		viewPersp = true;
	}
	else if(viewPersp){
printf("  persp\n");
		gluPerspective(90.0, (float)width/(float)height, 0.1, 2000.0);
		viewPersp = false;
		viewOrtho = true;
	}
}*/

void setPerspective(){ // false normal, if true ortho
	gluPerspective(90.0, (float)width/(float)height, 0.1, 50.0);
}

void myinit(int width, int height) 
{
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	glEnable(GL_DITHER);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);  
	
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE); // not needed since my normals are unit lenght

// BEGIN INIT LIGHT
///*

	GLfloat light0Position[] = {20,20,20,1.0};
	GLfloat light0SpotDirection[] = {0,0,0};
	GLfloat light0Color[]=	{1.0,1.0,1.0,1.0};
	
	glLightfv (GL_LIGHT0, GL_POSITION, light0Position);
	glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, light0SpotDirection);
	glLightfv(GL_LIGHT0, GL_SPECULAR,light0Color);

	//GLfloat ambientColor[3] ={0.5,0.5,0.5};
	//GLfloat diffuseColor[3] ={0.5,0.5,0.5};
	//GLfloat specularColor[3]={0.5,0.5,0.5};


	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);
//*/	
// END INIT LIGHT
	
//	cubeTexture = new Texture("textures/test_256.png");
//	cubeTexture = new Texture("textures/psychoRaster_1024.png");
//	cubeTexture->load();

//	setPerspective();
//glDisable(GL_LIGHTING);
	gluPerspective(90.0, (float)width/(float)height, 0.1, 100.0);


}

GLvoid getMovement(){
	if (moveRight) dX -= cameraSpeed;
	if (moveLeft) dX += cameraSpeed;
}

void mydisplay(eschrPlane &myPlane)
{
	if(changePerspective){
		glLoadIdentity();
		setPerspective();
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	if (resetCamera){
		dX = 0;
		resetCamera = false;
	}
	
	getMovement();
/*	gluLookAt(5+dX, 5+dY, 5+dZ,  // eye
            0, 0, 0, // center
            0.0, 1.0, 0.0); // up
*/
	gluLookAt(15, 15, 15,  // eye
            0, 0, 0, // center
            0.0, 1.0, 0.0); // up
	
	glRotatef(dX,0,1,0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

// ### wireframe
//	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	if (cubeStretchAdd && cubeStretchFactor < 1){
		cubeStretchFactor += 0.01;
	}else if(cubeStretchSub && cubeStretchFactor > 0){
		cubeStretchFactor -= 0.01;
	}else if(cubeStretchReset){
		cubeStretchFactor=1;
		cubeStretchReset = false;
	}

	if(eschrMoveUp){
		eschrMoveFactor +=0.1;
	}else if(eschrMoveDown){
		eschrMoveFactor -=0.1;
	}else if(eschrMoveReset){
		eschrMoveFactor = 0;
		eschrMoveReset = false;
	}
	
	if(eschrRotateXpos){
		eschrRotateX += 0.5;
	}else if(eschrRotateXneg){
		eschrRotateX -= 0.5;
	}
	if(eschrRotateYpos){
		eschrRotateY += 0.5;
	}else if(eschrRotateYneg){
		eschrRotateY -= 0.5;
	}
	if(eschrRotateZpos){
		eschrRotateZ += 0.5;
	}else if(eschrRotateZneg){
		eschrRotateZ -= 0.5;
	}
	if(eschrRotateReset){
		eschrRotateX = 0;
		eschrRotateY = 0;
		eschrRotateZ = 0;
		
		eschrRotateReset = false;
	}
	
	myPlane.draw();
/*	
// draw projection planes
	
	GLfloat calcY;
	
// 2nd plane:	x+y+z = 0
// I		y=-x-z
	
	
	glColor3f(1.0,0,0);

	for(float i=-10; i<=10; i+=0.5){
		glBegin(GL_LINES);
			calcY = (-i-10.0f);
			glVertex3f(i,calcY,10);
			calcY = (-i+10.0f);
			glVertex3f(i,calcY,-10);
		glEnd();
		
		glBegin(GL_LINES);
			calcY = (-10.0f-i);
			glVertex3f(10,calcY,i);
			calcY = (10.0f-i);
			glVertex3f(-10,calcY,i);
		glEnd();
	}
*/
/*	
// axes
	glColor3f(0,0,1);
	glBegin(GL_LINES);
		glVertex3f( -10,0,0);
		glVertex3f( 10,0,0);
		glVertex3f(  0,-10,0);
		glVertex3f(  0,10,0);
		glVertex3f(  0,0,-10);
		glVertex3f(  0,0,10);
	glEnd();
*/
	

	frameCounter++;
	
	glPopMatrix();
	SDL_GL_SwapBuffers();
}

int main(int argc, char ** argv){

	// Initialize SDL

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}

//	if (!SDL_SetVideoMode(width, height, 32, SDL_OPENGL|SDL_FULLSCREEN)) {
	if (!SDL_SetVideoMode(width, height, 32, SDL_OPENGL)) {
		fprintf(stderr, "Unable set SDL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	
	SDL_WM_SetCaption(":: eschr 2.0 beta ::", NULL);  // window title
	myinit(width, height);  // initialize OpenGL
	
	eschrPlane myPlane;
	
	// main application loop
	bool done = false;
	while (!done) {
		mydisplay(myPlane);
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) done = true;
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						done = true;
					break;
					case SDLK_p:
						changePerspective = true;
					break;
					case SDLK_q:
						cubeStretchAdd = true;
					break;
					case SDLK_a:
						cubeStretchSub = true;
					break;
					case SDLK_y:
						cubeStretchReset = true;
					break;
					case SDLK_RIGHT:
						moveRight = true;
					break;
					case SDLK_LEFT:
						moveLeft = true;
					break;
					case SDLK_r:
						resetCamera = true;
					break;
					
					
					case SDLK_w:
						eschrMoveUp = true;
					break;
					case SDLK_s:
						eschrMoveDown = true;
					break;
					case SDLK_x:
						eschrMoveReset = true;
					break;
					case SDLK_e:
						eschrColorUp = true;
					break;
					case SDLK_d:
						eschrColorDown = true;
					break;
					case SDLK_c:
						eschrColorReset = true;
					break;
					
					
					case SDLK_i:
						eschrRotateXpos = true;
					break;
					case SDLK_k:
						eschrRotateXneg = true;
					break;
					case SDLK_l:
						eschrRotateYpos = true;
					break;
					case SDLK_j:
						eschrRotateYneg = true;
					break;
					case SDLK_o:
						eschrRotateZpos = true;
					break;
					case SDLK_u:
						eschrRotateZneg = true;
					break;
					case SDLK_m:
						eschrRotateReset = true;
					break;
					default:
						break;
				}
			}else if(event.type == SDL_KEYUP){
				switch (event.key.keysym.sym){
					case SDLK_q:
						cubeStretchAdd = false;
					break;
					case SDLK_a:
						cubeStretchSub = false;
					break;
					case SDLK_RIGHT:
						moveRight = false;
					break;
					case SDLK_LEFT:
						moveLeft = false;
					break;
					case SDLK_w:
						eschrMoveUp = false;
					break;
					case SDLK_s:
						eschrMoveDown = false;
					break;
					case SDLK_e:
						eschrColorUp = false;
					break;
					case SDLK_d:
						eschrColorDown = false;
					break;
					
					case SDLK_i:
						eschrRotateXpos = false;
					break;
					case SDLK_k:
						eschrRotateXneg = false;
					break;
					case SDLK_l:
						eschrRotateYpos = false;
					break;
					case SDLK_j:
						eschrRotateYneg = false;
					break;
					case SDLK_o:
						eschrRotateZpos = false;
					break;
					case SDLK_u:
						eschrRotateZneg = false;
					break;
					default:
						break;
					
				}
			}
		}
	}

	// get endticks
	fcEnd = SDL_GetTicks();
	printf("%i fps av\n",frameCounter*1000/(fcEnd-fcStart) );
	
	SDL_Quit();
	return 0;
}
