#pragma once
#ifndef PARAM_H
#define PARAM_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class BaseParam {
protected:
	const char * param;
	const char * desc;
public:
	template<std::size_t N, std::size_t M>
	constexpr BaseParam(const char(&p)[N], const char(&des)[M]) : param(p), desc(des) {}
	constexpr BaseParam() : param(nullptr), desc(nullptr) {}
	template <std::size_t K>
	friend class Param;
};
template <std::size_t N>
class ConstParam {
protected:
	const BaseParam *par;

public:
	constexpr ConstParam(const BaseParam (in) [N]) : par(in) {
		static_assert(N > 0, "ConstParam<size_t N> : N must be 1 or greater");
	}
	template <size_t K>
	friend class Param;
};
template <std::size_t N>
class Param {
protected:
	const ConstParam<N> * CP = nullptr;
	int noname = -1;
public:
	Param(const ConstParam<N> * par) noexcept : CP(par) {
		for (int i = 0; i < N; i++) {		
			if (std::strcmp(par->par[i].param, "") == 0) {
				_ASSERTE(noname == -1);
				noname = i;
			}
		}
	}
	void TakeAgrs(const int argn, char * argv[], void(*func)(const int, std::vector<std::string> &)) const
	{
		if (argn < 2) return;
		std::vector<std::string> res;
		int i = 1;
		do {
			if (argv[i][0] != '-') {
				if (noname != -1) {
					res.push_back(argv[i]);
				}
				else {
					throw std::invalid_argument("Didn't find \"-\" before parameter");
				}
			}
			else
				break;
			i++;
		} while (i < argn);
		if (i != 1) {
			func(noname, res);
			res.clear();
		}
		int last = -1;
		for (; i < argn; i++) {
			if (argv[i][0] == '-') {
				//search for parameter
				if (last != -1) {
					func(last, res);
					res.clear();
					last = -1;
				}
				for (int k = 0; k < N; k++) {
					if (std::strcmp((CP->par[k]).param, argv[i]) == 0) {
						last = k;
					}
				}
				if (last == -1) {
					throw std::invalid_argument("Unknown parameter: " + std::string(argv[i]));
				}
			}
			else {
				//add new res
				res.push_back(argv[i]);
			}
		}
		if (last != -1) {
			func(last, res);
			res.clear();
		}
		help(res);
		func(-1, res);
	}
private:
	void help(std::vector<std::string> & out) const noexcept
	{
		std::vector<std::string> res;
		std::stringstream sstemp;
		if (noname!=-1) {
			res.push_back("Basic input:");
			sstemp << "    " << CP->par[noname].desc;
			res.push_back(sstemp.str());
			sstemp.str(std::string());
		}
		res.push_back("Parameters:");
		for (int i = 0; i < N; i++) {
			if (noname == i) continue;
			sstemp << "    " << CP->par[i].param << "\t: " << CP->par[i].desc;
			res.push_back(sstemp.str());
			sstemp.str(std::string());
		}
		res.swap(out);
	}
};

#endif
