#pragma once
#ifndef PARAM_H
#define PARAM_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#define PARAMFUNC(Name) void Name (const int n, std::vector<std::string> & vec)
typedef std::exception ParamException;
class BaseParam {
protected:
	const char shortparam;
	const char * longparam;
	const char * paramlist;
	const char * desc;
public:
	constexpr BaseParam(const char shortp, const char * longp, const char * paraml, const char * des)
		: shortparam(shortp), longparam(longp), paramlist(paraml), desc(des) {}
	//template <std::size_t K>
	friend class Param;
};
class Param {
protected:
	const BaseParam * p;
	const size_t N;
	const bool noname;
public:
	constexpr Param(const BaseParam in[], const size_t N_of_in) noexcept : p(in), N(N_of_in), noname(in[0].shortparam == 0 && in[0].longparam[0] == 0) {}


	void TakeAgrs(const int argn, const char * argv[], void(*func)(const int, std::vector<std::string> &)) const
	{
		int lastone = -1;
		//bool lastone_changed = false;
		std::vector<std::string> result;
		for (size_t i = 1; i < argn; i++)
		{
			if (argv[i][0] == '-') {
				int newone;
				// argv[i] - parameter!
				if (argv[i][1] == '-') {
					// Long parameter
					if ((newone = TakeLong(argn, &(argv[i][2]))) == 0) {
						throw ParamException(argv[i],1);
					}
				}
				else {
					// Short parameter
					if ((newone = TakeShort(argn, argv[i])) == 0) {
						throw ParamException(argv[i],1);
					}
				}
				if (newone == -1) {
					help();
					exit(0);
				}
				if (lastone != -1)
				{
					func(lastone, result);
					result.clear();
				}
				lastone = newone;
			}
			else {
				// argv[i] is not parameter!
				if (lastone == -1) {
					if (noname == true) {
						lastone = 0;
					}
					else {
						throw ParamException(argv[i],2);
					}
				}
				result.push_back(std::string(argv[i]));
			}
		}
		
	}
private:
	int TakeLong(const int argn, const char argv[]) const noexcept{

		for (size_t j = 0; j < N; j++)
		{
			if (strcmp(p[j].longparam, argv) == 0) {
				return j;
			}
		}
		if (strcmp("help", argv) == 0) {
			return -1;
		}
		return 0;
	}
	int TakeShort(const int argn, const char argv[]) const noexcept {
		for (size_t j = 0; j < N; j++)
		{
			if (p[j].shortparam == argv[1] && argv[2] == 0) {
				return j;
			}
		}
		if ('h' == argv[1] && argv[2] == 0) {
			return -1;
		}
		return 0;
	}
	void help() const{
		cout << "options: ";
		for (size_t i = 0; i < N; i++)
		{
			//+
		}
	}
};

//template <std::size_t N>
//class Param {
//protected:
//	const ConstParam<N> * CP = nullptr;
//	int noname = -1;
//public:
//	Param(const ConstParam<N> * par) noexcept : CP(par) {
//		for (auto i = 0; i < N; i++) {		
//			if (par->par[i].shortparam == (char)0 && par->par[i].longparam[0] == 0) {
//				_ASSERTE(noname == -1);
//				noname = i;
//			}
//		}
//	}
//	void TakeAgrs(const int argn, char * argv[], void(*func)(const int, std::vector<std::string> &)) const
//	{
//		if (argn < 2) return;
//		std::vector<std::string> res;
//		int i = 1;
//		do {
//			if (argv[i][0] != '-') {
//				if (noname != -1) {
//					res.push_back(argv[i]);
//				}
//				else {
//					throw std::invalid_argument("Didn't find \"-\" before parameter");
//				}
//			}
//			else
//				break;
//			i++;
//		} while (i < argn);
//		if (i != 1) {
//			func(noname, res);
//			res.clear();
//		}
//		int last = -1;
//		for (; i < argn; i++) {
//			if (argv[i][0] == '-') {
//				//search for parameter
//				if (last != -1) {
//					func(last, res);
//					res.clear();
//					last = -1;
//				}
//				for (int k = 0; k < N; k++) {
//					if (std::strcmp((CP->par[k]).param, argv[i]) == 0) {
//						last = k;
//					}
//				}
//				if (last == -1) {
//					throw std::invalid_argument("Unknown parameter: " + std::string(argv[i]));
//				}
//			}
//			else {
//				//add new res
//				res.push_back(argv[i]);
//			}
//		}
//		if (last != -1) {
//			func(last, res);
//			res.clear();
//		}
//		help(res);
//		func(-1, res);
//	}
//private:
//	void help(std::vector<std::string> & out) const noexcept
//	{
//		std::vector<std::string> res;
//		std::stringstream sstemp;
//		if (noname!=-1) {
//			res.push_back("Basic input:");
//			sstemp << "    " << CP->par[noname].desc;
//			res.push_back(sstemp.str());
//			sstemp.str(std::string());
//		}
//		res.push_back("Parameters:");
//		for (int i = 0; i < N; i++) {
//			if (noname == i) continue;
//			sstemp << "    " << CP->par[i].param << "\t: " << CP->par[i].desc;
//			res.push_back(sstemp.str());
//			sstemp.str(std::string());
//		}
//		res.swap(out);
//	}
//};

#endif
