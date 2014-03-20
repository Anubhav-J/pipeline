#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class Matrix
{
	public:
		float mat[16];
		Matrix()	// Constructor
		{
			int i;
			for(i=0;i<16;i++)
			{
					if(i%5==0)
						mat[i]=1;
					else
						mat[i]=0;
			}
		}		
};

// ----==== MODELLING ====----//
void sglInit();
void sglRotate(float,float,float,float);
void sglTranslate(float,float,float);
void sglScale(float,float,float);
void sglMatrix(float []);
void sglLoadIdentity();
void sglPushMatrix();
void sglPopMatrix();


// ----==== VIEWING ====---- //

void sglViewRotate(float,float,float,float);
void sglViewTranslate(float,float,float);
void sglLookAt(float,float,float,float,float,float,float,float,float);
void sglViewMatrix(float []);
void sglViewLoadIdentity();

// ----==== PROJECTION ====---- //

void sglProjOrtho(float,float,float,float,float,float);
void sglProjFrustum(float,float,float,float,float,float);


// ----==== VIEWPORT ====---- //

void sglViewport(float,float,float,float);

// ----==== PRIMITIVES ====---- //

void sglBegin();
void sglEnd();
void sglColour(float,float,float);
void sglClear(float,float,float);
void sglVertex(float,float,float);
void sglShow();


float* multiply(float a[],float b[]);
