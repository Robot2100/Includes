#pragma once
#ifndef DINMAT_H
#define DINMAT_H
#include "Matrix.h"

__if_not_exists (flo) {
	typedef double flo;
}

template<class _Ty = Matrix::value_type>
struct tDinmat
{
	static_assert(std::is_floating_point<_Ty>::value,"type \"tDinmat<_T>\" is floating-point-based type.");
	typedef typename _Ty value_type;
	typedef typename std::array<_Ty,6> array_type;
			 // 0  1  2  3  4  5
			 // x2 y2 z2 xy xz yz
	array_type U = { static_cast<_Ty>(0.0), static_cast<_Ty>(0.0), static_cast<_Ty>(0.0), 
				static_cast<_Ty>(0.0), static_cast<_Ty>(0.0), static_cast<_Ty>(0.0)};
	constexpr tDinmat() noexcept {}
	constexpr tDinmat(const tMatrix<_Ty>& in) noexcept {
		U = array_type{ in.El(0, 0), in.El(1, 1), in.El(2, 2), in.El(0, 1),in.El(0, 2),in.El(1, 2) };
	}
	constexpr explicit tDinmat(const array_type& in) noexcept(std::is_nothrow_copy_constructible<array_type>::value) : U(in) {}
	constexpr explicit tDinmat(array_type && in) noexcept(std::is_nothrow_move_constructible<array_type>::value) : U(in) {}
	constexpr tMatrix<_Ty> ToMatrix() const noexcept {
		return tMatrix<_Ty>(tMatrix<_Ty>::array_type({ tMatrix<_Ty>::array_type::value_type({U[0], U[3], U[4]}), tMatrix<_Ty>::array_type::value_type({U[3], U[1], U[5]}), tMatrix<_Ty>::array_type::value_type({U[4], U[5], U[2]}) }));
	}

	template<class T2>
	constexpr tMatrix<T2> ToMatrix() const noexcept {
		return tMatrix<T2>(tMatrix<T2>::array_type({ tMatrix<T2>::array_type::value_type({static_cast<T2>(U[0]), static_cast<T2>(U[3]), static_cast<T2>(U[4])}),
			tMatrix<T2>::array_type::value_type({static_cast<T2>(U[3]), static_cast<T2>(U[1]), static_cast<T2>(U[5])}),
			tMatrix<T2>::array_type::value_type({static_cast<T2>(U[4]), static_cast<T2>(U[5]), static_cast<T2>(U[2])}) }));
	}

};
typedef tDinmat<> Dinmat;

#endif
