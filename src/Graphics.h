#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <GL/glut.h>
#include <GL/GLAux.h>
#include <stdio.h>
#include <math.h>

using namespace std;

/*AUX_RGBImageRec * loadBMP(char *Filename)                    // Loads A Bitmap Image
{
	FILE *File=NULL;                            // File Handle

	// Make Sure A Filename Was Given
	if (!Filename)
		return NULL;

	File = fopen(Filename,"r");                       // Check To See If The File Exists

	// Does The File Exist?
	if (File){
		fclose(File);
		// Load The Bitmap And Return A Pointer
		return auxDIBImageLoad(Filename);
	}else{
		cout << "No encontrado " << endl;
	}

	return NULL;                                // If Load Failed Return NULL
}


int loadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]= loadBMP("Data/Crate.bmp"))
	{
		//cout << "Cargar texturas" << endl;

		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(3, &texture[0]);					// Create Three Textures

		// Create Nearest Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		// Create Linear Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}

	if (TextureImage[0])								// If Texture Exists
	{

		if (TextureImage[0]->data)						// If Texture Image Exists
		{
			free(TextureImage[0]->data);// Free The Texture Image Memory will not compile in dev
		}

		free(TextureImage[0]);							// Free The Image Structure
	}

	return Status;										// Return The Status
}*/

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
/*void InitGL()	        // We call this right after our OpenGL window is created.
{
	// Jump To Texture Loading Routine
	//if (!loadGLTextures()){
	//	cout << "Textura no cargada " << endl;			// Texture Didn't Load
	//}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping

	// Iluminacion:
	//    * GL_FLAT (Color plano)
	//    * GL_SMOOTH (Interpolacion de color entre los 3 vertices)
	//    * PHONG (Interpolacion de normales de los 3 vertices y posterior calculo de color)
	// Ademas, definir normal para cada vertice / triangulo
	glShadeModel(GL_SMOOTH);

	// Material:

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Luces
	//glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	//glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	//glEnable(GL_LIGHT1);


	// Culling
	//glCullFace(GL_FRONT);
	//glEnable(GL_CULL_FACE);
}*/

#endif /* GRAPHICS_H_ */
