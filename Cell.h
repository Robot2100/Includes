#pragma once
#ifndef CELL_H
#define CELL_H
#include "Matrix.h"
#include "Cell.h"
__if_not_exists (flo) {
	typedef float flo;
}

template<class _T = flo>
struct tCell
{
private:
	const _T Pi2 = (_T) 1.57079632679489661923;
	const _T RadtoGrad = (_T)(90 / Pi2);
	const _T GradtoRad = (_T)(Pi2 / 90);
	_T lat_dir[3] = {0, 0, 0};
	_T angle_rad[3] = {Pi2, Pi2, Pi2};
	_T angle_grad[3] = {90, 90, 90};
	tMatrix<_T> fracToCart, cartToFrac;
	inline _T _quad(_T a) {
		return a*a;
	}
	void CreateFromFracToCart(const tMatrix<_T> & Mat) {
		fracToCart = Mat;
		cartToFrac = fracToCart.Invert();
		TakeCellFromFracToCart();
	}
	void TakeCellFromFracToCart() {
		lat_dir[0] = fracToCart.El(0,0);
		lat_dir[1] = sqrt((fracToCart.El(1,1)*fracToCart.El(1,1))+(fracToCart.El(0,1)*fracToCart.El(0,1)));
		_T CosG = fracToCart.El(0,1) / lat_dir[1]; //CosG
		_T SinG = sqrt(1 - (CosG*CosG)); //SinG
		angle_rad[2] = acos(CosG);
		_T temp = fracToCart.El(0,2) * CosG + fracToCart.El(1,2) * SinG;
		lat_dir[2] = sqrt(_quad(fracToCart.El(2,2) * SinG) + _quad(temp) - 2 * CosG*temp*fracToCart.El(0,2) + _quad(fracToCart.El(0,2))) / SinG;
		angle_rad[1] = acos(fracToCart.El(0,2) / lat_dir[2]);
		angle_rad[0] = acos(temp/lat_dir[2]);
		for(int i = 0; i < 3; i++)
			angle_grad[i] = angle_rad[i] * RadtoGrad;
	}
	void CreateFromCartToFrac(const tMatrix<_T> & Mat) {
		cartToFrac = Mat;
		fracToCart = cartToFrac.Invert();
		TakeCellFromFracToCart();
	}
public:
	explicit tCell(const tMatrix<_T> & Mat, const bool is_FracToCart = true) {
		if(is_FracToCart) CreateFromFracToCart(Mat);
		else CreateFromCartToFrac(Mat);
	}
	explicit tCell(const _T a = 10, const _T b = 10, const _T c = 10, const _T alpha = 90, const _T beta = 90, const _T gamma = 90, const bool is_grad = true) {
		lat_dir[0](a);
		lat_dir[1](b);
		lat_dir[2](c);
		if (is_grad) {
			angle_grad[0] = alpha;
			angle_grad[1] = beta;
			angle_grad[2] = gamma;
			for (int i = 0; i < 3; i++) {
				angle_rad[i] = angle_grad[i] * GradtoRad;
			}
		}
		else {
			angle_rad[0] = alpha;
			angle_rad[1] = beta;
			angle_rad[2] = gamma;
			for (int i = 0; i < 3; i++) {
				angle_grad[i] = angle_rad[i] * RadtoGrad;
			}
		}
	}
	const _T & Lat_dir(const int i) const{
		return lat_dir[i];
	}
	const _T & Angle_rad(const int i) const{
		return angle_rad[i];
	}
	const _T & Angle_grad(const int i) const{
		return angle_grad[i];
	}
	const tMatrix<_T> & FracToCart() const {
		return fracToCart;
	}
	const tMatrix<_T> & CartToFrac() const {
		return cartToFrac;
	}
};
typedef tCell<> Cell;


#endif
