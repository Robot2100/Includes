#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

__if_not_exists (flo) {
	typedef double flo;
}

template<class _Ty = flo>
class tMatrix
{
	static_assert(std::is_floating_point<_Ty>::value,"type \"tMatrix<_Ty>\" is floating-point-based type.");
public:
	typedef typename _Ty value_type;
	typedef typename std::vector<std::vector<_Ty> > vector_type;
private:
	size_t size_a, size_b;
	vector_type A;
	void resize(const size_t a, const size_t b) noexcept {
		_ASSERTE(a != 0 && b != 0);
		size_a = a;
		size_b = b;
		A.resize(a, std::vector<_Ty>(b, static_cast<_Ty>(0.0f)));
	}
	void resizeU(const size_t a, const size_t b) noexcept {
		_ASSERTE(a != 0 && b != 0);
		size_a = a;
		size_b = b;
		A.resize(a, std::vector<_Ty>(b));
	}
public:
	explicit tMatrix(const size_t a = 3, const size_t b = 3, bool empty = false) noexcept {
		if (empty == true) resizeU(a, b);
		else resize(a, b);
	}
	tMatrix(const tMatrix<_Ty> & right) noexcept : A(right.A), size_a(right.size_a), size_b(right.size_b) {}
	tMatrix(tMatrix<_Ty> && right) noexcept : A(std::move(right.A)), size_a(right.size_a), size_b(right.size_b) {}
	explicit tMatrix(const _Ty** input_massive, const size_t a = 3, const size_t b = 3) noexcept {
		resizeU(a,b);
		for(size_t i = 0; i < a; i++) {
			for(size_t j = 0; j < b; j++) {
				A[i][j] = input_massive[i][j];
			}
		}
	}
	explicit tMatrix(const _Ty * input_massive, const size_t a = 3, const size_t b = 3) noexcept {
		resizeU(a, b);
		for (size_t i = 0; i < a; i++) {
			for (size_t j = 0; j < b; j++) {
				A[i][j] = input_massive[i*a + j];
			}
		}
	}
	explicit tMatrix(const vector_type & in) noexcept : A(in), size_a(A.size()), size_b(A[0].size()) {}
	explicit tMatrix(vector_type && in) noexcept : A(in), size_a(A.size()), size_b(A[0].size()) {}
	tMatrix<_Ty> & operator= (const tMatrix<_Ty> & other) noexcept {
		if (&other != this) {
			A = other.A;
			size_a = other.size_a;
			size_b = other.size_b;
		}
		return *this;
	}
	tMatrix<_Ty> & operator= (tMatrix<_Ty> && other) noexcept {
		if (&other != this) {
			A = std::move(other.A);
			size_a = other.size_a;
			size_b = other.size_b;
		}
		return *this;
	}
	inline const int sizeA() const noexcept {
		return size_a;
	}
	inline const int sizeB() const noexcept {
		return size_b;
	}
	inline _Ty & El(const size_t a, const size_t b) {
		return A[a][b];
	}
	inline const _Ty & El(const size_t a, const size_t b) const {
		return A[a][b];
	}
	tMatrix<_Ty> operator*(const tMatrix<_Ty> & right) const {
		if(this->size_b != right.size_a)
			throw std::length_error("tMatrix<_Ty>::operator*(tMatrix<_Ty>) : Matrixes can't be multiplied (by sizes).");
		const size_t & sa = this->size_a;
		const size_t & sb = right.size_b;
		const size_t & sm = this->size_b;
		tMatrix<_Ty> res(sa,sb);
		for(size_t i = 0; i < sa; i++) {
			for(size_t j = 0; j < sb; j++) {
				_Ty sum = (_Ty)0;
				for(size_t k = 0; k < sm; k++) {
					sum += this->El(i,k) * right.El(k,j);
				}
				res.El(i,j) = sum;
			}
		}
		return res;
	}
	std::vector<_Ty> operator*(const std::vector<_Ty> & right) const {
		if(this->size_b != right.size())
			throw std::length_error("tMatrix<_Ty>::operator*(tMatrix<_Ty>,std::vector<_Ty>) : Wrong size of matrix or vector.");
		std::vector<_Ty> res(this->size_a, 0);
		for(int i = 0; i < this->size_a; i++) {
			for(int j = 0; j < this->size_b; j++) {
				res[i] += this->A[i][j]*right[j];
			}
		}
		return res;
	}
	tMatrix<_Ty> operator/(const _Ty right) const noexcept{
		tMatrix<_Ty> left(*this);
		for(int i = 0; i < this->size_a; i++) {
			for(int j = 0; j < this->size_b; j++) {
				left.A[i][j]/=right;
			}
		}
		return left;
	}
	tMatrix<_Ty> Minor(size_t a, size_t b) const {
		if(a >= size_a || b >= size_b)
			throw std::out_of_range("tMatrix<_Ty>::Minor(int,int)");
		if(size_a == 1 || size_b == 1)
			throw std::length_error("tMatrix<_Ty>::Minor(int,int) : Cannot take a minor of size(1) matrix.");
		tMatrix<_Ty> res(size_a - 1, size_b - 1);
		for(size_t i = 0, i1 = 0; i < size_a; i++) {
			if(i==a) continue;
			for(size_t j = 0, j1 = 0; j < size_b; j++) {
				if(j==b) continue;
				res.El(i1,j1) = A[i][j];
				j1++;
			}
			i1++;
		}
		return std::move(res);
	}
	tMatrix<_Ty> Transponate() const noexcept {
		tMatrix<_Ty> res(size_b,size_a);
		for(size_t i = 0; i < size_a; i++) {
			for(size_t j = 0; j < size_b; j++) {
				res.El(j,i) = A[i][j];
			}
		}
		return res;
	}
	tMatrix<_Ty> Invert() const {
		if (size_a != size_b)
			throw std::length_error("tMatrix<_Ty>::Invert() : Matrix must be square.");
		tMatrix<_Ty> res(size_a, size_a, true);
		_Ty det = this->Det();
		if(det == _Ty(0))
			throw std::runtime_error("tMatrix<_Ty>::Invert() : Determinant equal zero.");
		for (size_t k = 0; k < size_a; k++) {
			for (size_t i = 0; i < size_a; i++) {
				res.A[k][i] = (this->Minor(i, k).Det()) / det;
				if (((i + k) & 1) == 1) res.A[k][i] *= -1;
			}
		}
		return res;
	}
	tMatrix<_Ty> Modul() const {
		tMatrix<_Ty> ret(size_a, size_b, true);
		for (size_t i = 0; i < size_a; i++) {
			for (size_t j = 0; j < size_b; j++) {
				ret.A[i][j] = abs(A[i][j]);
			}
		}
		return ret;
	}
	_Ty Trace() const {
		if (size_a != size_b)
			throw std::length_error("tMatrix<_Ty>::Trace() : Matrix must be square.");
		_Ty res = 0;
		for (size_t i = 0; i < size_a; i++) {
			res += A[i][i];
		}
		return res / size_a;
	}
	_Ty Det() const {
		if(size_a != size_b)
			throw std::length_error("tMatrix<_Ty>::Det(int=,bool=) : Matrix must be square.");
		switch(size_a) {
		case 1:
			return A[0][0];
		case 2:
			return A[0][0]*A[1][1] - A[0][1]*A[1][0];
		case 3:
			return A[0][0]*A[1][1]*A[2][2] + A[0][1]*A[1][2]*A[2][0] + A[0][2]*A[1][0]*A[2][1]
			- A[0][2]*A[1][1]*A[2][0] - A[0][1]*A[1][0]*A[2][2] - A[0][0]*A[1][2]*A[2][1];
		}
		int mod = 1;
		_Ty det(0);
		for (unsigned int i = 0; i < size_b; i++, mod *= (-1)) {
			if (A[0][i] == (_Ty)0) continue;
			det += mod*A[0][i] * ((this->Minor(0, i)).Det());
		}
		return det;
	}

	static tMatrix EqualMatrix(const size_t n = 3) const noexcept {
		using namespace std;
		vector<vector<_Ty> > vec(n, vector<_Ty>(n, static_cast<_Ty>(0.0)));
		for (size_t i = 0; i < n; i++)
		{
			vec[i][i] = static_cast<_Ty>(1.0);
		}
		return tMatrix(std::move(vec));
	}

};

template<class _Ty>
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
