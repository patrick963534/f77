/* Some simple mathematical functions. Don't look for some logic in
   the function names :-) */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "r7_zmath.h"

void r7_m44_identity(M44* a)
{
    int i,j;
    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j<4; ++j) 
        {
            if (i == j) 
                a->m[i][j] = 1.0f; 
            else 
                a->m[i][j] = 0.0f;
        }
    }
}

int r7_m44_is_identity(const M44* a)
{
    int i,j;
    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j < 4; ++j) 
        {
            if (i == j) 
            { 
                if (a->m[i][j] != 1.0f) 
                    return 0;
            } 
            else if (a->m[i][j] != 0.0f) 
                return 0;
        }
    }
    return 1;
}

void r7_m44_multiply(M44* c, const M44* a, const M44* b)
{
    int i,j,k;
    float s;

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++) 
        {
            s=0.0;

            for(k=0;k<4;k++) 
                s+=a->m[i][k]*b->m[k][j];

            c->m[i][j]=s;
        }
    }
}

/* c=c*a */
void r7_m44_multiply_left(M44* c, const M44* b)
{
    int i,j,k;
    float s;
    M44 a;

    /*memcpy(&a, c, 16*sizeof(float));
    */
    a=*c;

    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j<4; ++j) 
        {
            s = 0.0;

            for(k = 0;k < 4; ++k) 
                s+=a.m[i][k]*b->m[k][j];

            c->m[i][j]=s;
        }
    }
}

void r7_m44_copy(M44* a, const M44* b)
{
	memcpy(a,b,sizeof(M44));
}

void r7_v3_copy(V3* a, const V3* b)
{
	memcpy(a,b,sizeof(V3));
}

void r7_m43_multiply(V3* a, const M44* b, const V3* c)
{
	 a->X=b->m[0][0]*c->X+b->m[0][1]*c->Y+b->m[0][2]*c->Z+b->m[0][3];
	 a->Y=b->m[1][0]*c->X+b->m[1][1]*c->Y+b->m[1][2]*c->Z+b->m[1][3];
	 a->Z=b->m[2][0]*c->X+b->m[2][1]*c->Y+b->m[2][2]*c->Z+b->m[2][3];
}

void r7_m33_multiply(V3* a, const M44* b, const V3* c)
{
	 a->X=b->m[0][0]*c->X+b->m[0][1]*c->Y+b->m[0][2]*c->Z;
	 a->Y=b->m[1][0]*c->X+b->m[1][1]*c->Y+b->m[1][2]*c->Z;
	 a->Z=b->m[2][0]*c->X+b->m[2][1]*c->Y+b->m[2][2]*c->Z;
}

void r7_m44_mul_vector4(V4* a, const M44* b, const V4* c)
{
	 a->X=b->m[0][0]*c->X+b->m[0][1]*c->Y+b->m[0][2]*c->Z+b->m[0][3]*c->W;
	 a->Y=b->m[1][0]*c->X+b->m[1][1]*c->Y+b->m[1][2]*c->Z+b->m[1][3]*c->W;
	 a->Z=b->m[2][0]*c->X+b->m[2][1]*c->Y+b->m[2][2]*c->Z+b->m[2][3]*c->W;
	 a->W=b->m[3][0]*c->X+b->m[3][1]*c->Y+b->m[3][2]*c->Z+b->m[3][3]*c->W;
}
	
/* transposition of a 4x4 matrix */
void r7_m44_transpose(M44* a, const M44* b)
{
  a->m[0][0]=b->m[0][0]; 
  a->m[0][1]=b->m[1][0]; 
  a->m[0][2]=b->m[2][0]; 
  a->m[0][3]=b->m[3][0]; 

  a->m[1][0]=b->m[0][1]; 
  a->m[1][1]=b->m[1][1]; 
  a->m[1][2]=b->m[2][1]; 
  a->m[1][3]=b->m[3][1]; 

  a->m[2][0]=b->m[0][2]; 
  a->m[2][1]=b->m[1][2]; 
  a->m[2][2]=b->m[2][2]; 
  a->m[2][3]=b->m[3][2]; 

  a->m[3][0]=b->m[0][3]; 
  a->m[3][1]=b->m[1][3]; 
  a->m[3][2]=b->m[2][3]; 
  a->m[3][3]=b->m[3][3]; 
}

/* inversion of an orthogonal matrix of type Y=M.X+P */ 
void r7_m44_inv_ortho(M44* a, M44 b)
{
	int i,j;
	float s;
	for(i=0;i<3;i++)
	for(j=0;j<3;j++) a->m[i][j]=b.m[j][i];
	a->m[3][0]=0.0; a->m[3][1]=0.0; a->m[3][2]=0.0; a->m[3][3]=1.0;
	for(i=0;i<3;i++) {
		s=0;
		for(j=0;j<3;j++) s-=b.m[j][i]*b.m[j][3];
		a->m[i][3]=s;
	}
}

/* Inversion of a general nxn matrix.
   Note : m is destroyed */

static int matrix_inverse(float* r,float* m,int n)
{
	 int i,j,k,l;
	 float max,tmp,t;

	 /* identitée dans r */
	 for(i=0;i<n*n;i++) r[i]=0;
	 for(i=0;i<n;i++) r[i*n+i]=1;
	 
	 for(j=0;j<n;j++) {
			
			/* recherche du nombre de plus grand module sur la colonne j */
			max=m[j*n+j];
			k=j;
			for(i=j+1;i<n;i++)
				if (fabs(m[i*n+j])>fabs(max)) {
					 k=i;
					 max=m[i*n+j];
				}

      /* non intersible matrix */
      if (max==0) return 1;

			
			/* permutation des lignes j et k */
			if (k!=j) {
				 for(i=0;i<n;i++) {
						tmp=m[j*n+i];
						m[j*n+i]=m[k*n+i];
						m[k*n+i]=tmp;
						
						tmp=r[j*n+i];
						r[j*n+i]=r[k*n+i];
						r[k*n+i]=tmp;
				 }
			}
			
			/* multiplication de la ligne j par 1/max */
			max=1/max;
			for(i=0;i<n;i++) {
				 m[j*n+i]*=max;
				 r[j*n+i]*=max;
			}
			
			
			for(l=0;l<n;l++) if (l!=j) {
				 t=m[l*n+j];
				 for(i=0;i<n;i++) {
						m[l*n+i]-=m[j*n+i]*t;
						r[l*n+i]-=r[j*n+i]*t;
				 }
			}
	 }

	 return 0;
}


/* inversion of a 4x4 matrix */

void r7_m44_inverse(M44* a, const M44* b)
{
  M44 tmp;
  memcpy(&tmp, b, 16*sizeof(float));
  /*tmp=*b;*/
  matrix_inverse(&a->m[0][0],&tmp.m[0][0],4);
}

void r7_m44_rotate(M44* a,float t,int u)
{
	 float s,c;
	 int v,w;
   if ((v=u+1)>2) v=0;
	 if ((w=v+1)>2) w=0;
	 s=(float)sin(t);
	 c=(float)cos(t);
	 r7_m44_identity(a);
	 a->m[v][v]=c;	a->m[v][w]=-s;
	 a->m[w][v]=s;	a->m[w][w]=c;
}
	

/* inverse of a 3x3 matrix */
void r7_m33_inverse(M33* a, M33* m)
{
	 float det;
	 
	 det = m->m[0][0]*m->m[1][1]*m->m[2][2]-m->m[0][0]*m->m[1][2]*m->m[2][1]-
		 m->m[1][0]*m->m[0][1]*m->m[2][2]+m->m[1][0]*m->m[0][2]*m->m[2][1]+
		 m->m[2][0]*m->m[0][1]*m->m[1][2]-m->m[2][0]*m->m[0][2]*m->m[1][1];

	 a->m[0][0] = (m->m[1][1]*m->m[2][2]-m->m[1][2]*m->m[2][1])/det;
	 a->m[0][1] = -(m->m[0][1]*m->m[2][2]-m->m[0][2]*m->m[2][1])/det;
	 a->m[0][2] = -(-m->m[0][1]*m->m[1][2]+m->m[0][2]*m->m[1][1])/det;
	 
	 a->m[1][0] = -(m->m[1][0]*m->m[2][2]-m->m[1][2]*m->m[2][0])/det;
	 a->m[1][1] = (m->m[0][0]*m->m[2][2]-m->m[0][2]*m->m[2][0])/det;
	 a->m[1][2] = -(m->m[0][0]*m->m[1][2]-m->m[0][2]*m->m[1][0])/det;

	 a->m[2][0] = (m->m[1][0]*m->m[2][1]-m->m[1][1]*m->m[2][0])/det;
	 a->m[2][1] = -(m->m[0][0]*m->m[2][1]-m->m[0][1]*m->m[2][0])/det;
	 a->m[2][2] = (m->m[0][0]*m->m[1][1]-m->m[0][1]*m->m[1][0])/det;
}

																										
/* vector arithmetic */

int r7_v3_normal(V3* a)
{
	float n;
	n=(float)sqrt(a->X*a->X+a->Y*a->Y+a->Z*a->Z);
	if (n==0) return 1;
	a->X/=n;
	a->Y/=n;
	a->Z/=n;
	return 0;
}

V3 r7_v3_new(float x,float y,float z)
{
	 V3 a;
	 a.X=x;
	 a.Y=y;
	 a.Z=z;
	 return a;
}

V4 r7_v4_new(float x,float y,float z,float w)
{
  V4 a;
  a.X=x;
  a.Y=y;
  a.Z=z;
  a.W=w;
  return a;
}


