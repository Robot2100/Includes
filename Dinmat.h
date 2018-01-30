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
			 // x2 y2 z2 xy xz yz
	_T U[6] = { static_cast<flo>(0.0), static_cast<flo>(0.0), static_cast<flo>(0.0), static_cast<flo>(0.0), static_cast<flo>(0.0), static_cast<flo>(0.0)};
	tDinmat() noexcept {}
	tDinmat(const tMatrix<_T> & in) {
		_ASSERTE(in.sizeA() == 3 && in.sizeB() == 3);
		U[0] = in.El(0, 0);
		U[1] = in.El(1, 1);
		U[2] = in.El(2, 2);
		U[3] = in.El(0, 1);
		U[4] = in.El(0, 2);
		U[5] = in.El(1, 2);
	}
	tMatrix<_T> ToMatrix() const noexcept {
		_T temp[9] = {U[0], U[3], U[4], U[3], U[1], U[5], U[4], U[5], U[2]};
		return tMatrix<_T>(temp, 3, 3);
	}
};
typedef tDinmat<> Dinmat;

#endif
