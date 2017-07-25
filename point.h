#pragma once
#ifndef POINT_H
#define POINT_H

__if_not_exists (flo) {
	typedef float flo;
}

template<class _T = flo>
struct tPoint
{
	static_assert(std::is_floating_point<_T>::value,"type 'Point' is floating-point-based type.");
	_T x;
	_T y;
	_T z;
	_T r() const
	{
		return sqrt(x*x + y*y + z*z);
	}
	tPoint() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	tPoint(_T _x, _T _y, _T _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	tPoint<_T> operator-(tPoint<_T> & a)
	{
		Point r;
		r.x = -a.x + x;
		r.y = -a.y + y;
		r.z = -a.z + z;
		return r;
	}
	tPoint<_T> operator+(tPoint<_T> & a)
	{
		Point r(*this);
		r.x = a.x + x;
		r.y = a.y + y;
		r.z = a.z + z;
		return r;
	}
	tPoint<_T> operator*(const tPoint<_T> & a)
	{
		Point r(*this);
		r.x *= a.x;
		r.y *= a.y;
		r.z *= a.z;
		return r;
	}
	tPoint<_T> operator*(const _T & a)
	{
		Point r(*this);
		r.x *= a;
		r.y *= a;
		r.z *= a;
		return r;
	}
	tPoint<_T> operator*(_T && a)
	{
		Point r(*this);
		r.x *= a;
		r.y *= a;
		r.z *= a;
		return r;
	}
	tPoint<_T> & operator+=(const tPoint<_T> & a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	tPoint<_T> & operator-=(const tPoint<_T> & a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}
	bool operator!=(const tPoint<_T> & a)
	{
		return (a.x != x) || (a.y != y) || (a.z != z);
	}
	bool operator==(const tPoint<_T> & a)
	{
		return !((a.x != x) || (a.y != y) || (a.z != z));
	}
	void normalize()
	{
		_T R = r();
		x /= R;
		y /= R;
		z /= R;
	}
	static tPoint<_T> corrf(tPoint<_T> & p1, tPoint<_T> & p2) {
		return p1*p2;
	}
};

typedef tPoint<> Point;
#endif