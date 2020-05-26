#pragma once
#ifndef DINMAT_H
#define DINMAT_H


__if_not_exists (flo) {
	typedef double flo;
}

template<class _Ty = Matrix::value_type>
struct tDinmat
{
	static_assert(std::is_floating_point<_Ty>::value,"type \"tDinmat<_T>\" is floating-point-based type.");
	typedef typename _Ty value_type;
			 // 0  1  2  3  4  5
			 // x2 y2 z2 xy xz yz
	_Ty U[6] = { static_cast<_Ty>(0.0), static_cast<_Ty>(0.0), static_cast<_Ty>(0.0), 
				static_cast<_Ty>(0.0), static_cast<_Ty>(0.0), static_cast<_Ty>(0.0)};
	tDinmat() noexcept {}
	tDinmat(const tMatrix<_Ty> & in) {
		U[0] = in.El(0, 0);
		U[1] = in.El(1, 1);
		U[2] = in.El(2, 2);
		U[3] = in.El(0, 1);
		U[4] = in.El(0, 2);
		U[5] = in.El(1, 2);
	}
	tMatrix<_Ty> ToMatrix() const noexcept {
		_Ty temp[9] = {U[0], U[3], U[4], U[3], U[1], U[5], U[4], U[5], U[2]};
		return tMatrix<_Ty>(temp);
	}
};
typedef tDinmat<> Dinmat;

#endif
