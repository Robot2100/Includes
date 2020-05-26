#pragma once
#ifndef CELL_H
#define CELL_H
#include "Matrix.h"

template<class _Ty = Matrix::value_type>
struct tCell
{
private:
	static constexpr _Ty GradtoRad = 0.0174532925199432957692;
	static constexpr _Ty RadtoGrad = 57.295779513082320877;
	static constexpr bool _MoveNothrow = std::is_nothrow_move_constructible< tMatrix<_Ty> >::value;
protected:
	_Ty lat_dir[3];
	_Ty angle_rad[3];
	_Ty angle_grad[3];
	tMatrix<_Ty> fracToCart, cartToFrac;
	void CreateFromFracToCart(const tMatrix<_Ty> & Mat) {
		fracToCart = Mat;
		cartToFrac = fracToCart.Invert();
		TakeCellFromFracToCart();
	}
	void TakeCellFromFracToCart() noexcept {
		lat_dir[0] = fracToCart.El(0,0);
		lat_dir[1] = sqrt((fracToCart.El(1,1)*fracToCart.El(1,1))+(fracToCart.El(0,1)*fracToCart.El(0,1)));
		_Ty CosG = fracToCart.El(0,1) / lat_dir[1]; //CosG
		_Ty SinG = sqrt(1 - (CosG*CosG)); //SinG
		angle_rad[2] = acos(CosG);
		_Ty tem2 = fracToCart.El(0, 2);
		_Ty temp = tem2 * CosG + fracToCart.El(1,2) * SinG;
		_Ty tem1 = fracToCart.El(2, 2) * SinG;
		lat_dir[2] = sqrt((tem1* tem1) + (temp*temp) - 2 * CosG*temp*tem2 + (tem2*tem2)) / SinG;
		angle_rad[1] = acos(fracToCart.El(0,2) / lat_dir[2]);
		angle_rad[0] = acos(temp/lat_dir[2]);
		for(int i = 0; i < 3; i++)
			angle_grad[i] = angle_rad[i] * RadtoGrad;
	}
	void CreateFromCartToFrac(const tMatrix<_Ty> & Mat) {
		cartToFrac = Mat;
		fracToCart = cartToFrac.Invert();
		TakeCellFromFracToCart();
	}
	void CreateMatrix() 
	{
		_Ty cos0 = cos(angle_rad[0]);
		_Ty cos1 = cos(angle_rad[1]);
		_Ty cos2 = cos(angle_rad[2]);
		_Ty sin2 = sin(angle_rad[2]);
		fracToCart.El(0, 0) = lat_dir[0];
		fracToCart.El(1, 0) = lat_dir[1] * cos2;
		fracToCart.El(1, 1) = lat_dir[1] * sin2;
		fracToCart.El(2, 0) = lat_dir[2] * cos1;
		fracToCart.El(1, 2) = lat_dir[2] * (cos0 - cos1 * cos2) / sin2;
		fracToCart.El(2, 2) = lat_dir[2] * sqrt(sin2*sin2 - cos0*cos0 - cos1*cos1 + 2 * cos0* cos1* cos2)/sin2;
		cartToFrac = fracToCart.Invert();
	}
public:
	typedef typename _Ty value_type;
	explicit tCell(const tMatrix<_Ty> & Mat, const bool is_FracToCart = true) noexcept {
		Create(Mat, is_FracToCart);
	}
	explicit tCell(const _Ty a = 10, const _Ty b = 10, const _Ty c = 10, const _Ty alpha = 90, const _Ty beta = 90, const _Ty gamma = 90, const bool is_grad = true) {
		Create(a, b, c, alpha, beta, gamma, is_grad);
	}
	tCell(tCell<_Ty> && right) noexcept(_MoveNothrow) : fracToCart(std::move(right.fracToCart)), cartToFrac(std::move(right.cartToFrac)),
		lat_dir{ right.lat_dir[0],right.lat_dir[1],right.lat_dir[2] },
		angle_rad{ right.angle_rad[0],right.angle_rad[1],right.angle_rad[2] },
		angle_grad{ right.angle_grad[0],right.angle_grad[1],right.angle_grad[2] } {}
	tCell<_Ty> & operator= (tCell<_Ty> && other) noexcept {
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
	tCell<_Ty> & operator= (tCell<_Ty> & other) noexcept {
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
	const _Ty & Lat_dir(const int i) const noexcept {
		return lat_dir[i];
	}
	void Create(const tMatrix<_Ty> & Mat, const bool is_FracToCart = true) noexcept {
		if (is_FracToCart) CreateFromFracToCart(Mat);
		else CreateFromCartToFrac(Mat);
	}
	void Create(const _Ty a = 10, const _Ty b = 10, const _Ty c = 10, const _Ty alpha = 90, const _Ty beta = 90, const _Ty gamma = 90, const bool is_grad = true) {
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
		CreateMatrix();
	}

	const _Ty & Angle_rad(const int i) const noexcept {
		return angle_rad[i];
	}
	const _Ty & Angle_grad(const int i) const noexcept {
		return angle_grad[i];
	}
	const tMatrix<_Ty> & FracToCart() const noexcept {
		return fracToCart;
	}
	const tMatrix<_Ty> & CartToFrac() const noexcept {
		return cartToFrac;
	}
};
typedef tCell<> Cell;

#endif
