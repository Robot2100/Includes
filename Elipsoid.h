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

enum ELIPSOID_RESULT {
	OK = 0,
	WrongParameters,
	ZeroVecPoints,
	UndefenedCell,
	Uncalculated
};

class Elipsoid {
private:
    const flo RadtoGrad = static_cast<flo> (90/1.57079632679489661923);
	const flo GradtoRad = static_cast<flo> (1.57079632679489661923/90);

    Point center = Point(0,0,0);
	bool useCenter = false;

	int number = 0;
	int type = 1;
	std::string label = "Q";
	bool useLabel = false;

    bool is_fractal = true;

    Dinmat dinmat;
    bool useDinmat = false;

    std::vector<Point> vecPoints;
    bool useVecPoints = false;

	Cell * pCell = nullptr;
	bool useCell = false;
public:
    Elipsoid() {};
    ~Elipsoid() {};
	void DefineLabel(std::string & s, const int num, const int typ)  {
		label = s;
		type = typ;
		number = num;
		useLabel = true;
	}
	void DefineCell(Cell & s) {
		pCell = &s;
		useCell = true;
	}
    ELIPSOID_RESULT AddVecPoints(std::vector<Point> & in_points, bool is_fract = true) {
        if(in_points.size() == 0)
            return ELIPSOID_RESULT::WrongParameters;
		is_fractal = is_fract;
        vecPoints.swap(in_points);
        useVecPoints = true;
        return ELIPSOID_RESULT::OK;
    }
    ELIPSOID_RESULT CalculateCenter() {
        if(useVecPoints == false)
            return ELIPSOID_RESULT::ZeroVecPoints;
        Point cent(0,0,0);
        int size = vecPoints.size();
        for(int i = 0; i < size; i++)
            cent += vecPoints[i];
        cent = cent * static_cast<flo> (1.0 / size);
        center = cent;
		useCenter = true;
        return ELIPSOID_RESULT::OK;
    }
    ELIPSOID_RESULT CalculateDinmat() {
		if(is_fractal == true) {
			if(useCell == false)
				return ELIPSOID_RESULT::UndefenedCell;
			useCenter = false;
			int size = vecPoints.size();
			Matrix FTC = pCell->FracToCart();
			for(int i = 0; i < size; i++) {
				vecPoints[i] = FTC*vecPoints[i];
			}
		}
		if(useCenter == false) {
        	ELIPSOID_RESULT calcres = CalculateCenter();
        	if(calcres != ELIPSOID_RESULT::OK) return calcres;
		}
        int size = static_cast<int>(vecPoints.size());
        if(useDinmat == true) {
            dinmat = Dinmat();
        }
        for(int i = 0; i < size; i++) {
            flo dx = vecPoints[i].a[0]-center.a[0];
            flo dy = vecPoints[i].a[1] -center.a[1];
            flo dz = vecPoints[i].a[2] -center.a[2];
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
			throw "Elipsoid Output Error!";
		ss.precision(6);
		ss << label << number << ' ' << type << ' ' << center.a[0] << ' ' << center.a[1] << ' ' << center.a[2] << ' '
			<< dinmat.U[0] << ' ' << dinmat.U[1] << ' ' << dinmat.U[2] << " =\n " 
			<< dinmat.U[5] << ' ' << dinmat.U[4] << ' ' << dinmat.U[3];
		return std::string(ss.str());
	}
};

#endif
