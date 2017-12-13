#pragma once
#ifndef HKL_H
#define HKL_H
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct HklSignal {
	float inten;
	float sig;
	explicit HklSignal (float intens = 0.0F, float sigma = 0.0F) noexcept : inten(intens), sig(sigma) {}
};
class Hkl{
private:
	std::vector<std::vector<std::vector<HklSignal> > > sig;
	int hmin, hmax, kmin, kmax, lmin, lmax;
	int _use[6];
	int dh, dk, dl;
	HklSignal * bigest = nullptr;
	void _resize(const int Hmin, const int Hmax, const int Kmin, const int Kmax, const int Lmin, const int Lmax) {
		std::vector<std::vector<std::vector<HklSignal> > >	temp;
		const unsigned int newdh = Hmax - Hmin + 1;
		const unsigned int newdk = Kmax - Kmin + 1;
		const unsigned int newdl = Lmax - Lmin + 1;
		temp.resize(newdh);
		int Hstart = hmin - Hmin;
		int Hend = hmax - Hmax;
		int Kstart = kmin - Kmin;
		int Kend = kmax - Kmax;
		int Lstart = lmin - Lmin;
		int Lend = lmax - Lmax;

		for (int i = 0; i < static_cast<const int>(newdh); i++, Hstart++) {
			temp[i].resize(newdk);
			if (Hstart < 0) continue;
			for (int j = 0; j < static_cast<const int>(newdk); j++, Kstart++) {
				temp[i][j].resize(newdl);
				if (Kstart < 0) continue;
				for (int k = 0; k < static_cast<const int>(newdl); k++, Lstart++) {
					temp[i][j][k] = sig[Hstart][Kstart][Lstart];
					//+
				}
			}
		}
		dh = newdh;
		dk = newdk;
		dl = newdl;
		hmin = _use[0];
		hmax = _use[1];
		kmin = _use[2];
		kmax = _use[3];
		lmin = _use[4];
		lmax = _use[5];

		temp.swap(sig);
		//+
	}
	void _ClearAndResize(std::ifstream & in) {
		//+
	}
	void _ReadFromFstream(std::ifstream & in) {
		int h, k, l;
		char buf[256];
		std::stringstream ss;
		do {
			in.getline(buf, 255);
			ss << buf;
			ss >> buf;
			h = atoi(buf);
			ss >> buf;
			k = atoi(buf);
			ss >> buf;
			l = atoi(buf);
			ss >> buf;
			float value = float(atof(buf));
			ss >> buf;
			float sigma = float(atof(buf));
resizeloop:
			try {
				AddtoHkl(h, k, l, value, sigma);
			}
			catch (std::out_of_range) {
				_resize(hmin - 50, hmax + 50, kmin - 50, kmax + 50, lmin - 50, lmax + 50);
				goto resizeloop;
			}

		} while (in.eof() == false);
	}
public:
	Hkl(const int Hmin = -50, const int Hmax = 50, const int Kmin = -50, const int Kmax = 50, const int Lmin = -50, const int Lmax = 50)
			: hmin(Hmin), hmax(Hmax), kmin(Kmin), kmax(Kmax), lmin(Lmin), lmax(Lmax) {
		dh = Hmax - Hmin + 1;
		dk = Kmax - Kmin + 1;
		dl = Lmax - Lmin + 1;
		if (dh < 1 || dk < 1 || dl < 1)
			throw std::invalid_argument("Hkl constructor");
		for (int i = 0; i < 6; i++) {
			_use[i] = 0;
		}
		sig.resize(dh);
		for (int i = 0; i < dh; i++) {
			sig[i].resize(dk);
			for (int j = 0; j < dk; j++) {
				sig[i][j].resize(dl);
			}
		}
	}
	void AddtoHkl(const int h, const int k, const int l, float value, float sigma) {
		if (h < hmin || h > hmax || k < kmin || k > kmax || l < lmin || l > lmax) {
			throw std::out_of_range("Hkl::AddtoHkl");
		}
		HklSignal & res = sig[h - hmin][k - kmin][l - lmin];
		res.inten += value;
		res.sig += sigma;
		if (h < _use[0]) _use[0] = h;
		if (h > _use[1]) _use[1] = h;
		if (k < _use[2]) _use[2] = k;
		if (k > _use[3]) _use[3] = k;
		if (l < _use[4]) _use[4] = l;
		if (l > _use[5]) _use[5] = l;
	}
	void Shrink_to_fit() noexcept {
		if (hmin == _use[0] && hmax == _use[1] &&
			kmin == _use[2] && kmax == _use[3] &&
			lmin == _use[4] && lmax == _use[5]) return;
		std::vector<std::vector<std::vector<HklSignal> > >	temp;
		temp.swap(sig);
		const unsigned int newdh = _use[1] - _use[0] + 1;
		const unsigned int newdk = _use[3] - _use[2] + 1;
		const unsigned int newdl = _use[5] - _use[4] + 1;
		sig.resize(newdh);
		for (int i = 0, h1 = _use[0] - hmin; i < static_cast<const int>(newdh); i++, h1++) {
			sig[i].resize(newdk);
			for (int j = 0, k1 = _use[2] - kmin; j < static_cast<const int>(newdk); j++, k1++) {
				sig[i][j].resize(newdl);
				for (int k = 0, l1 = _use[4] - lmin; k < static_cast<const int>(newdl); k++, l1++) {
					sig[i][j][k] = temp[h1][k1][l1];
				}
			}
		}
		dh = newdh;
		dk = newdk;
		dl = newdl;
		hmin = _use[0];
		hmax = _use[1];
		kmin = _use[2];
		kmax = _use[3];
		lmin = _use[4];
		lmax = _use[5];
	}
	inline HklSignal & El(const int h, const int k, const int l) {
		if (h < hmin || h > hmax || k < kmin || k > kmax || l < lmin || l > lmax) {
			throw std::out_of_range("Hkl::AddtoHkl");
		}
		return sig[h - hmin][k - kmin][l - lmin];
	}
	void ReadFromFile(const std::string & filename, const bool clear) {
		std::ifstream in(filename);
		if (in.is_open() != true)
			throw std::ios_base::failure("Hkl::ReadFromFile(std::string, bool)");
		if (clear) {
			_ClearAndResize(in);
			in.close();
			in.open(filename);
			if (in.is_open() != true)
				throw std::ios_base::failure("Hkl::ReadFromFile(std::string, bool)");
		}
		_ReadFromFstream(in);
		in.close();
	}
	void ReadFromFile(const char * filename, const bool clear) {
		std::ifstream in(filename);
		if (in.is_open() != true)
			throw std::ios_base::failure("Hkl::ReadFromFile(char *, bool)");
		if (clear) {
			_ClearAndResize(in);
			in.close();
			in.open(filename);
			if (in.is_open() != true)
				throw std::ios_base::failure("Hkl::ReadFromFile(char *, bool)");
		}
		_ReadFromFstream(in);
		in.close();
	}

};
#endif