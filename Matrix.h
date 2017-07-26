#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "Error.h"

__if_not_exists (flo) {
	typedef float flo;
}

template<class _T = flo>
class tMatrix
{
	static_assert(std::is_floating_point<_T>::value,"type \"tMatrix<_T>\" is floating-point-based type.");
private:
	unsigned int size_a, size_b;
	std::vector<std::vector<_T> > A;
	void resize(const unsigned int a, const unsigned int b) {
		if(a <= 0 || b <= 0)
			throw MyError(std::string("tMatrix<_T>::resize(int,int) : Parameters must be greater than zero."), nsError::EXIT | nsError::ERROR);
		size_a = a;
		size_b = b;
		A.resize(a);
		for(unsigned int i = 0; i < a; i++) {
			A[i].resize(b);
		}
	}
	_T HorDet(const unsigned int line) const {
		int mod;
		_T det = 0;
		if(line%2 == 1) mod = -1;
		else mod = 1;
		for(unsigned int i = 0; i < size_b; i++, mod*=(-1)) {
			if(A[line][i] == (_T)0) continue;
			det+=mod*A[line][i]*((Minor(line,i)).Det());
		}
		return det;
	}
	_T VerDet(const unsigned int line) const{
		int mod;
		_T det = 0;
		if(line%2 == 1) mod = -1;
		else mod = 1;
		for(unsigned int i = 0; i < size_a; i++, mod*=(-1)) {
			if(A[i][line] == (_T)0) continue;
			det+=mod*A[i][line]*((Minor(i,line)).Det());
		}
		return det;
	}
public:
	tMatrix(const unsigned int a = 3, const unsigned int b = 3) {
		try {
			resize(a,b);
		}
		catch(MyError & er) {
			er.mes = "tMatrix<_T>::tMatrix<_T>(int,int) : Parameters must be greater than zero.";
			er.Execute();
		}
		if(a==b) {
			for(unsigned int i = 0; i < a; i++) {
				for(unsigned int j = 0; j < b; j++) {
					if(i==j) A[i][j] = (_T)1;
					else A[i][j] = (_T)0;
				}
			}
		}
		else {
			for(unsigned int i = 0; i < a; i++) {
				for(int j = 0; j < b; j++) {
					A[i][j] = (_T)0;
				}
			}
		}
	}
	tMatrix(_T** input_massive, const unsigned int a = 3, const unsigned int b = 3) {
		try {
			resize(a,b);
		}
		catch(MyError & er) {
			er.mes = "tMatrix<_T>::tMatrix<_T>(_T**,int,int) : Parameters must be greater than zero.";
			er.Execute();
		}
		for(int i = 0; i < a; i++) {
			for(int j = 0; j < b; j++) {
				A[i][j] = input_massive[i][j];
			}
		}
	}
	tMatrix(_T * input_massive, const unsigned int a = 3, const unsigned int b = 3) {
		try {
			resize(a,b);
		}
		catch(MyError & er) {
			er.mes = "tMatrix<_T>::tMatrix<_T>(_T*,int,int) : Parameters must be greater than zero.";
			er.Execute();
		}
		for(int i = 0; i < a; i++) {
			for(int j = 0; j < b; j++) {
				A[i][j] = input_massive[i*a+j];
			}
		}
	}
	inline int sizeA() const {
		return size_a;
	}
	inline int sizeB() const {
		return size_b;
	}
	inline _T & El(const unsigned int a, const unsigned int b) {
		if(a >= size_a || b >= size_b)
			throw MyError(std::string("tMatrix<_T>::El(int,int) : Parameter is out of the range."), nsError::EXIT | nsError::ERROR);
		return A[a][b];
	}
	inline const _T & El(const unsigned int a, const unsigned int b) const {
		if(a >= size_a || b >= size_b)
			throw MyError(std::string("tMatrix<_T>::El(int,int) : Parameter is out of the range."), nsError::EXIT | nsError::ERROR);
		return A[a][b];
	}
	tMatrix<_T> operator*(const tMatrix<_T> & right) {
		if(this->size_b != right.size_a)
			throw MyError(std::string("tMatrix<_T>::operator*(tMatrix<_T>,tMatrix<_T>) : Wrong sizes of matrixes."),nsError::EXIT | nsError::ERROR);
		const int & sa = this->size_a;
		const int & sb = right.size_b;
		const int & sm = this->size_b;
		tMatrix<_T> res(sa,sb);
		for(int i = 0; i < sa; i++) {
			for(int j = 0; j < sb; j++) {
				_T sum = (_T)0;
				for(int k = 0; k < sm; k++) {
					sum += this->El(i,k) * right.El(k,j);
				}
				res.El(i,j) = sum;
			}
		}
	}
	std::vector<_T> operator*(const std::vector<_T> & right) {
		if(this->size_b != right.size())
			throw MyError("tMatrix<_T>::operator*(tMatrix<_T>,std::vector<_T>) : Wrong size of matrix or vector.",nsError::EXIT | nsError::MyError);
		std::vector<_T> res(this->size_a, 0);
		for(int i = 0; i < this->size_a; i++) {
			for(int j = 0; j < this->size_b; j++) {
				res[i] += this->A[i][j]*right[j];
			}
		}
		return res;
	}
	tMatrix<_T> operator/(const _T right) {
		for(int i = 0; i < this->size_a; i++) {
			for(int j = 0; j < this->size_b; j++) {
				this->A[i][j]/=right;
			}
		}
		return left;
	}
	tMatrix<_T> Minor(const unsigned int a, const unsigned int b) const {
		if(a >= size_a || b >= size_b)
			throw MyError("tMatrix<_T>::Minor(int,int) : Parameter is out of the range.", nsError::EXIT | nsError::ERROR);
		if(size_a <= 1 || size_b <= 1)
			throw MyError("tMatrix<_T>::Minor(int,int) : Cannot take a minor of size(1) matrix.", nsError::EXIT | nsError::ERROR);
		tMatrix<_T> res(size_a - 1, size_b - 1);
		for(int i = 0, i1 = 0; i < size_a; i++) {
			if(i==a) continue;
			for(int j = 0, j1 = 0; j < size_b; j++) {
				if(j==b) continue;
				res.El(i1,j1) = A[i][j];
				j1++;
			}
			i1++;
		}
		return res;
	}
	tMatrix<_T> Transponate() const {
		tMatrix<_T> res(size_b,size_a);
		for(int i = 0; i < size_a; i++) {
			for(int j = 0; j < size_b; j++) {
				res.El(j,i) = A[i][j];
			}
		}
		return res;
	}
	tMatrix<_T> Invert() const {
		tMatrix<_T> tran(Transponate());
		tMatrix<_T> res(tran.sizeA(), tran.sizeB());
		_T det = Det();
		for(int i = 0; i < size_b; i++) {
			for(int j = 0; j < size_a; j++) {
				res.El(i,j) = (tran.Minor(i,j)).Det() / det;
			}
		}
		return res;
	}
	_T Det(const unsigned int line = 0, const bool horisontaly = true) const {
		if(size_a != size_b)
			throw MyError("tMatrix<_T>::Det(int=,bool=) : Matrix must be square.", nsError::EXIT | nsError::ERROR);
		switch(size_a) {
		case 1:
			return A[0][0];
		case 2:
			return A[0][0]*A[1][1] - A[0][1]*A[1][0];
		case 3:
			return A[0][0]*A[1][1]*A[2][2] + A[0][1]*A[1][2]*A[2][0] + A[0][2]*A[1][0]*A[2][1]
			- A[0][2]*A[1][1]*A[2][0] - A[0][1]*A[1][0]*A[2][2] - A[0][0]*A[1][2]*A[2][1];
		}
		if(line >= size_a)
			throw MyError("tMatrix<_T>::Minor(int,int) : Parameter is out of the range.", nsError::EXIT | nsError::ERROR);
		if(horisontaly) {
			return HorDet(line);
		} else {
			return VerDet(line);
		}
	}
};

template<class _T = flo>
tMatrix<_T> operator*(const _T left, const tMatrix<_T> right) {
	for (int i = 0; i < right.size_a; i++) {
		for (int j = 0; j < right.size_b; j++) {
			right.A[i][j] *= left;
		}
	}
	return right;
}


typedef tMatrix<> Matrix;

#endif
