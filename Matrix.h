#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <functional>
#include <array>

__if_not_exists (flo) {
	typedef double flo;
}



template<class _Ty = flo>
class tMatrix
{
	static_assert(std::is_arithmetic<_Ty>::value,"type _Ty in \"tMatrix<_Ty>\" should be arithmetic.");

public:
	typedef typename _Ty value_type;
	typedef typename std::array< std::array<_Ty, 3>, 3> array_type;
	typedef typename const std::array<std::array<_Ty, 3>, 3> const_array_type;
private:
	array_type A { { {0,0,0},{0,0,0},{0,0,0} } };
public:
	explicit tMatrix() noexcept {}
	tMatrix(const tMatrix<_Ty> & right) noexcept : A(right.A) {}
	tMatrix(tMatrix<_Ty>&& right) noexcept : A(std::move(right.A)) {}
	template<class _T2>
	explicit tMatrix(const tMatrix<_T2>& right) noexcept : A(right.A) {
		static_assert(std::is_convertible<_T2, _Ty>::value, "type _T2 should be convertible to _Ty.");
	}
	explicit tMatrix(const _Ty** input_massive) noexcept {
		for(size_t i = 0; i < 3; i++) {
			for(size_t j = 0; j < 3; j++) {
				A[i][j] = input_massive[i][j];
			}
		}
	}
	explicit tMatrix(const _Ty * input_massive) noexcept {
		for (size_t i = 0, k = 0; i < 3; i++) {
			for (size_t j = 0; j < 3; j++, k++) {
				A[i][j] = input_massive[k];
			}
		}
	}
	constexpr explicit tMatrix(const_array_type & in) noexcept : A(in) {}
	constexpr explicit tMatrix(array_type && in) noexcept : A(in) {}
	tMatrix<_Ty> & operator= (const tMatrix<_Ty> & other) noexcept {
		if (&other != this) {
			A = other.A;
		}
		return *this;
	}
	tMatrix<_Ty> & operator= (tMatrix<_Ty> && other) noexcept {
		if (&other != this) {
			A = other.A;
		}
		return *this;
	}
	inline _Ty & El(const size_t a, const size_t b) {
		return A[a][b];
	}
	inline const _Ty & El(const size_t a, const size_t b) const {
		return A[a][b];
	}
	template<class _T2, class RET = decltype(_Ty()*_T2())>
	tMatrix<RET> operator*(const tMatrix<_T2>& right) const noexcept {
		static_assert(std::is_arithmetic<_T2>::value, "type _T2 in \"tMatrix<_Ty>::operator*(const tMatrix<_T2> &)\" should be arithmetic.");
		tMatrix<RET> res;
		res.A[0][0] = A[0][0] * right.El(0, 0) + A[0][1] * right.El(1, 0) + A[0][2] * right.El(2, 0);
		res.A[0][1] = A[0][0] * right.El(0, 1) + A[0][1] * right.El(1, 1) + A[0][2] * right.El(2, 1);
		res.A[0][2] = A[0][0] * right.El(0, 2) + A[0][1] * right.El(1, 2) + A[0][2] * right.El(2, 2);
		res.A[1][0] = A[1][0] * right.El(0, 0) + A[1][1] * right.El(1, 0) + A[1][2] * right.El(2, 0);
		res.A[1][1] = A[1][0] * right.El(0, 1) + A[1][1] * right.El(1, 1) + A[1][2] * right.El(2, 1);
		res.A[1][2] = A[1][0] * right.El(0, 2) + A[1][1] * right.El(1, 2) + A[1][2] * right.El(2, 2);
		res.A[2][0] = A[2][0] * right.El(0, 0) + A[2][1] * right.El(1, 0) + A[2][2] * right.El(2, 0);
		res.A[2][1] = A[2][0] * right.El(0, 1) + A[2][1] * right.El(1, 1) + A[2][2] * right.El(2, 1);
		res.A[2][2] = A[2][0] * right.El(0, 2) + A[2][1] * right.El(1, 2) + A[2][2] * right.El(2, 2);
		return res;
	}
	template<class _T2, class RET = decltype(_Ty()*_T2())>
	std::array<RET, 3> operator*(const std::array<_T2, 3>& right) const {
		std::array<RET, 3> res;
		res[0] = A[0][0] * right[0] + A[0][1] * right[1] + A[0][2] * right[2];
		res[1] = A[1][0] * right[0] + A[1][1] * right[1] + A[1][2] * right[2];
		res[2] = A[2][0] * right[0] + A[2][1] * right[1] + A[2][2] * right[2];
		return res;
	}
	tMatrix<_Ty> operator/(const _Ty right) const noexcept {
		tMatrix<_Ty> res;
		res.A[0][0] = A[0][0] / right;
		res.A[0][1] = A[0][1] / right;
		res.A[0][2] = A[0][2] / right;
		res.A[1][0] = A[1][0] / right;
		res.A[1][1] = A[1][1] / right;
		res.A[1][2] = A[1][2] / right;
		res.A[2][0] = A[2][0] / right;
		res.A[2][1] = A[2][1] / right;
		res.A[2][2] = A[2][2] / right;
		return res;
	}
	//tMatrix<_Ty> Minor(size_t a, size_t b) const {
	//	if(a >= size_a || b >= size_b)
	//		throw std::out_of_range("tMatrix<_Ty>::Minor(int,int)");
	//	if(size_a == 1 || size_b == 1)
	//		throw std::length_error("tMatrix<_Ty>::Minor(int,int) : Cannot take a minor of size(1) matrix.");
	//	tMatrix<_Ty> res(size_a - 1, size_b - 1);
	//	for(size_t i = 0, i1 = 0; i < size_a; i++) {
	//		if(i==a) continue;
	//		for(size_t j = 0, j1 = 0; j < size_b; j++) {
	//			if(j==b) continue;
	//			res.El(i1,j1) = A[i][j];
	//			j1++;
	//		}
	//		i1++;
	//	}
	//	return std::move(res);
	//}
	tMatrix<_Ty> Transponate() const noexcept {
		tMatrix<_Ty> res;
		res.A[0][0] = A[0][0];
		res.A[0][1] = A[1][0];
		res.A[0][2] = A[2][0];
		res.A[1][0] = A[0][1];
		res.A[1][1] = A[1][1];
		res.A[1][2] = A[2][1];
		res.A[2][0] = A[0][2];
		res.A[2][1] = A[1][2];
		res.A[2][2] = A[2][2];
		return res;
	}
	tMatrix<_Ty> Invert() const {
		_Ty det = Det();
		if(det == static_cast<_Ty>(0))
			throw std::runtime_error("tMatrix<_Ty>::Invert() : Determinant equal zero.");
		tMatrix<_Ty> res;
		res.A[0][0] = (A[1][1] * A[2][2] - A[1][2] * A[2][1]) / det;
		res.A[0][1] = (A[0][2] * A[2][1] - A[0][1] * A[2][2]) / det;
		res.A[0][2] = (A[0][1] * A[1][2] - A[0][2] * A[1][1]) / det;
		res.A[1][0] = (A[1][2] * A[2][0] - A[1][0] * A[2][2]) / det;
		res.A[1][1] = (A[0][0] * A[2][2] - A[0][2] * A[2][0]) / det;
		res.A[1][2] = (A[0][2] * A[1][0] - A[0][0] * A[1][2]) / det;
		res.A[2][0] = (A[1][0] * A[2][1] - A[2][0] * A[1][1]) / det;
		res.A[2][1] = (A[0][1] * A[2][0] - A[0][0] * A[2][1]) / det;
		res.A[2][2] = (A[1][1] * A[0][0] - A[1][0] * A[0][1]) / det;
		return res;
	}
	tMatrix<_Ty> Modul() const noexcept {
		tMatrix<_Ty> res;
		res.A[0][0] = abs(A[0][0]);
		res.A[0][1] = abs(A[0][1]);
		res.A[0][2] = abs(A[0][2]);
		res.A[1][0] = abs(A[1][0]);
		res.A[1][1] = abs(A[1][1]);
		res.A[1][2] = abs(A[1][2]);
		res.A[2][0] = abs(A[2][0]);
		res.A[2][1] = abs(A[2][1]);
		res.A[2][2] = abs(A[2][2]);
		return res;
	}
	_Ty Trace() const noexcept {
		return (A[0][0] + A[1][1] + A[2][2]) / 3;
	}
	_Ty Det() const noexcept {
		return A[0][0]*A[1][1]*A[2][2] + A[0][1]*A[1][2]*A[2][0] + A[0][2]*A[1][0]*A[2][1]
			- A[0][2]*A[1][1]*A[2][0] - A[0][1]*A[1][0]*A[2][2] - A[0][0]*A[1][2]*A[2][1];
	}
}; 
template<class _Ty = flo>
constexpr tMatrix<_Ty> EqualMatrix { std::array< std::array<_Ty,3>, 3> {std::array<_Ty,3> { 0,0,0 }, std::array<_Ty,3> { 0, 0, 0}, std::array<_Ty,3> {0, 0, 0 } } };
template<class _Ty, class _T2>
tMatrix<_Ty> operator*(const _Ty left, tMatrix<_Ty> & right) noexcept {
	for (int i = 0; i < right.sizeA(); i++) {
		for (int j = 0; j < right.sizeB(); j++) {
			right.El(i,j) *= left;
		}
	}
	return right;
}
typedef tMatrix<> Matrix;
#endif
