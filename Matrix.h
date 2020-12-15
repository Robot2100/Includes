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
	static constexpr bool _CopyNothrow = std::is_nothrow_copy_constructible<value_type>::value;
	static constexpr bool _MoveNothrow = std::is_nothrow_move_constructible<value_type>::value;
protected:
	array_type A{ { {0,0,0},{0,0,0},{0,0,0} } };
public:
	explicit constexpr tMatrix() noexcept {}
	template<class _T2>	constexpr tMatrix(const tMatrix<_T2>& r) noexcept(_CopyNothrow) {
		if constexpr (std::is_same<_Ty, _T2>::value == true) {
			if (this != &r) return;
			A = r.A;
		}
		else {
			static_assert(std::is_convertible<_T2, _Ty>::value, "type _T2 should be convertible to _Ty.");
			A[0][0] = static_cast<_Ty>(r.A[0][0]);
			A[0][1] = static_cast<_Ty>(r.A[0][1]);
			A[0][2] = static_cast<_Ty>(r.A[0][2]);
			A[1][0] = static_cast<_Ty>(r.A[1][0]);
			A[1][1] = static_cast<_Ty>(r.A[1][1]);
			A[1][2] = static_cast<_Ty>(r.A[1][2]);
			A[2][0] = static_cast<_Ty>(r.A[2][0]);
			A[2][1] = static_cast<_Ty>(r.A[2][1]);
			A[2][2] = static_cast<_Ty>(r.A[2][2]);
		}
	}
	constexpr tMatrix(const tMatrix<_Ty>& r) noexcept(_CopyNothrow) {
		if (this != &r) return;
		A = r.A;
	}
	template<class _T2>	constexpr tMatrix(tMatrix<_T2>&& r) noexcept(_MoveNothrow) {
		if constexpr (std::is_same<_Ty, _T2>::value == true) {
			if (this != &r) return;
			A = std::move(r.A);
		}
		else {
			static_assert(std::is_convertible<_T2, _Ty>::value, "type _T2 should be convertible to _Ty.");
			A[0][0] = static_cast<_Ty &&>(r.A[0][0]);
			A[0][1] = static_cast<_Ty &&>(r.A[0][1]);
			A[0][2] = static_cast<_Ty &&>(r.A[0][2]);
			A[1][0] = static_cast<_Ty &&>(r.A[1][0]);
			A[1][1] = static_cast<_Ty &&>(r.A[1][1]);
			A[1][2] = static_cast<_Ty &&>(r.A[1][2]);
			A[2][0] = static_cast<_Ty &&>(r.A[2][0]);
			A[2][1] = static_cast<_Ty &&>(r.A[2][1]);
			A[2][2] = static_cast<_Ty &&>(r.A[2][2]);
		}
	}
	constexpr tMatrix(tMatrix<_Ty>&& r) noexcept(_MoveNothrow) {
		if (this != &r) return;
		A = std::move(r.A);
	}
	constexpr tMatrix(const _Ty v) noexcept : A{ { {v,0,0},{0,v,0},{0,0,v} } } {}
	explicit constexpr tMatrix(const _Ty** input_massive) noexcept {
		for (size_t i = 0; i < 3; i++) {
			for (size_t j = 0; j < 3; j++) {
				A[i][j] = input_massive[i][j];
			}
		}
	}
	explicit constexpr tMatrix(const _Ty * input_massive) noexcept {
		for (size_t i = 0, k = 0; i < 3; i++) {
			for (size_t j = 0; j < 3; j++, k++) {
				A[i][j] = input_massive[k];
			}
		}
	}
	explicit constexpr tMatrix(const_array_type & in) noexcept : A(in) {}
	explicit constexpr tMatrix(array_type && in) noexcept : A(in) {}
	constexpr tMatrix<_Ty> & operator= (const tMatrix<_Ty> & other) noexcept(_CopyNothrow) {
		if (&other != this) {
			A = other.A;
		}
		return *this;
	}
	constexpr tMatrix<_Ty> & operator= (tMatrix<_Ty> && other) noexcept(_MoveNothrow) {
		if (&other != this) {
			A = std::move(other.A);
		}
		return *this;
	}
	[[nodiscard]] inline constexpr _Ty & El(const size_t &a, const size_t &b) noexcept {
		return A[a][b];
	}
	[[nodiscard]] inline constexpr _Ty El(const size_t &a, const size_t &b) const noexcept {
		return A[a][b];
	}
	template<class _T2>
	[[nodiscard]] constexpr tMatrix<decltype(_Ty()* _T2())> operator*(const tMatrix<_T2>& right) const noexcept {
		using resv = decltype(_Ty()* _T2());
		std::array<resv, 3> a1 = {	A[0][0] * right.A[0][0] + A[0][1] * right.A[1][0] + A[0][2] * right.A[2][0],
									A[0][0] * right.A[0][1] + A[0][1] * right.A[1][1] + A[0][2] * right.A[2][1],
									A[0][0] * right.A[0][2] + A[0][1] * right.A[1][2] + A[0][2] * right.A[2][2]};
		std::array<resv, 3> a2 = {	A[1][0] * right.A[0][0] + A[1][1] * right.A[1][0] + A[1][2] * right.A[2][0],
									A[1][0] * right.A[0][1] + A[1][1] * right.A[1][1] + A[1][2] * right.A[2][1],
									A[1][0] * right.A[0][2] + A[1][1] * right.A[1][2] + A[1][2] * right.A[2][2]};
		std::array<resv, 3> a3 = {	A[2][0] * right.A[0][0] + A[2][1] * right.A[1][0] + A[2][2] * right.A[2][0],
									A[2][0] * right.A[0][1] + A[2][1] * right.A[1][1] + A[2][2] * right.A[2][1],
									A[2][0] * right.A[0][2] + A[2][1] * right.A[1][2] + A[2][2] * right.A[2][2]};
		std::array<std::array<resv, 3>, 3> b{ a1,a2,a3 };
		return tMatrix<resv>(b);
	}
	template<class _T2>
	[[nodiscard]] constexpr std::array<decltype(_Ty()* _T2()), 3> operator*(const std::array<_T2, 3>& right) const {
		std::array<decltype(_Ty()* _T2()), 3> res;
		res[0] = A[0][0] * right[0] + A[0][1] * right[1] + A[0][2] * right[2];
		res[1] = A[1][0] * right[0] + A[1][1] * right[1] + A[1][2] * right[2];
		res[2] = A[2][0] * right[0] + A[2][1] * right[1] + A[2][2] * right[2];
		return res;
	}
	template<class _T2>
	[[nodiscard]] constexpr tMatrix<decltype(_Ty() /_T2())> operator/(const _T2 right) const noexcept {

		std::array<_Ty, 3> a1 = { A[0][0] / right, A[0][1] / right, A[0][2] / right };
		std::array<_Ty, 3> a2 = { A[1][0] / right, A[1][1] / right, A[1][2] / right };
		std::array<_Ty, 3> a3 = { A[2][0] / right, A[2][1] / right, A[2][2] / right };
		std::array<std::array<_Ty, 3>, 3> b{ a1,a2,a3 };
		return tMatrix<_Ty>(b);
	}
	[[nodiscard]] constexpr tMatrix<_Ty> Transponate() const noexcept {
		std::array<_Ty, 3> a1 = { A[0][0],A[1][0],A[2][0] };
		std::array<_Ty, 3> a2 = { A[0][1],A[1][1],A[2][1] };
		std::array<_Ty, 3> a3 = { A[0][2],A[1][2],A[2][2] };
		std::array<std::array<_Ty, 3>, 3> b{ a1,a2,a3 };
		return tMatrix<_Ty>(b);
	}
	[[nodiscard]] constexpr tMatrix<_Ty> Invert() const noexcept {
		_Ty det = Det();
		std::array<_Ty, 3> a1 = {(A[1][1] * A[2][2] - A[1][2] * A[2][1]) / det, (A[0][2] * A[2][1] - A[0][1] * A[2][2]) / det, (A[0][1] * A[1][2] - A[0][2] * A[1][1]) / det};
		std::array<_Ty, 3> a2 = {(A[1][2] * A[2][0] - A[1][0] * A[2][2]) / det, (A[0][0] * A[2][2] - A[0][2] * A[2][0]) / det, (A[0][2] * A[1][0] - A[0][0] * A[1][2]) / det};
		std::array<_Ty, 3> a3 = {(A[1][0] * A[2][1] - A[2][0] * A[1][1]) / det, (A[0][1] * A[2][0] - A[0][0] * A[2][1]) / det, (A[1][1] * A[0][0] - A[1][0] * A[0][1]) / det};
		std::array<std::array<_Ty, 3>, 3> b = { a1,a2,a3 };
		return tMatrix<_Ty>(b);
	}
	[[nodiscard]] constexpr tMatrix<_Ty> Modul() const noexcept {
		constexpr _Ty zero(0);
		std::array<_Ty, 3> a1 = { A[0][0] < zero ? A[0][0] : -A[0][0], A[0][1] < zero ? A[0][1] : -A[0][1], A[0][2] < zero ? A[0][2] : -A[0][2] };
		std::array<_Ty, 3> a2 = { A[1][0] < zero ? A[1][0] : -A[1][0], A[1][1] < zero ? A[1][1] : -A[1][1], A[1][2] < zero ? A[1][2] : -A[1][2] };
		std::array<_Ty, 3> a3 = { A[2][0] < zero ? A[2][0] : -A[2][0], A[2][1] < zero ? A[2][1] : -A[2][1], A[2][2] < zero ? A[2][2] : -A[2][2] };
		std::array<std::array<_Ty, 3>, 3> b{ a1,a2,a3 };
		return tMatrix<_Ty>(b);
	}
	constexpr double Trace() const noexcept {
		return (A[0][0] + A[1][1] + A[2][2]) / 3.0;
	}
	constexpr _Ty Det() const noexcept {
		return A[0][0]*A[1][1]*A[2][2] + A[0][1]*A[1][2]*A[2][0] + A[0][2]*A[1][0]*A[2][1]
			- A[0][2]*A[1][1]*A[2][0] - A[0][1]*A[1][0]*A[2][2] - A[0][0]*A[1][2]*A[2][1];
	}
	template <class _T2> friend class tMatrix;
};

template<class _Ty, class _T2>
[[nodiscard]] constexpr auto operator*(const _T2 left, const tMatrix<_Ty>& right) noexcept -> tMatrix<std::remove_reference<decltype(left* (std::declval<_Ty>()))> > {
	using resv = decltype(left* (right.El(0, 0)));
	std::array<resv, 3> a1 = { left * right.El(0,0) , left * right.El(0,1), left * right.El(0,2) };
	std::array<resv, 3> a2 = { left * right.El(1,0) , left * right.El(1,1), left * right.El(1,2) };
	std::array<resv, 3> a3 = { left * right.El(2,0) , left * right.El(2,1), left * right.El(2,2) };
	std::array<std::array<resv, 3>, 3> b{ a1,a2,a3 };
	return tMatrix<resv>(b);
}
typedef tMatrix<> Matrix;

template<class _Ty>
constexpr tMatrix<_Ty> EqualMatrix(static_cast<_Ty>(1));

#endif
