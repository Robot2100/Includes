#pragma once
#ifndef DINMAT_H
#define DINMAT_H

#include "Error.h"

__if_not_exists (flo) {
	typedef float flo;
}

template<class _T = flo>
struct tDinmat
{
	static_assert(std::is_floating_point<_T>::value,"type \"tDinmat<_T>\" is floating-point-based type.");
			 // 0  1  2  3  4  5
	_T U[6]; // x2 y2 z2 xy xz yz
	tDinmat() noexcept {
		for (int i = 0; i<6; i++)
			U[i] = 0;
	}
	tDinmat(const tMatrix<_T> & in) {
		if(in.size_a != 3 in.size_b !=3)
			throw std::logic_error("tDinmat<_T>::tDinmat<_T>(tMatrix<_T>) : Size of parameter is not 3.");
		U[0] = in.A[0][0];
		U[1] = in.A[1][1];
		U[2] = in.A[2][2];
		U[3] = in.A[0][1];
		U[4] = in.A[0][2];
		U[5] = in.A[1][2];
	}
	tMatrix<_T> ToMatrix() const noexcept{
		_T temp[3][3] = {{U[0], U[3], U[4]}, {U[3], U[1], U[5]}, {U[4], U[5], U[2]}};
		return tMatrix<_T>(temp, 3, 3);
	}
};
typedef tDinmat<> Dinmat;

#endif
