#pragma once
#ifndef POINT_H
#define POINT_H

#include "Error.h"
#include <vector>
#include "Matrix.h"
__if_not_exists (flo) {
	typedef float flo;
}

template<class _T = flo>
struct tPoint
{
	static_assert(std::is_floating_point<_T>::value,"Type \"Point\" is floating-point-based type.");
	_T a[3];

	tPoint() : a{0,0,0} {}
	tPoint(_T _x, _T _y, _T _z) : a{ _x,_y,_z } {};
	tPoint(const std::vector<_T> & vec) {
		if(vec.size() < 3) Error::FatalError("Size of std::vector < 3 in constructor tPoint<_T>(std::vector<_T>).")
		a{vec[0], vec[1], vec[2]};
	}

	_T r() const {
		return sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	}
	_T V() const {
		return a[0]*a[1]*a[2];
	}
	void normalize() {
		_T R( r() );
		a[0] /= R;
		a[1] /= R;
		a[2] /= R;
	}
	std::vector<_T> ToVector() const {
		return std::vector<_T> {a[0], a[1], a[2]};
	}

	tPoint<_T> operator-() const
	{
		return tPoint<_T>(-this->a[0], -this->a[1], -this->a[2]);
	}
	tPoint<_T> operator+(const tPoint<_T> & right) const
	{
		return tPoint<_T>(this->a[0]+right.a[0], this->a[1]+right.a[1], this->a[2]+right.a[2]);
	}
	tPoint<_T> operator+(const _T b) const
	{
		return tPoint<_T>(this->a[0]+b, this->a[1]+b, this->a[2]+b);
	}
	tPoint<_T> operator-(const tPoint<_T> & right)
	{
		return tPoint<_T>(this->a[0]-right.a[0], this->a[1]-right.a[1], this->a[2]-right.a[2]);
	}
	tPoint<_T> operator-(const _T b)
	{
		return tPoint<_T>(this->a[0]-b, this->a[1]-b, this->a[2]-b);
	}
	tPoint<_T> operator*(const tPoint<_T> & right)
	{
		return tPoint<_T>(this->a[0]*right.a[0], this->a[1]*right.a[1], this->a[2]*right.a[2]);
	}
	tPoint<_T> operator*(const _T b)
	{
		return tPoint<_T>(this->a[0]*b, this->a[1]*b, this->a[2]*b);
	}
	tPoint<_T> operator/(const tPoint<_T> & right)
	{
		return tPoint<_T>(this->a[0]/right.a[0], this->a[1]/right.a[1], this->a[2]/right.a[2]);
	}
	tPoint<_T> operator/(const _T b)
	{
		return tPoint<_T>(this->a[0]/b, this->a[1]/b, this->a[2]/b);
	}
	tPoint<_T> operator+=(const tPoint<_T> & right)
	{
		return tPoint<_T> (this->a[0]+=right.a[0], this->a[1]+=right.a[1], this->a[2]+=right.a[2]);
	}
	tPoint<_T> operator+=(const _T right)
	{
		return tPoint<_T> (this->a[0]+=right, this->a[1]+=right, this->a[2]+=right);
	}
	tPoint<_T> operator-=(const tPoint<_T> & right)
	{
		return tPoint<_T> (this->a[0]-=right.a[0], this->a[1]-=right.a[1], this->a[2]-=right.a[2]);
	}
	tPoint<_T> operator-=(const _T right)
	{
		return tPoint<_T> (this->a[0]-=right, this->a[1]-=right, this->a[2]-=right);
	}
	tPoint<_T> operator*=(const tPoint<_T> & right)
	{
		return tPoint<_T> (this->a[0]*=right.a[0], this->a[1]*=right.a[1], this->a[2]*=right.a[2]);
	}
	tPoint<_T> operator*=(const _T right)
	{
		return tPoint<_T> (this->a[0]*=right, this->a[1]*=right, this->a[2]*=right);
	}
	tPoint<_T> operator/=(const _T right)
	{
		return tPoint<_T> (this->a[0]/=right, this->a[1]/=right, this->a[2]/=right);
	}
	bool operator==(const tPoint<_T> & right)
	{
		return ((this->a[0] == right.a[0]) && (this->a[1] == right.a[1]) && (this->a[2] == right.a[2]));
	}
	bool operator!=(const tPoint<_T> & right)
	{
		return ((this->a[0] != right.a[0]) || (this->a[1] != right.a[1]) || (this->a[2] != right.a[2]));
	}
};

template<class _T = flo>
tPoint<_T> operator+(const _T b, const tPoint<_T> & right)
{
	return tPoint<_T>(b + right.a[0], b + right.a[1], b + right.a[2]);
}

template<class _T = flo>
tPoint<_T> operator-(const _T b, const tPoint<_T> & right)
{
	return tPoint<_T>(b - right.a[0], b - right.a[1], b - right.a[2]);
}

template<class _T = flo>
tPoint<_T> operator*(const _T b, const tPoint<_T> & right)
{
	return tPoint<_T>(b*right.a[0], b*right.a[1], b*right.a[2]);
}

template<class _T = flo>
tPoint<_T> operator/(const _T b, const tPoint<_T> & right)
{
	return tPoint<_T>(b / right.a[0], b / right.a[1], b / right.a[2]);
}

template<class _T = flo>
tPoint<_T> operator*(const tMatrix<_T> & left, const tPoint<_T> & right) {
	if (left.sizeA() != 3 || left.sizeB() != 3)
		throw Error("tPoint<_T>::operator*(tMatrix<_T>,tPoint<_T>) : Matrix must have size 3x3.", nsError::EXIT | nsError::ERROR);
	tPoint<_T> res;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			res.a[i] += left.El(i, j) * right.a[j];
		}
	}
	return res;
}

typedef tPoint<> Point;
#endif
