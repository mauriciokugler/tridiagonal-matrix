//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#include "TridiagonalMatrix.h"

int main(void)
{
	//Tridiagonal inverse
	TridiagonalMatrix *TRI = new TridiagonalMatrix(5, 1, 4, 1);

	float **f = TRI->inverse();

	//Tridiagonal system (dynamic)
	float y[] ={ 1, 2, 3, 4, 5 };

	float *x = TRI->system(y);

	//Tridiagonal system (static)
	float *z = new float[5];

	TDM::system(y, z, 5, 1, 4, 1);

	delete TRI;

	return 0;
}