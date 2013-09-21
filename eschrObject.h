#ifndef ESCHROBJECT_H
#define ESCHROBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#endif

#include "eschr.h"

/*
// the eschr-class
// pos		coords of the object
// colorSet	int 0-2
//		0 "reddish"
//		1 "greenish"
//		2 "blueish"
*/

class eschr
{
	GLfloat pos[3];
	GLfloat colors[22][4];
	GLfloat * colorTable[19][12];
	int colorSet;
	
	// "R+B = 3+5" 
	GLfloat targetColor1[4];
	// "R+G = 1+5"
	GLfloat targetColor2[4];
	// "G+B = 1+3"
	GLfloat targetColor3[4];

	public:
		eschr(GLfloat, GLfloat, GLfloat, int);
		~eschr();
		void draw();
		void printColorTable();
	private:
		void drawPlane();
		void drawObject();
		void initColors(GLfloat colorSeed[]);
		void colorTransform();
		
};

#endif // ESCHROBJECT_H
