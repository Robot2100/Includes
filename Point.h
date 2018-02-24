#pragma once
#ifndef POINT_H
#define POINT_H

#include <vector>
#include "Matrix.h"
__if_not_exists (flo) {
	typedef double flo;
}

template<class _Ty = flo>
struct tPoint
{
	static_assert(std::is_floating_point<_Ty>::value,"Type \"Point\" is floating-point-based type.");

	typedef	typename _Ty value_type;

	_Ty a[3];

	constexpr tPoint() noexcept : a{ static_cast<_Ty>(0.0), static_cast<_Ty>(0.0), static_cast<_Ty>(0.0) } {}
	constexpr tPoint(_Ty _x, _Ty _y, _Ty _z) noexcept : a{ _x,_y,_z } {};

	constexpr _Ty r() const noexcept {
		return sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	}
	constexpr _Ty V() const noexcept {
		return a[0]*a[1]*a[2];
	}
	void normalize() noexcept {
		_Ty R(r());
		a[0] /= R;
		a[1] /= R;
		a[2] /= R;
	}
	constexpr tPoint<_Ty> norm() const noexcept {
		_Ty R(r());
		return tPoint<_Ty>(a[0] / R, a[1] / R, a[2] / R);
	}
	static constexpr _Ty Scalar(const tPoint<_Ty> & left, const tPoint<_Ty> & right) noexcept
	{
		return (left.a[0] * right.a[0] + left.a[1] * right.a[1] + left.a[2] * right.a[2]);
	}
	static constexpr tPoint<_Ty> Vector(const tPoint<_Ty> & left, const tPoint<_Ty> & right) noexcept
	{
		return tPoint<_Ty>(left.a[1] * right.a[2] - left.a[2] * right.a[1], left.a[2] * right.a[0] - left.a[0] * right.a[2], left.a[0] * right.a[1] - left.a[1] * right.a[0]);
	}

	constexpr tPoint<_Ty> operator-() const noexcept
	{
		return tPoint<_Ty>(-a[0], -a[1], -a[2]);
	}
	constexpr tPoint<_Ty> operator+(const tPoint<_Ty> & right) const noexcept
	{
		return tPoint<_Ty>(a[0]+right.a[0], a[1]+right.a[1], a[2]+right.a[2]);
	}
	constexpr tPoint<_Ty> operator+(const _Ty b) const noexcept
	{
		return tPoint<_Ty>(a[0]+b, a[1]+b, a[2]+b);
	}
	constexpr tPoint<_Ty> operator-(const tPoint<_Ty> & right) const noexcept
	{
		return tPoint<_Ty>(a[0]-right.a[0], a[1]-right.a[1], a[2]-right.a[2]);
	}
	constexpr tPoint<_Ty> operator-(const _Ty b) const noexcept
	{
		return tPoint<_Ty>(a[0]-b, a[1]-b, a[2]-b);
	}
	constexpr tPoint<_Ty> operator*(const tPoint<_Ty> & right) const noexcept
	{
		return tPoint<_Ty>(a[0]*right.a[0], a[1]*right.a[1], a[2]*right.a[2]);
	}
	constexpr tPoint<_Ty> operator*(const _Ty b) const noexcept
	{
		return tPoint<_Ty>(a[0]*b, a[1]*b, a[2]*b);
	}
	constexpr tPoint<_Ty> operator/(const tPoint<_Ty> & right) const noexcept
	{
		return tPoint<_Ty>(a[0]/right.a[0], a[1]/right.a[1], a[2]/right.a[2]);
	}
	constexpr tPoint<_Ty> operator/(const _Ty b) const noexcept
	{
		return tPoint<_Ty>(a[0]/b, a[1]/b, a[2]/b);
	}
	tPoint<_Ty> & operator+=(const tPoint<_Ty> & right) noexcept
	{
		a[0] += right.a[0];
		a[1] += right.a[1];
		a[2] += right.a[2];
		return *this;
	}
	tPoint<_Ty> & operator+=(const _Ty right) noexcept
	{
		a[0] += right;
		a[1] += right;
		a[2] += right;
		return *this;
	}
	tPoint<_Ty> & operator-=(const tPoint<_Ty> & right) noexcept
	{
		a[0] -= right.a[0];
		a[1] -= right.a[1];
		a[2] -= right.a[2];
		return *this;
	}
	tPoint<_Ty> & operator-=(const _Ty right) noexcept
	{
		a[0] -= right;
		a[1] -= right;
		a[2] -= right;
		return *this;
	}
	tPoint<_Ty> & operator*=(const _Ty right) noexcept
	{
		a[0] *= right;
		a[1] *= right;
		a[2] *= right;
		return *this;
	}
	tPoint<_Ty> & operator/=(const _Ty right) noexcept
	{
		a[0] /= right;
		a[1] /= right;
		a[2] /= right;
		return *this;
	}
	constexpr bool operator==(const tPoint<_Ty> & right) const noexcept
	{
		return ((a[0] == right.a[0]) && (a[1] == right.a[1]) && (a[2] == right.a[2]));
	}
	constexpr bool operator!=(const tPoint<_Ty> & right) const noexcept
	{
		return ((a[0] != right.a[0]) || (a[1] != right.a[1]) || (a[2] != right.a[2]));
	}
};

template<class _Ty = flo>
constexpr tPoint<_Ty> operator+(const _Ty b, const tPoint<_Ty> & right) noexcept
{
	return tPoint<_Ty>(b + right.a[0], b + right.a[1], b + right.a[2]);
}

template<class _Ty = flo>
constexpr tPoint<_Ty> operator-(const _Ty b, const tPoint<_Ty> & right) noexcept
{
	return tPoint<_Ty>(b - right.a[0], b - right.a[1], b - right.a[2]);
}

template<class _Ty = flo>
constexpr tPoint<_Ty> operator*(const _Ty b, const tPoint<_Ty> & right) noexcept
{
	return tPoint<_Ty>(b*right.a[0], b*right.a[1], b*right.a[2]);
}

template<class _Ty = flo>
constexpr tPoint<_Ty> operator/(const _Ty b, const tPoint<_Ty> & right) noexcept
{
	return tPoint<_Ty>(b / right.a[0], b / right.a[1], b / right.a[2]);
}

template<class _Ty = flo>
tPoint<_Ty> operator*(const tMatrix<_Ty> & left, const tPoint<_Ty> & right) noexcept {
	_ASSERTE(left.sizeA() == 3 && left.sizeB() == 3);
	tPoint<_Ty> res;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			res.a[i] += left.El(i, j) * right.a[j];
		}
	}
	return res;
}

typedef tPoint<> Point;
#endif
