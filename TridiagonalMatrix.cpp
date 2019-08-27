//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "TridiagonalMatrix.h" 

TridiagonalMatrix::TridiagonalMatrix(unsigned int n, float *lower, float *diagonal, float *upper)
{
	N = n;

	a = new float[N];
	b = new float[N-1];
	c = new float[N-1];

	memcpy(a, diagonal, sizeof(float)*N);
	memcpy(b, diagonal, sizeof(float)*(N-1));
	memcpy(c, diagonal, sizeof(float)*(N-1));

	t = NULL;
	x = NULL;
}

TridiagonalMatrix::TridiagonalMatrix(unsigned int n, float lower, float diagonal, float upper)
{
	N = n;

	a = new float[N];
	b = new float[N-1];
	c = new float[N-1];

	for(unsigned int i=0;i<N-1;i++) {
		a[i] = diagonal;
		b[i] = upper;
		c[i] = lower;
	}

	a[N-1] = diagonal;

	t = NULL;
	x = NULL;
}

TridiagonalMatrix::~TridiagonalMatrix()
{
	if(N>0) {
		delete[] a;
		delete[] b;
		delete[] c;
	}

	if(t!=NULL) {
		for(unsigned int i=0;i<N;i++) delete[] t[i];
		delete[] t;

		delete[] p;
		delete[] q;
	}

	if(x!=NULL) {
		delete[] x;
		delete[] u;
		delete[] v;
	}
}

float **TridiagonalMatrix::inverse()
{
	if(a==NULL || b==NULL || c==NULL) return(NULL);

	if(t==NULL) {
		t = new float*[N];
		for(unsigned int i=0;i<N;i++) {
			t[i] = new float[N];
		}

		p = new float[N+1];
		q = new float[N+1];
	}

	p[0] = 1;
	p[1] = a[0];

	for(unsigned int i=2;i<N+1;i++) {
		p[i] = a[i-1]*p[i-1] - b[i-2]*c[i-2]*p[i-2];
	}

	q[N] = 1;
	q[N-1] = a[N-1];
	q[N-2] = a[N-2]*q[N-1] - b[N-2]*c[N-2]*q[N];

	for(int i=N-3;i>=0;i--) {
		q[i] = a[i]*q[i+1] - b[i]*c[i]*q[i+2];
	}

	for(unsigned int i=0;i<N;i++) {
		for(unsigned int j=0;j<N;j++) {
			int s = 1-(((i+j)%2)<<1);

			if(i<j) {
				t[i][j] = s*p[i]*q[j+1]/p[N];

				for(unsigned int k=i;k<j-1;k++) {
					t[i][j] *= b[k];
				}
			}
			if(i>j) {
				t[i][j] = s*p[j]*q[i+1]/p[N];

				for(unsigned int k=j;k<i-1;k++) {
					t[i][j] *= c[k];
				}
			}
			else {
				t[i][j] = s*p[i]*q[j+1]/p[N];
			}
			
		}
	}

	return(t);
}

float *TridiagonalMatrix::system(float *y)
{
	if(a==NULL || b==NULL || c==NULL) return(NULL);

	if(x==NULL) {
		x = new float[N];
		u = new float[N];
		v = new float[N];
	}

	//Forward Sweep
	u[0] = b[0]/a[0];
	v[0] = y[0]/a[0];

	for(unsigned int i=1;i<N;i++) {
		u[i] = b[i]/(a[i]-c[i-1]*u[i-1]);
		v[i] = (y[i]-c[i-1]*v[i-1])/(a[i]-c[i-1]*u[i-1]);
	}

	//Back Substitution
	x[N-1] = v[N-1];

	for(int i=N-2;i>=0;i--) {
		x[i] = v[i] - u[i]*x[i+1];
	}

	return(x);
}