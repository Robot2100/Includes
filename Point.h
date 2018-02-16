#pragma once
#ifndef POINT_H
#define POINT_H

#include <vector>
#include "Matrix.h"
__if_not_exists (flo) {
	typedef double flo;
}

template<class _T = flo>
struct tPoint
{
	static_assert(std::is_floating_point<_T>::value,"Type \"Point\" is floating-point-based type.");
	_T a[3];

	constexpr tPoint() noexcept : a{ static_cast<flo>(0.0), static_cast<flo>(0.0), static_cast<flo>(0.0) } {}
	constexpr tPoint(_T _x, _T _y, _T _z) noexcept : a{ _x,_y,_z } {};
	tPoint(const std::vector<_T> & vec) {
		if (vec.size() != 3)
			throw std::invalid_argument("WrongVectorSize");
		a{vec[0], vec[1], vec[2]};
	}

	constexpr _T r() const noexcept {
		return sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	}
	constexpr _T V() const noexcept {
		return a[0]*a[1]*a[2];
	}
	void normalize() noexcept {
		_T R(r());
		a[0] /= R;
		a[1] /= R;
		a[2] /= R;
	}
	constexpr tPoint<_T> norm() const noexcept {
		_T R(r());
		return tPoint<_T>(a[0] / R, a[1] / R, a[2] / R);
	}
	static constexpr _T Scalar(const tPoint<_T> & left, const tPoint<_T> & right) noexcept
	{
		return (left.a[0] * right.a[0] + left.a[1] * right.a[1] + left.a[2] * right.a[2]);
	}
	static constexpr tPoint<_T> Vector(const tPoint<_T> & left, const tPoint<_T> & right) noexcept
	{
		return tPoint<_T>(left.a[1] * right.a[2] - left.a[2] * right.a[1], left.a[2] * right.a[0] - left.a[0] * right.a[2], left.a[0] * right.a[1] - left.a[1] * right.a[0]);
	}

	constexpr tPoint<_T> operator-() const noexcept
	{
		return tPoint<_T>(-a[0], -a[1], -a[2]);
	}
	constexpr tPoint<_T> operator+(const tPoint<_T> & right) const noexcept
	{
		return tPoint<_T>(a[0]+right.a[0], a[1]+right.a[1], a[2]+right.a[2]);
	}
	constexpr tPoint<_T> operator+(const _T b) const noexcept
	{
		return tPoint<_T>(a[0]+b, a[1]+b, a[2]+b);
	}
	constexpr tPoint<_T> operator-(const tPoint<_T> & right) const noexcept
	{
		return tPoint<_T>(a[0]-right.a[0], a[1]-right.a[1], a[2]-right.a[2]);
	}
	constexpr tPoint<_T> operator-(const _T b) const noexcept
	{
		return tPoint<_T>(a[0]-b, a[1]-b, a[2]-b);
	}
	constexpr tPoint<_T> operator*(const tPoint<_T> & right) const noexcept
	{
		return tPoint<_T>(a[0]*right.a[0], a[1]*right.a[1], a[2]*right.a[2]);
	}
	constexpr tPoint<_T> operator*(const _T b) const noexcept
	{
		return tPoint<_T>(a[0]*b, a[1]*b, a[2]*b);
	}
	constexpr tPoint<_T> operator/(const tPoint<_T> & right) const noexcept
	{
		return tPoint<_T>(a[0]/right.a[0], a[1]/right.a[1], a[2]/right.a[2]);
	}
	constexpr tPoint<_T> operator/(const _T b) const noexcept
	{
		return tPoint<_T>(a[0]/b, a[1]/b, a[2]/b);
	}
	tPoint<_T> & operator+=(const tPoint<_T> & right) noexcept
	{
		a[0] += right.a[0];
		a[1] += right.a[1];
		a[2] += right.a[2];
		return *this;
	}
	tPoint<_T> & operator+=(const _T right) noexcept
	{
		a[0] += right;
		a[1] += right;
		a[2] += right;
		return *this;
	}
	tPoint<_T> & operator-=(const tPoint<_T> & right) noexcept
	{
		a[0] -= right.a[0];
		a[1] -= right.a[1];
		a[2] -= right.a[2];
		return *this;
	}
	tPoint<_T> & operator-=(const _T right) noexcept
	{
		a[0] -= right;
		a[1] -= right;
		a[2] -= right;
		return *this;
	}
	tPoint<_T> & operator*=(const _T right) noexcept
	{
		a[0] *= right;
		a[1] *= right;
		a[2] *= right;
		return *this;
	}
	tPoint<_T> & operator/=(const _T right) noexcept
	{
		a[0] /= right;
		a[1] /= right;
		a[2] /= right;
		return *this;
	}
	constexpr bool operator==(const tPoint<_T> & right) const noexcept
	{
		return ((a[0] == right.a[0]) && (a[1] == right.a[1]) && (a[2] == right.a[2]));
	}
	constexpr bool operator!=(const tPoint<_T> & right) const noexcept
	{
		return ((a[0] != right.a[0]) || (a[1] != right.a[1]) || (a[2] != right.a[2]));
	}
};

template<class _T = flo>
constexpr tPoint<_T> operator+(const _T b, const tPoint<_T> & right) noexcept
{
	return tPoint<_T>(b + right.a[0], b + right.a[1], b + right.a[2]);
}

template<class _T = flo>
constexpr tPoint<_T> operator-(const _T b, const tPoint<_T> & right) noexcept
{
	return tPoint<_T>(b - right.a[0], b - right.a[1], b - right.a[2]);
}

template<class _T = flo>
constexpr tPoint<_T> operator*(const _T b, const tPoint<_T> & right) noexcept
{
	return tPoint<_T>(b*right.a[0], b*right.a[1], b*right.a[2]);
}

template<class _T = flo>
constexpr tPoint<_T> operator/(const _T b, const tPoint<_T> & right) noexcept
{
	return tPoint<_T>(b / right.a[0], b / right.a[1], b / right.a[2]);
}

template<class _T = flo>
tPoint<_T> operator*(const tMatrix<_T> & left, const tPoint<_T> & right) noexcept {
	_ASSERTE(left.sizeA() == 3 && left.sizeB() == 3);
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
