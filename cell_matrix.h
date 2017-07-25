#pragma once
#ifndef CELL_MATRIX_H
#define CELL_MATRIX_H


__if_not_exists (flo) {
	typedef float flo;
}

template<class _T = flo>
struct tMatrix
{             // 0  1  2  3  4  5
	_T U[6]; // x2 y2 z2 xy xz yz
	tMatrix()
	{
		for (int i = 0; i<3; i++)
			U[i] = 1;
		for (int i = 3; i<6; i++)
			U[i] = 0;

	}

	void RotateZ(_T angle)
	{
		_T Cos = cos(angle);
		_T Sin = sin(angle);

		_T temp[6];

		temp[0] = U[0] * _quad(Cos) + U[1] * _quad(Sin) - U[3] * Cos*Sin;
		temp[1] = U[0] * _quad(Sin) + U[1] * _quad(Cos) + U[3] * Cos*Sin;
		temp[2] = U[2];
		temp[3] = U[3] * (_quad(Cos) - _quad(Sin)) + 2 * Cos*Sin*(U[0] - U[1]);
		temp[4] = U[4] * Cos - U[5] * Sin;
		temp[5] = U[5] * Cos + U[4] * Sin;

		for (int i = 0; i<6; i++)
			U[i] = temp[i];
	}
	void RotateX(_T angle)
	{
		_T Cos = cos(angle);
		_T Sin = sin(angle);

		_T temp[6];

		temp[0] = U[0];
		temp[1] = U[1] * _quad(Cos) + U[2] * _quad(Sin) - U[0] * Cos*Sin;
		temp[2] = U[1] * _quad(Sin) + U[2] * _quad(Cos) + U[0] * Cos*Sin;
		temp[3] = U[3] * Cos - U[4] * Sin;
		temp[4] = U[3] * Cos + U[4] * Sin;
		temp[5] = U[5] * (_quad(Cos) - _quad(Sin)) + 2 * Cos*Sin*(U[1] - U[2]);

		for (int i = 0; i<6; i++)
			U[i] = temp[i];
	}
	tMatrix<_T> Mirror() const
	{
		Matrix mat;
		mat.U[0] = 1 / U[0];
		mat.U[1] = 1 / U[1];
		mat.U[2] = 1 / U[2];
		mat.U[3] = -U[3] / (U[0] * U[1]);
		mat.U[4] = (U[3] * U[5] - U[4] * U[1]) / (U[0] * U[1] * U[2]);
		mat.U[5] = -U[5] / (U[2] * U[1]);
		return mat;
	}
	Point Transform(Point a) const
	{
		Point ret;
		ret.x = U[0] * a.x + U[3] * a.y + U[4] * a.z;
		ret.y = U[1] * a.y + U[5] * a.z;
		ret.z = U[2] * a.z;
		return ret;
	}
};
typedef tMatrix<> Matrix;

template<class _T = flo>
struct tCell
{
private:
	const _T RadtoGrad = (_T)(90 / 1.57079632679489661923);
	const _T GradtoRad = (_T)(1.57079632679489661923 / 90);
public:
	_T a, b, c;
	_T alpha, beta, gamma;
	tCell(Matrix Mat) {
		for (int i = 0; i< 6; i++)
			if (Mat.U[i] == 0) Mat.U[i] = _T(0.0000000000000001);
		a = Mat.U[0];
		b = sqrt(_quad(Mat.U[1]) + _quad(Mat.U[3]));
		_T CosG = Mat.U[3] / b; //CosG
		_T SinG = sqrt(1 - _quad(CosG)); //SinG
		gamma = acos(CosG);

		_T temp = Mat.U[4] * CosG + Mat.U[5] * SinG;
		c = sqrt(_quad(Mat.U[2] * SinG + _quad(temp) - 2 * CosG*temp*Mat.U[4] + _quad(Mat.U[4]))) / SinG;
		beta = acos(Mat.U[4] / c);
		alpha = acos(cos(beta)*temp);
		toGrad();
	}
	tCell() {
		a = b = c = 0;
		alpha = beta = gamma = 0;
	}
	void toGrad()
	{
		alpha *= RadtoGrad;
		beta *= RadtoGrad;
		gamma *= RadtoGrad;
	}
	void toRad()
	{
		alpha *= GradtoRad;
		beta *= GradtoRad;
		gamma *= GradtoRad;
	}
	tMatrix<_T> CreateMatrix()
	{
		toRad();
		Matrix res;
		res.U[0] = a;
		res.U[1] = b*sin(gamma);
		res.U[2] = c*sqrt(1 - (cos(alpha)*cos(alpha)) - (cos(beta)*cos(beta)) - (cos(gamma)*cos(gamma)) + 2 * cos(alpha)*cos(beta)*cos(gamma)) / sin(gamma);
		res.U[3] = b*cos(gamma);
		res.U[4] = c*cos(beta);
		res.U[5] = c / sin(gamma)*(cos(alpha) - cos(beta)*cos(gamma));
		for (int i = 3; i < 6; i++)
			if (abs(res.U[i]) < (1E-10)) res.U[i] = 0;
		toGrad();
		return res;
	}
};
typedef tCell<> Cell;


#endif