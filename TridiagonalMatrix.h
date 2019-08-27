//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#ifndef TRIDIAGONALMATRIXH
#define TRIDIAGONALMATRIXH

class TridiagonalMatrix
{
private:

	unsigned int N;		//matrix dimension

	float *a;			//diagonal
	float *b;			//upper 
	float *c;			//lower

	float *p;
	float *q;
	float **t;			//inverse matrix

	float *u;
	float *v;			
	float *x;			//system solution

public:

	TridiagonalMatrix(unsigned int n, float *lower, float *diagonal, float *upper);
	TridiagonalMatrix(unsigned int n, float lower, float diagonal, float upper);
	~TridiagonalMatrix();

	float **inverse();
	float *system(float *y);

	static void inline system(float *y, float *x, unsigned int n, float lower, float diagonal, float upper);

};

using TDM = TridiagonalMatrix;

void inline TridiagonalMatrix::system(float *y, float *x, unsigned int n, float lower, float diagonal, float upper)
{
	//Forward Sweep
	x[0] = diagonal;

	for(unsigned int i=1;i<n;i++) {
		float w = lower/x[i-1];
		x[i] = diagonal - w*upper;
		y[i] = y[i] - w*y[i-1];
	}
	
	//Back Substitution
	x[n-1] = y[n-1]/x[n-1];
	
	for(int i=n-2;i>=0;i--) {
		x[i] = (y[i] - upper*x[i+1]) / x[i];
	}
}

#endif