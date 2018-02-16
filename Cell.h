#pragma once
#ifndef CELL_H
#define CELL_H
#include "Matrix.h"
__if_not_exists (flo) {
	typedef double flo;
}

#ifndef _MATH_DEFINES_DEFINED
#define M_PI_2        (1.57079632679489661923)
#endif // _MATH_DEFINES_DEFINED
#define GradtoRad     (0.0174532925199432957692)
#define RadtoGrad     (57.295779513082320877)
template<class _T = flo>
struct tCell
{
private:
	static constexpr bool _MoveNothrow = std::is_nothrow_move_constructible< tMatrix<_T> >::value;
protected:
	_T lat_dir[3];
	_T angle_rad[3];
	_T angle_grad[3];
	tMatrix<_T> fracToCart, cartToFrac;
	void CreateFromFracToCart(const tMatrix<_T> & Mat) {
		fracToCart = Mat;
		cartToFrac = fracToCart.Invert();
		TakeCellFromFracToCart();
	}
	void TakeCellFromFracToCart() noexcept {
		lat_dir[0] = fracToCart.El(0,0);
		lat_dir[1] = sqrt((fracToCart.El(1,1)*fracToCart.El(1,1))+(fracToCart.El(0,1)*fracToCart.El(0,1)));
		_T CosG = fracToCart.El(0,1) / lat_dir[1]; //CosG
		_T SinG = sqrt(1 - (CosG*CosG)); //SinG
		angle_rad[2] = acos(CosG);
		_T tem2 = fracToCart.El(0, 2);
		_T temp = tem2 * CosG + fracToCart.El(1,2) * SinG;
		_T tem1 = fracToCart.El(2, 2) * SinG;
		lat_dir[2] = sqrt((tem1* tem1) + (temp*temp) - 2 * CosG*temp*tem2 + (tem2*tem2)) / SinG;
		angle_rad[1] = acos(fracToCart.El(0,2) / lat_dir[2]);
		angle_rad[0] = acos(temp/lat_dir[2]);
		for(int i = 0; i < 3; i++)
			angle_grad[i] = angle_rad[i] * (_T)RadtoGrad;
	}
	void CreateFromCartToFrac(const tMatrix<_T> & Mat) {
		cartToFrac = Mat;
		fracToCart = cartToFrac.Invert();
		TakeCellFromFracToCart();
	}
	void CreateMatrix() 
	{
		_T cos0 = cos(angle_rad[0]);
		_T cos1 = cos(angle_rad[1]);
		_T cos2 = cos(angle_rad[2]);
		fracToCart.El(0, 0) = lat_dir[0];
		fracToCart.El(0, 1) = lat_dir[1] * cos2;
		fracToCart.El(1, 1) = lat_dir[1] * sin(angle_rad[2]);
		fracToCart.El(0, 2) = lat_dir[2] * cos1;
		fracToCart.El(1, 2) = (lat_dir[2] - cos1)*cos1;
		fracToCart.El(2, 2) = lat_dir[2] / sqrt(1 - cos0*cos0 - cos1*cos1 - cos2*cos2 + 2 * cos0* cos1* cos2);
		cartToFrac = fracToCart.Invert();
	}
public:
	explicit tCell(const tMatrix<_T> & Mat, const bool is_FracToCart = true) noexcept {
		Create(Mat, is_FracToCart);
	}
	explicit tCell(const _T a = 10, const _T b = 10, const _T c = 10, const _T alpha = 90, const _T beta = 90, const _T gamma = 90, const bool is_grad = true) {
		Create(a, b, c, alpha, beta, gamma);
	}
	tCell(tCell<_T> && right) noexcept(_MoveNothrow) : fracToCart(std::move(right.fracToCart)), cartToFrac(std::move(right.cartToFrac)) {
		for (int i = 0; i < 3; i++) {
			lat_dir[i] = right.lat_dir[i];
			angle_rad[i] = right.angle_rad[i];
			angle_grad[i] = right.angle_grad[i];
		}
	}
	tCell<_T> & operator= (tCell<_T> && other) noexcept {
		if (&other != this) {
			fracToCart = std::move(other.fracToCart);
			cartToFrac = std::move(other.cartToFrac);
			for (int i = 0; i < 3; i++) {
				lat_dir[i] = other.lat_dir[i];
				angle_rad[i] = other.angle_rad[i];
				angle_grad[i] = other.angle_grad[i];
			}
		}
		return *this;
	}
	tCell<_T> & operator= (tCell<_T> & other) noexcept {
		if (&other != this) {
			fracToCart = (other.fracToCart);
			cartToFrac = (other.cartToFrac);
			for (int i = 0; i < 3; i++) {
				lat_dir[i] = other.lat_dir[i];
				angle_rad[i] = other.angle_rad[i];
				angle_grad[i] = other.angle_grad[i];
			}
		}
		return *this;
	}
	const _T & Lat_dir(const int i) const noexcept {
		return lat_dir[i];
	}
	void Create(const tMatrix<_T> & Mat, const bool is_FracToCart = true) noexcept {
		if (is_FracToCart) CreateFromFracToCart(Mat);
		else CreateFromCartToFrac(Mat);
	}
	void Create(const _T a = 10, const _T b = 10, const _T c = 10, const _T alpha = 90, const _T beta = 90, const _T gamma = 90, const bool is_grad = true) {
		if (a <= 0 || b <= 0 || c <= 0)
			throw std::invalid_argument("");
		lat_dir[0] = a;
		lat_dir[1] = b;
		lat_dir[2] = c;
		if (is_grad) {
			angle_grad[0] = alpha;
			angle_grad[1] = beta;
			angle_grad[2] = gamma;
			for (int i = 0; i < 3; i++) {
				angle_rad[i] = angle_grad[i] * (_T)GradtoRad;
			}
		}
		else {
			angle_rad[0] = alpha;
			angle_rad[1] = beta;
			angle_rad[2] = gamma;
			for (int i = 0; i < 3; i++) {
				angle_grad[i] = angle_rad[i] * (_T)RadtoGrad;
			}
		}
		CreateMatrix();
	}

	const _T & Angle_rad(const int i) const noexcept {
		return angle_rad[i];
	}
	const _T & Angle_grad(const int i) const noexcept {
		return angle_grad[i];
	}
	const tMatrix<_T> & FracToCart() const noexcept {
		return fracToCart;
	}
	const tMatrix<_T> & CartToFrac() const noexcept {
		return cartToFrac;
	}
};
typedef tCell<> Cell;

#ifndef _MATH_DEFINES_DEFINED
#undef M_PI_2        
#endif // _MATH_DEFINES_DEFINED
#undef GradtoRad 
#undef RadtoGrad 

#endif
