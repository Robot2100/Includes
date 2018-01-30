#pragma once

#ifndef ELIPSOID_H
#define ELIPSOID_H

// include section
#include "Point.h" // class Point
#include "Cell.h" // class Cell
#include "Dinmat.h" // class Dinmat
#include "Matrix.h" // class Matrix
#include <vector>
#include <string>
#include <sstream>
__if_not_exists (flo) {
	typedef float flo;
}

enum [[deprecated]] ELIPSOID_RESULT {
	OK = 0,
	WrongParameters,
	ZeroVecPoints,
	UndefenedCell,
	Uncalculated
};

class [[deprecated]] Elipsoid {
public:
#pragma warning(push)
#pragma warning( disable : 4996 )
	
    Point cartCenter = Point(0,0,0);
	bool useCartCenter = false;

	Point fracCenter = Point(0, 0, 0);
	bool useFracCenter = false;

	int number = 0;
	int type = 1;
	std::string label = "Q";
	bool useLabel = false;

    bool is_fractal = true;

    Dinmat dinmat;
    bool useDinmat = false;

    std::vector<Point> vecPoints;
    bool useVecPoints = false;

	Cell * pCell = 0;
public:
	void DefineLabel(std::string & s, const int num, const int typ) noexcept {
		label = s;
		type = typ;
		number = num;
		useLabel = true;
	}
	void DefineCell(Cell & s) noexcept {
		pCell = &s;
	}
	void AddVecPoints(std::vector<Point> && in_points, const bool is_fract = true) noexcept {
		if (in_points.size() == 0)
			useVecPoints = false;
		else
			useVecPoints = true;
		is_fractal = is_fract;
		vecPoints = in_points;
	}
    ELIPSOID_RESULT CalculateCenter() noexcept {
        if(useVecPoints == false)
            return ELIPSOID_RESULT::ZeroVecPoints;
        Point cent(0,0,0);
        size_t size = vecPoints.size();
        for(size_t i = 0; i < size; i++)
            cent += vecPoints[i];
        cent = cent * static_cast<flo> (1.0 / size);
		if (is_fractal) {
			fracCenter = cent;
			useFracCenter = true;
		}
		else {
			cartCenter = cent;
			useCartCenter = true;
		}
        return ELIPSOID_RESULT::OK;
    }
    ELIPSOID_RESULT CalculateDinmat() noexcept {
		if (pCell == 0)
			return ELIPSOID_RESULT::UndefenedCell;
		if(is_fractal == true) {
			useCartCenter = false;
			size_t size = vecPoints.size();
			Matrix FTC (pCell->FracToCart());
			for(size_t i = 0; i < size; i++) {
				vecPoints[i] = FTC*vecPoints[i];
			}
			is_fractal = false;

		}
		if(useCartCenter == false) {
        	ELIPSOID_RESULT calcres = CalculateCenter();
        	if(calcres != ELIPSOID_RESULT::OK) return calcres;
		}

		fracCenter = pCell->CartToFrac()*cartCenter;
		useFracCenter = true;

        int size = static_cast<int>(vecPoints.size());
        if(useDinmat == true) {
            dinmat = Dinmat();
        }
        for(int i = 0; i < size; i++) {
            flo dx = vecPoints[i].a[0] - cartCenter.a[0];
            flo dy = vecPoints[i].a[1] - cartCenter.a[1];
            flo dz = vecPoints[i].a[2] - cartCenter.a[2];
            dinmat.U[0] += dx*dx;
			dinmat.U[1] += dy*dy;
			dinmat.U[2] += dz*dz;
			dinmat.U[3] += dx*dy;
			dinmat.U[4] += dx*dz;
			dinmat.U[5] += dy*dz;
        }
		for(int i = 0; i < 6; i++) {
			dinmat.U[i] /= size;
		}
		useDinmat = true;
		return ELIPSOID_RESULT::OK;
    }
	std::string OutShelxString() const {
		std::stringstream ss;
		if (!(useLabel && useDinmat))
			throw std::logic_error("Unprepared Elipsoid");
		ss.precision(5);
		ss.setf(std::stringstream::fixed);
		ss << label << number << ' ' << type << ' ' << fracCenter.a[0] << ' ' << fracCenter.a[1] << ' ' << fracCenter.a[2]
			<< " 11.0000 " << dinmat.U[0] << ' ' << dinmat.U[1] << ' ' << dinmat.U[2] << " =\n "
			<< dinmat.U[5] << ' ' << dinmat.U[4] << ' ' << dinmat.U[3];
		return std::string(ss.str());
	}
#pragma warning(pop)
};

#endif
