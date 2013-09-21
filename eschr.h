#ifndef ESCHR_H
#define ESCHR_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#endif

void printArray(GLfloat *myArray, int mySize);
GLvoid calcNV(	GLfloat vertA[], GLfloat vertB[], GLfloat vertC[], GLfloat *fNormalX, GLfloat *fNormalY, GLfloat *fNormalZ);
GLvoid drawCubePlane (GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat v4[3]);
GLvoid drawCubeTriPlanePart(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat vx1[3], GLfloat color1[4], GLfloat color2[4] );
GLvoid drawCubeParts (GLfloat posX, GLfloat posY, GLfloat posZ, int parts, GLfloat **colors, GLfloat length, bool cube);

extern bool eschrColorUp;
extern bool eschrColorDown;
extern GLfloat eschrColorFactor;
extern GLfloat eschrRotateX;
extern GLfloat eschrRotateY;
extern GLfloat eschrRotateZ;

extern int colorGroup1[5];
extern int colorGroup1Base[5];

extern int colorGroup2[5];
extern int colorGroup2Base[5];

extern int colorGroup3[5];
extern int colorGroup3Base[5];

#endif // ESCHR_H
