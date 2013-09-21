#include "eschr.h"
#include "eschrObject.h"


// cube parts
enum {
	cubeFront= 1, cubeFrontTop=   2, cubeFrontBottom=   4,
	cubeRight= 8, cubeRightTop=  16, cubeRightBottom=  32,
	cubeTop=  64, cubeTopLeft=  128, cubeTopRight=    256
};


// constructor
eschr::eschr(GLfloat a, GLfloat b, GLfloat c, int cS)
{
	pos[0]=a;
	pos[1]=b;
	pos[2]=c;
	colorSet = cS;

	GLfloat *colorSeed;
	
	// switch base colors
	switch(colorSet){
		case 0: // R
			static GLfloat colorSeed0[] = {
				0.0,1.0,0.0,1.0, //1
				1.0,1.0,1.0,1.0, //2
				0.0,0.0,1.0,1.0,
				1.0,1.0,1.0,1.0, //4
				1.0,0.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //6
				0.0,0.0,0.0,1.0,
				0.0,1.0,0.0,1.0, //8
				0.0,1.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //10
				1.0,1.0,1.0,1.0,
				0.0,0.0,1.0,1.0, //
				0.0,0.0,1.0,1.0,
				1.0,1.0,1.0,1.0, //
				1.0,1.0,1.0,1.0,
				1.0,0.0,0.0,1.0, //16
				1.0,0.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //
				1.0,1.0,1.0,1.0,
				0.0,0.0,0.0,1.0, //20
				0.0,0.0,0.0,1.0,
				0.0,0.0,0.0,1.0
			};
			colorSeed = colorSeed0;
		break;
		case 1: // G
			static GLfloat colorSeed1[] = {
				0.0,0.0,1.0,1.0, //1
				1.0,1.0,1.0,1.0, //2
				1.0,0.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //4
				0.0,1.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //6
				0.0,0.0,0.0,1.0,
				0.0,0.0,1.0,1.0, //8
				0.0,0.0,1.0,1.0,
				1.0,1.0,1.0,1.0, //10
				1.0,1.0,1.0,1.0,
				1.0,0.0,0.0,1.0, //
				1.0,0.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //
				1.0,1.0,1.0,1.0,
				0.0,1.0,0.0,1.0, //16
				0.0,1.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //
				1.0,1.0,1.0,1.0,
				0.0,0.0,0.0,1.0, //20
				0.0,0.0,0.0,1.0,
				0.0,0.0,0.0,1.0
			};
			colorSeed = colorSeed1;
		break;
		case 2: // B
			static GLfloat colorSeed2[] = {
				1.0,0.0,0.0,1.0, //1
				1.0,1.0,1.0,1.0, //2
				0.0,1.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //4
				0.0,0.0,1.0,1.0,
				1.0,1.0,1.0,1.0, //6
				0.0,0.0,0.0,1.0,
				1.0,0.0,0.0,1.0, //8
				1.0,0.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //10
				1.0,1.0,1.0,1.0,
				0.0,1.0,0.0,1.0, //
				0.0,1.0,0.0,1.0,
				1.0,1.0,1.0,1.0, //
				1.0,1.0,1.0,1.0,
				0.0,0.0,1.0,1.0, //16
				0.0,0.0,1.0,1.0,
				1.0,1.0,1.0,1.0, //
				1.0,1.0,1.0,1.0,
				0.0,0.0,0.0,1.0, //20
				0.0,0.0,0.0,1.0,
				0.0,0.0,0.0,1.0
			};
			colorSeed = colorSeed2;
		break;
	}

	initColors(colorSeed);
	
	// calculate targetColors
	for(int i=0;i<3;i++){
		targetColor1[i] = colors[2][i] + colors[4][i];
		targetColor2[i] = colors[0][i] + colors[4][i];
		targetColor3[i] = colors[0][i] + colors[2][i];
	}
	// set alpha values by hand
	targetColor1[3]=1.0;
	targetColor2[3]=1.0;
	targetColor3[3]=1.0;

	
// cube 1
	colorTable[0][0]=colors[15];
	colorTable[0][1]=colors[15];
	colorTable[0][2]=colors[15];
	colorTable[0][3]=colors[21];
	colorTable[0][4]=colors[20];
	colorTable[0][5]=colors[20];
	colorTable[0][6]=colors[20];
	colorTable[0][7]=colors[18];
	colorTable[0][8]=colors[17];
	colorTable[0][9]=colors[17];
	colorTable[0][10]=colors[17];
	colorTable[0][11]=colors[16];
// cube 2
	colorTable[1][0]=colors[15];
	colorTable[1][1]=colors[21];
	colorTable[1][2]=colors[21];
	colorTable[1][3]=colors[21];
	colorTable[1][4]=colors[21];
	colorTable[1][5]=colors[21];
	colorTable[1][6]=colors[21];
	colorTable[1][7]=colors[21];
	colorTable[1][8]=colors[21];
	colorTable[1][9]=colors[21];
	colorTable[1][10]=colors[21];
	colorTable[1][11]=colors[21];
// cube 3
	colorTable[2][0]=colors[21];
	colorTable[2][1]=colors[21];
	colorTable[2][2]=colors[21];
	colorTable[2][3]=colors[21];
	colorTable[2][4]=colors[20];
	colorTable[2][5]=colors[20];
	colorTable[2][6]=colors[20];
	colorTable[2][7]=colors[20];
	colorTable[2][8]=colors[19];
	colorTable[2][9]=colors[19];
	colorTable[2][10]=colors[19];
	colorTable[2][11]=colors[19];
// cube 4
	colorTable[3][0]=colors[21];
	colorTable[3][1]=colors[21];
	colorTable[3][2]=colors[21];
	colorTable[3][3]=colors[21];
	colorTable[3][4]=colors[20];
	colorTable[3][5]=colors[20];
	colorTable[3][6]=colors[20];
	colorTable[3][7]=colors[20];
	colorTable[3][8]=colors[7];
	colorTable[3][9]=colors[19];
	colorTable[3][10]=colors[19];
	colorTable[3][11]=colors[19];
// cube 5
	colorTable[4][0]=colors[21];
	colorTable[4][1]=colors[21];
	colorTable[4][2]=colors[21];
	colorTable[4][3]=colors[10];
	colorTable[4][4]=colors[9];
	colorTable[4][5]=colors[9];
	colorTable[4][6]=colors[9];
	colorTable[4][7]=colors[8];
	colorTable[4][8]=colors[7];
	colorTable[4][9]=colors[7];
	colorTable[4][10]=colors[7];
	colorTable[4][11]=colors[19];
// cube 6
	colorTable[5][0]=colors[21];
	colorTable[5][1]=colors[21];
	colorTable[5][2]=colors[21];
	colorTable[5][3]=colors[21];
	colorTable[5][4]=colors[11];
	colorTable[5][5]=colors[20];
	colorTable[5][6]=colors[20];
	colorTable[5][7]=colors[20];
	colorTable[5][8]=colors[19];
	colorTable[5][9]=colors[19];
	colorTable[5][10]=colors[19];
	colorTable[5][11]=colors[19];
// cube 7
	colorTable[6][0]=colors[13];
	colorTable[6][1]=colors[13];
	colorTable[6][2]=colors[13];
	colorTable[6][3]=colors[12];
	colorTable[6][4]=colors[11];
	colorTable[6][5]=colors[11];
	colorTable[6][6]=colors[11];
	colorTable[6][7]=colors[20];
	colorTable[6][8]=colors[19];
	colorTable[6][9]=colors[19];
	colorTable[6][10]=colors[19];
	colorTable[6][11]=colors[14];
// cube 8
	colorTable[7][0]=colors[1];
	colorTable[7][1]=colors[1];
	colorTable[7][2]=colors[1];
	colorTable[7][3]=colors[1];
	colorTable[7][4]=colors[1];
	colorTable[7][5]=colors[1];
	colorTable[7][6]=colors[1];
	colorTable[7][7]=colors[1];
	colorTable[7][8]=colors[5];
	colorTable[7][9]=colors[4];
	colorTable[7][10]=colors[4];
	colorTable[7][11]=colors[4];
// cube 9
	colorTable[8][0]=colors[1];
	colorTable[8][1]=colors[1];
	colorTable[8][2]=colors[1];
	colorTable[8][3]=colors[1];
	colorTable[8][4]=colors[4];
	colorTable[8][5]=colors[4];
	colorTable[8][6]=colors[4];
	colorTable[8][7]=colors[4];
	colorTable[8][8]=colors[4];
	colorTable[8][9]=colors[4];
	colorTable[8][10]=colors[4];
	colorTable[8][11]=colors[4];
// cube 10
	colorTable[9][0]=colors[1];
	colorTable[9][1]=colors[1];
	colorTable[9][2]=colors[1];
	colorTable[9][3]=colors[1];
	colorTable[9][4]=colors[1];
	colorTable[9][5]=colors[1];
	colorTable[9][6]=colors[4];
	colorTable[9][7]=colors[4];
	colorTable[9][8]=colors[4];
	colorTable[9][9]=colors[4];
	colorTable[9][10]=colors[4];
	colorTable[9][11]=colors[4];
// cube 11
	colorTable[10][0]=colors[1];
	colorTable[10][1]=colors[1];
	colorTable[10][2]=colors[1];
	colorTable[10][3]=colors[1];
	colorTable[10][4]=colors[1];
	colorTable[10][5]=colors[1];
	colorTable[10][6]=colors[4];
	colorTable[10][7]=colors[4];
	colorTable[10][8]=colors[1];
	colorTable[10][9]=colors[1];
	colorTable[10][10]=colors[1];
	colorTable[10][11]=colors[1];
// cube 12
	colorTable[11][0]=colors[1];
	colorTable[11][1]=colors[1];
	colorTable[11][2]=colors[1];
	colorTable[11][3]=colors[1];
	colorTable[11][4]=colors[1];
	colorTable[11][5]=colors[1];
	colorTable[11][6]=colors[1];
	colorTable[11][7]=colors[1];
	colorTable[11][8]=colors[6];
	colorTable[11][9]=colors[3];
	colorTable[11][10]=colors[3];
	colorTable[11][11]=colors[3];
// cube 13
	colorTable[12][0]=colors[1];
	colorTable[12][1]=colors[1];
	colorTable[12][2]=colors[1];
	colorTable[12][3]=colors[1];
	colorTable[12][4]=colors[3];
	colorTable[12][5]=colors[3];
	colorTable[12][6]=colors[3];
	colorTable[12][7]=colors[3];
	colorTable[12][8]=colors[3];
	colorTable[12][9]=colors[3];
	colorTable[12][10]=colors[3];
	colorTable[12][11]=colors[3];
// cube 14
	colorTable[13][0]=colors[1];
	colorTable[13][1]=colors[1];
	colorTable[13][2]=colors[1];
	colorTable[13][3]=colors[1];
	colorTable[13][4]=colors[1];
	colorTable[13][5]=colors[1];
	colorTable[13][6]=colors[3];
	colorTable[13][7]=colors[3];
	colorTable[13][8]=colors[3];
	colorTable[13][9]=colors[3];
	colorTable[13][10]=colors[3];
	colorTable[13][11]=colors[3];
// cube 15
	colorTable[14][0]=colors[1];
	colorTable[14][1]=colors[1];
	colorTable[14][2]=colors[1];
	colorTable[14][3]=colors[1];
	colorTable[14][4]=colors[1];
	colorTable[14][5]=colors[1];
	colorTable[14][6]=colors[3];
	colorTable[14][7]=colors[3];
	colorTable[14][8]=colors[1];
	colorTable[14][9]=colors[1];
	colorTable[14][10]=colors[1];
	colorTable[14][11]=colors[1];
// cube 16
	colorTable[15][0]=colors[1];
	colorTable[15][1]=colors[1];
	colorTable[15][2]=colors[1];
	colorTable[15][3]=colors[1];
	colorTable[15][4]=colors[6];
	colorTable[15][5]=colors[5];
	colorTable[15][6]=colors[1];
	colorTable[15][7]=colors[1];
	colorTable[15][8]=colors[1];
	colorTable[15][9]=colors[1];
	colorTable[15][10]=colors[1];
	colorTable[15][11]=colors[1];
// cube 17
	colorTable[16][0]=colors[6];
	colorTable[16][1]=colors[1];
	colorTable[16][2]=colors[1];
	colorTable[16][3]=colors[1];
	colorTable[16][4]=colors[1];
	colorTable[16][5]=colors[1];
	colorTable[16][6]=colors[1];
	colorTable[16][7]=colors[1];
	colorTable[16][8]=colors[1];
	colorTable[16][9]=colors[1];
	colorTable[16][10]=colors[1];
	colorTable[16][11]=colors[1];
// cube 18
	colorTable[17][0]=colors[1];
	colorTable[17][1]=colors[1];
	colorTable[17][2]=colors[1];
	colorTable[17][3]=colors[1];
	colorTable[17][4]=colors[1];
	colorTable[17][5]=colors[0];
	colorTable[17][6]=colors[1];
	colorTable[17][7]=colors[1];
	colorTable[17][8]=colors[1];
	colorTable[17][9]=colors[1];
	colorTable[17][10]=colors[1];
	colorTable[17][11]=colors[1];
// cube 19
	colorTable[18][0]=colors[3];
	colorTable[18][1]=colors[2];
	colorTable[18][2]=colors[1];
	colorTable[18][3]=colors[1];
	colorTable[18][4]=colors[1];
	colorTable[18][5]=colors[1];
	colorTable[18][6]=colors[1];
	colorTable[18][7]=colors[1];
	colorTable[18][8]=colors[1];
	colorTable[18][9]=colors[1];
	colorTable[18][10]=colors[1];
	colorTable[18][11]=colors[1];
}

// destructor
eschr::~eschr(){}

void eschr::initColors(GLfloat colorSeed[]){
	for(int i=0; i<22; i++){
		colors[i][0]=colorSeed[i*4];
		colors[i][1]=colorSeed[i*4+1];
		colors[i][2]=colorSeed[i*4+2];
		colors[i][3]=colorSeed[i*4+3];
	}
}
	
void eschr::printColorTable(){
	for(int i=8;i<12;i++){
		printf("---------\n");
		printArray(colorTable[4][i],4);
		printf("\n");
	}
//abort();	
}
	
void eschr::colorTransform()
{
	if(eschrColorUp || eschrColorDown){
		// calculate new colorFactor
		if (eschrColorUp && eschrColorFactor < 100){
			eschrColorFactor +=0.1;
		}else if(eschrColorDown && eschrColorFactor > 0.5){
			eschrColorFactor -=0.1;
		}

		// calculate new colors
		for(int i=0; i<5; i++){
			colors[ colorGroup1[i] ][0] = colors[ colorGroup1Base[i] ][0] + (targetColor1[0]-colors[ colorGroup1Base[i] ][0])*eschrColorFactor/100;
			colors[ colorGroup1[i] ][1] = colors[ colorGroup1Base[i] ][1] + (targetColor1[1]-colors[ colorGroup1Base[i] ][1])*eschrColorFactor/100;
			colors[ colorGroup1[i] ][2] = colors[ colorGroup1Base[i] ][2] + (targetColor1[2]-colors[ colorGroup1Base[i] ][2])*eschrColorFactor/100;

			colors[ colorGroup2[i] ][0] = colors[ colorGroup2Base[i] ][0] + (targetColor2[0]-colors[ colorGroup2Base[i] ][0])*eschrColorFactor/100;
			colors[ colorGroup2[i] ][1] = colors[ colorGroup2Base[i] ][1] + (targetColor2[1]-colors[ colorGroup2Base[i] ][1])*eschrColorFactor/100;
			colors[ colorGroup2[i] ][2] = colors[ colorGroup2Base[i] ][2] + (targetColor2[2]-colors[ colorGroup2Base[i] ][2])*eschrColorFactor/100;

			colors[ colorGroup3[i] ][0] = colors[ colorGroup3Base[i] ][0] + (targetColor3[0]-colors[ colorGroup3Base[i] ][0])*eschrColorFactor/100;
			colors[ colorGroup3[i] ][1] = colors[ colorGroup3Base[i] ][1] + (targetColor3[1]-colors[ colorGroup3Base[i] ][1])*eschrColorFactor/100;
			colors[ colorGroup3[i] ][2] = colors[ colorGroup3Base[i] ][2] + (targetColor3[2]-colors[ colorGroup3Base[i] ][2])*eschrColorFactor/100;
		}
	}
}

// main drawing function
void eschr::draw()
{
	glPushMatrix();
	glTranslatef(pos[0],pos[1],pos[2]);
	
	colorTransform();
	
	drawPlane();
	
	// rotate
	glRotatef(eschrRotateX,1,0,0);
	glRotatef(eschrRotateY,0,1,0);
	glRotatef(eschrRotateZ,0,0,1);
	drawObject();
	
	glPopMatrix();
}

// draws pieces for plane between the objects
void eschr::drawPlane()
{
	drawCubeParts(-4,2,0,cubeTop,colorTable[7],1,false);

	drawCubeParts(-4,2,2,cubeTop|cubeRight,colorTable[8],1,false);
	drawCubeParts(-4,2,4,cubeTop|cubeRight|cubeRightTop,colorTable[9],1,false);
	drawCubeParts(-4,0,2,cubeRight|cubeRightTop,colorTable[10],1,false);

	drawCubeParts(-2,-2,2,cubeTop,colorTable[11],1,false);

	drawCubeParts(-2,-2,4,cubeRight|cubeTop,colorTable[12],1,false);
	drawCubeParts(-2,-2,6,cubeTop|cubeRight|cubeRightTop,colorTable[13],1,false);
	drawCubeParts(-2,-4,4,cubeRight|cubeRightTop,colorTable[14],1,false);

	drawCubeParts(-2,2,-2,cubeRight|cubeRightBottom,colorTable[15],1,false);
	drawCubeParts(2,-2,-2,cubeFront|cubeFrontTop,colorTable[16],1,false);
	drawCubeParts(2,0,-4,cubeRight|cubeRightBottom,colorTable[17],1,false);
	drawCubeParts(0,-4,2,cubeFront|cubeFrontTop,colorTable[18],1,false);

}

// draws the Object
void eschr::drawObject()
{
    	drawCubeParts(0,0,0,cubeTop|cubeRight|cubeFront,colorTable[2],1,true);

     	drawCubeParts(-2,2,0,cubeTop|cubeRight|cubeFront,colorTable[0],1,true);
  	drawCubeParts(-2,0,0,cubeFront,colorTable[1],1,true);

 	drawCubeParts(2,0,-2,cubeTop|cubeRight|cubeFront,colorTable[4],1,true);
 	drawCubeParts(0,0,-2,cubeTop,colorTable[3],1,true);

 	drawCubeParts(0,-2,2,cubeTop|cubeRight|cubeFront,colorTable[6],1,true);
 	drawCubeParts(0,-2,0,cubeRight,colorTable[5],1,true);
}
