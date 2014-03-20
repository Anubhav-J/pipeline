#include "sgl.h"
#include <math.h>
#include <stdlib.h>
#include <stack>
#include <vector>
#include<GL/glut.h>

using namespace std;

#define PI 3.141592653589

Matrix View;
Matrix Proj;
Matrix Trans;
Matrix Viewport;

int flag=0;
float ColorR=0,ColorG=0,ColorB=0;
float vportx=0,vporty=0;
float width=2,height=2;
float vportW,vportH;

stack <Matrix> m_stack;
vector<float>vertices;
vector<float>colors;

void sglTranslate(float tx,float ty,float tz)	
{
	Matrix temp;
	int i;
	for(i=0;i<16;i++)
		temp.mat[i]=m_stack.top().mat[i];

	for(i=0;i<4;i++)
	{
		m_stack.top().mat[i]= temp.mat[i];
		m_stack.top().mat[i+4]=temp.mat[i+4];
		m_stack.top().mat[i+8]=temp.mat[i+8];
		m_stack.top().mat[i+12]=temp.mat[i]*tx + temp.mat[i+4]*ty + temp.mat[i+8]*tz + temp.mat[i+12];
	}

}

void sglRotate(float angle,float x,float y,float z)
{
	float c = cos(angle * PI / 180);
	float s = sin(angle * PI / 180);
	Matrix temp;
	int i;
	for(i=0;i<16;i++)
		temp.mat[i]=m_stack.top().mat[i];

	for(i=0;i<4;i++)	
	{
		m_stack.top().mat[i] = temp.mat[i]*((x*x)*(1-c)+c) + temp.mat[i+4]*(x*y*(1-c)+z*s) + temp.mat[i+8]*(x*z*(1-c) - y*s);
	        m_stack.top().mat[i+4] = temp.mat[i]*(x*y*(1-c) - z*s) + temp.mat[i+4]*((y*y)*(1-c)+c)+temp.mat[i+8]*(y*y*(1-c) + x*s);
		m_stack.top().mat[i+8] = temp.mat[i]*(x*z*(1-c) +y*s) + temp.mat[i+4]*(y*z*(1-c)-x*s) + temp.mat[i+8]*(z*z*(1-c) +c);	
	}
	
}

void sglScale(float sx, float sy, float sz)
{
	Matrix temp;
	int i;
	for(i=0;i<16;i++)
		temp.mat[i]=m_stack.top().mat[i];
	
	for(i=0;i<4;i++)
	{
		m_stack.top().mat[i]=temp.mat[i]*sx;
		m_stack.top().mat[i+4]=temp.mat[i+4]*sy;
		m_stack.top().mat[i+8]=temp.mat[i+8]*sz;
		m_stack.top().mat[i+12]=temp.mat[i+12];
	}

}

void sglPushMatrix()
{
	Matrix temp;
	int i;
	for(i=0;i<16;i++)
		temp.mat[i]=m_stack.top().mat[i];

	m_stack.push(temp);
}

void sglPopMatrix()
{
	if(!m_stack.empty())
	m_stack.pop();
}

void sglLoadIdentity()
{
	Matrix Model;
	int i;
	for(i=0;i<16;i++)
	{
		if(i%5==0)
			Model.mat[i]=1;
		else
			Model.mat[i]=0;
	}
	for(i=0;i<16;i++)
		m_stack.top().mat[i]=Model.mat[i];
}

void sglInit()
{
	while(!m_stack.empty())
		m_stack.pop();

	Matrix temp;
	m_stack.push(temp);
	vertices.clear();
	colors.clear();
}

void sglMatrix(float mat[16])
{
	Matrix t;
	int i;
	for(i=0;i<16;i++)
		t.mat[i]=mat[i];
	for(i=0;i<16;i++)
		m_stack.top().mat[i]=mat[i];
	
}
// < ------------------====================  VIEWING FUNCTIONS ===================------------------ >

void sglViewTranslate(float tx,float ty,float tz)
{
	Matrix temp;
	int i;
	for(i=0;i<16;i++)
		temp.mat[i]=View.mat[i];
	for(i=0;i<4;i++)
	{
		View.mat[i]= temp.mat[i];
		View.mat[i+4]=temp.mat[i+4];
		View.mat[i+8]=temp.mat[i+8];
		View.mat[i+12]=temp.mat[i]*tx + temp.mat[i+4]*ty + temp.mat[i+8]*tz + temp.mat[i+12];
	}	
}
void sglViewRotate(float angle,float x,float y,float z)
{
	float c = cos(angle * PI / 180);
	float s = sin(angle * PI / 180);
	Matrix temp;
	int i;
	for(i=0;i<16;i++)
		temp.mat[i]= View.mat[i];

	for(i=0;i<4;i++)	
	{
		View.mat[i] = temp.mat[i]*((x*x)*(1-c)+c) + temp.mat[i+4]*(x*y*(1-c)+z*s) + temp.mat[i+8]*(x*z*(1-c) - y*s);
	        View.mat[i+4] = temp.mat[i]*(x*y*(1-c) - z*s) + temp.mat[i+4]*((y*y)*(1-c)+c)+temp.mat[i+8]*(y*y*(1-c) + x*s);
		View.mat[i+8] = temp.mat[i]*(x*z*(1-c) +y*s) + temp.mat[i+4]*(y*z*(1-c)-x*s) + temp.mat[i+8]*(z*z*(1-c) +c);	
	}

}
void sglViewLoadIdentity()
{
	int i;
	for(i=0;i<16;i++)
	{
		if(i%5==0)
			View.mat[i]=1;
		else
			View.mat[i]=0;
	}
}

void sglViewMatrix(float mat[16])
{
	int i;
	for(i=0;i<16;i++)
		View.mat[i]=mat[i];
}

void sglLookAt(float cx,float cy,float cz,float lx, float ly, float lz, float upx,float upy, float upz)
{
	Matrix temp;
	float F[3];
	F[0]=lx-cx;
	F[1]=ly-cy;
	F[2]=lz-cz;

	float up[3];
	up[0]=upx;
	up[1]=upy;
	up[2]=upz;

	float d1=sqrt(F[0]*F[0] + F[1]*F[1] + F[2]*F[2]);
	float d2=sqrt(up[0]*up[0] + up[1]*up[1] + up[2]*up[2]);

	F[0]=F[0]/d1;
	F[1]=F[1]/d1;
	F[2]=F[2]/d1;

	up[0]=up[0]/d2;
	up[1]=up[1]/d2;
	up[2]=up[2]/d2;

	float s[3],u[3];

	s[0]=F[1]*up[2] - up[1]*F[2];
	s[1]=F[2]*up[0] - up[2]*F[0];
	s[2]=F[0]*up[1] - up[0]*F[1];

	d1=sqrt(s[0]*s[0] + s[1]*s[1] + s[2]*s[2]);
	s[0]=s[0]/d1;
	s[1]=s[1]/d1;
	s[2]=s[2]/d1;

	u[0]=s[1]*F[2] - F[1]*s[2];
	u[1]=s[2]*F[0] - F[2]*s[0];
	u[2]=s[0]*F[1] - F[0]*s[1];
	d1=sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
	u[0]=u[0]/d1;
	u[1]=u[1]/d1;
	u[2]=u[2]/d1;

	temp.mat[0]=s[0];
	temp.mat[1]=u[0];
	temp.mat[2]=-F[0];

	temp.mat[4]=s[1];
	temp.mat[5]=u[1];
	temp.mat[6]=-F[1];

	temp.mat[8]=s[2];
	temp.mat[9]=u[2];
	temp.mat[10]=-F[2];
	int i;	
	float *ans=multiply(View.mat,temp.mat);
	for(i=0;i<16;i++)
		View.mat[i]=ans[i];
	sglViewTranslate(-cx,-cy,-cz);	
}


// < ---------------=============== VIEWING ENDS ================------------------ >

float* multiply(float a[16], float b[16]) //  A X B
{
	float c[4][4];
	float d[4][4];
	float result[4][4] = {{0.0f}};
	int i,j,k;
	j=-1;

	float* ans;
	ans = (float *)malloc(sizeof(float) * 16);
	
	for(i=0;i<16;i++)
	{
		if(i%4==0)
		j++;
		c[i%4][j]=a[i];
		d[i%4][j]=b[i];
	}

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			for(k=0;k<4;k++)
			{
				result[i][j]+=c[i][k]*d[k][j];
			}
		}
	}
	
	j=-1;
	for(i=0;i<16;i++)
	{
		if(i%4==0)
		j++;
		ans[i]=result[i%4][j];
	}
	
	return &(ans[0]);
}


// <---------------===============  PROJECTION  =====================--------------- >

void sglProjOrtho(float left,float right,float bottom ,float top,float near,float far)
{
	int i;
	for(i=0;i<16;i++)
	{
		if(i%5==0)
			Proj.mat[i]=1;
		else
			Proj.mat[i]=0;
	}
	float current[16]= {2/(right-left),0,0,0,0,2/(top-bottom),0,0,0,0,2/(near-far),0,(right+left)/(left-right),(top+bottom)/(bottom-top),(far+near)/(near-far),1};

	float p[16];
	for(i=0;i<16;i++)
		p[i] = Proj.mat[i];

	float* ans1=multiply(p,current);
	
	for(i=0;i<16;i++)
		Proj.mat[i] = ans1[i];
}

void sglProjFrustum(float left,float right,float bottom,float top,float near,float far)
{
	int i;
	for(i=0;i<16;i++)
		Proj.mat[i]=0;
	Proj.mat[0]=(2*near)/(right-left);
	Proj.mat[5]=(2*near)/(top-bottom);
	Proj.mat[8]=(right+left)/(right-left);
	Proj.mat[9]=(top+bottom)/(top-bottom);
	Proj.mat[10]=(far+near)/(near-far);
	Proj.mat[11]=-1;
	Proj.mat[14]=(2*near*far)/(near-far);
}

void sglViewport(float lx,float ly,float w,float h)
{
	vportW=(float)glutGet(GLUT_WINDOW_WIDTH);
	vportH=(float)glutGet(GLUT_WINDOW_HEIGHT);
	vportx=lx;
	vporty=ly;
	width=w;
	height=h;
}

// < -----------------================= PRIMITIVES ==================----------------- >

void sglBegin()
{
	flag = 1;
	int i;
	float* a1=multiply(Proj.mat,View.mat);
	float* a2=multiply(a1,m_stack.top().mat);
	for(i=0;i<16;i++)
		Trans.mat[i]=a2[i];
}

void sglEnd()
{
	flag = 0;
	sglShow();
}

void sglColour(float r,float g,float b)
{
	ColorR=r;
	ColorG=g;
	ColorB=b;
}

void sglClear(float r,float g,float b)
{
	glClearColor(r,g,b,1.0f);
}

void sglVertex(float x, float y,float z)
{
	if(flag==1)
	{
		float orc[4]={x,y,z,1.0f};
		int i;
		
		float point[4];
		for(i=0;i<4;i++)
			point[i]=orc[0]*Trans.mat[i] + orc[1]*Trans.mat[i+4] + orc[2]*Trans.mat[i+8] + orc[3]*Trans.mat[i+12];

		point[0]=(point[0]+1)*(width/2);
		point[1]=(point[1]+1)*(height/2);

		point[0]+=vportx;
		point[1]+=vporty;
		
		point[0]=(point[0]*2)/vportW-1;
		point[1]=(point[1]*2)/vportH-1;

		vertices.push_back(point[0]);
		vertices.push_back(point[1]);
		colors.push_back(ColorR);
		colors.push_back(ColorG);
		colors.push_back(ColorB);
	}
}

void sglShow()
{
	FILE *out;
	out=fopen("draw.ply","w");
	fprintf(out,"ply\n");
	fprintf(out,"format ascii 1.0\n");
	fprintf(out,"element vertex %d\n",vertices.size());
	fprintf(out,"property float x\n");
	fprintf(out,"property float y\n");
	fprintf(out,"end header\n");
	
	glBegin(GL_TRIANGLES);
	int i,j=0;
	for(i=0;i<vertices.size();i+=2)
	{
		//cout<<vertices[i]<<" "<<vertices[i+1]<<endl;
		glColor3f(colors[j],colors[j+1],colors[j+2]);
		glVertex2f(vertices[i],vertices[i+1]);
		j+=3;
	}
	glEnd();
	

	for(i=0;i<vertices.size();i+=2)
		fprintf(out,"%f %f\n",vertices[i],vertices[i+1]);

	vertices.clear();
	colors.clear();
}

