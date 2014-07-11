/*
 * OneViewer.h
 *
 *  Created on: 04/07/2014
 *      Author: Hernan
 */

#ifndef ONEVIEWER_H_
#define ONEVIEWER_H_

#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/GLAux.h>
#include <time.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

/* ascii code for the escape key */
#define ESCAPE 27

using namespace std;

class OneViewer {
public:
	OneViewer();
	virtual ~OneViewer();

	void SetupWorld();
	AUX_RGBImageRec * LoadBMP(char *Filename);
	int LoadGLTextures();
	void InitGL(int Width, int Height);
	void ReSizeGLScene(int Width, int Height);
	void DrawGLScene();
	void keyPressed(unsigned char key, int x, int y);

private:
	/* The number of our GLUT window */
	int window;
	int mirror;
};

#endif /* ONEVIEWER_H_ */
