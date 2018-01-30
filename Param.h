#pragma once
#ifndef PARAM_H
#define PARAM_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Error.h"
class [[deprecated]]OldParam
{
#pragma warning(push)
#pragma warning( disable : 4996 )
protected:
	std::vector<bool> found;
	std::vector<std::vector<std::string>> names;
	std::vector<int> id;
	std::vector<std::vector<std::string>> strout;
	std::vector<std::string> descriptions;
	std::vector<int> minargs;
	std::vector<int> maxargs;
	std::vector<std::string> nonamestr;
	bool noname = false;
	std::string nonamePar;
public:
	OldParam() {}
	void AddNoname(std::string params)
	{
		nonamePar = params;
		noname = true;
	}
	void AddParam(std::vector<std::string> IN_names, int IN_id, std::string params, std::string description, int min, int max)
	{
		names.push_back(IN_names);
		id.push_back(IN_id);
		descriptions.push_back(" " + params + " - " + description);
		minargs.push_back(min);
		maxargs.push_back(max);
	}
	void TakeAgrs(int argn, char * argv[])
	{
		found.resize(id.size(), false);
		strout.resize(id.size());
		int k = -1, l = -1;
		if (argn < 2) return;
		int i = 1;
		do {
			if (argv[i][0] != '-') {
				if (noname) {
					nonamestr.push_back(argv[i]);
				}
				else {
					throw std::invalid_argument("Didn't find \"-\" before parameter");
				}
			}
			else
				break;
			i++;
		} while (i<argn);

		for (; i < argn; i++) {
			if (argv[i][0] == '-') {
				//search for parameter
				for (k = 0; k < (int)names.size(); k++) {
					for (l = 0; l < (int)names[k].size(); l++) {
						if (!(names[k][l].compare(argv[i]))) {
							found[k] = true;
							goto founded;
						}
					}
				}
				throw std::invalid_argument("Unknown parameter: " + std::string(argv[i]));
			}
			else {
				//add new strout
				strout[k].push_back(argv[i]);
			}

		founded:
			;
		}
		for (int i = 0; i < (int)found.size(); i++) {
			if (found[i] == false)
				continue;
			if (minargs[i] >(int)strout[i].size() || maxargs[i] < (int)strout[i].size()) {
				throw std::invalid_argument("Parameter has wrong number of args!");
			}
		}
	}
	int ReadNextParametr(std::vector<std::string> & OUT_str)
	{
		for (int i = 0; i < (int)id.size(); i++) {
			if (found[i]) {
				found[i] = false;
				OUT_str.swap(strout[i]);
				return id[i];
			}
		}
		return -1;
	}
	void ReadNoname(std::vector<std::string> & OUT_str)
	{
		OUT_str.swap(nonamestr);
	}
	void help() const
	{
		if (noname) {
			std::cout << "Basic input:" << std::endl;
			std::cout << "    " << nonamePar << std::endl;
		}
		std::cout << "Parameters:" << std::endl;
		for (int i = 0; i < (int)found.size(); i++) {
			std::cout << "    ";
			for (int j = 0; j < (int)names[i].size(); j++) {
				std::cout << names[i][j];
				if (j != names[i].size() - 1)
					std::cout << " or ";
			}
			std::cout <<descriptions[i] << std::endl;
		}
	}
#pragma warning(pop)
};

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
	constexpr ConstParam(const BaseParam (&in) [N]) : par(in) {
		static_assert(N > 0, "ConstParam<size_t N> : N must be 1 or greater");
	}
	template <size_t K>
	friend class Param;
};
template <std::size_t N>
class Param {
protected:
	const ConstParam<N> * CP = nullptr;
	bool found[N];
	std::vector<std::string> strout[N];
	int noname = -1;
public:
	Param(const ConstParam<N> * par) noexcept : CP(par) {
		for (int i = 0; i < N; i++) {
			found[i] = false;
			if (std::strcmp(par->par[i].param, "") == 0) {
				_ASSERTE(noname == -1);
				noname = i;
			}
		}
	}
	void TakeAgrs(int argn, char * argv[], void(*func)(const int, std::vector<std::string> &))
	{
		//int k = -1, l = -1;
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
		stringstream sstemp;
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
