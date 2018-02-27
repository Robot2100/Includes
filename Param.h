#pragma once
#ifndef PARAM_H
#define PARAM_H

#include <string>
#include <vector>
#include <ostream>
#include <exception>

namespace IncExceptions {
	class ParamException : public std::runtime_error {
	public:
		explicit ParamException(const std::string & _Message) : runtime_error(_Message) {}
		explicit ParamException(const char * _Message) : runtime_error(_Message) {}
	};
}

#define PARAMFUNC(Name) void Name (const int n, std::vector<std::string> & vec)
class BaseParam {
protected:
	const char * shortparam;
	const char * longparam;
	const char * paramlist;
	const char * desc;
public:
	constexpr BaseParam(const char * shortp, const char * longp, const char * paraml, const char * des)
		: shortparam(shortp), longparam(longp), paramlist(paraml), desc(des) {}
	template <std::size_t K>
	friend class Param;
};
template <std::size_t N>
class Param {
protected:
	const BaseParam * p;
	const bool noname;
public:
	constexpr Param(const BaseParam in[]) noexcept : p(in), noname(in[0].shortparam[0] == 0 && in[0].longparam[0] == 0) {}
	void TakeAgrs(const int argn, char * argv[], void(*func)(const int, std::vector<std::string> &)) const
	{
		int lastone = -1;
		//bool lastone_changed = false;
		std::vector<std::string> result;
		for (size_t i = 1; i < static_cast<size_t>(argn); i++)
		{
			if (argv[i][0] == '-') {
				int newone;
				// argv[i] - parameter!
				if (argv[i][1] == '-') {
					// Long parameter
					if ((newone = TakeLong(&(argv[i][2]))) == -2) {
						throw IncExceptions::ParamException(argv[i]);
					}
				}
				else {
					// Short parameter
					if ((newone = TakeShort(&(argv[i][1]))) == -2) {
						throw IncExceptions::ParamException(argv[i]);
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
						throw IncExceptions::ParamException(argv[i]);
					}
				}
				result.push_back(std::string(argv[i]));
			}
		}
		if (lastone != -1)
		{
			func(lastone, result);
		}

	}
private:
	int TakeLong(const char argv[]) const noexcept {

		for (size_t j = 0; j < N; j++)
		{
			if (strcmp(p[j].longparam, argv) == 0) {
				return j;
			}
		}
		if (strcmp("help", argv) == 0) {
			return -1;
		}
		return -2;
	}
	int TakeShort(const char argv[]) const noexcept {
		for (size_t j = 0; j < N; j++)
		{
			if (strcmp(p[j].shortparam, argv) == 0) {
				return j;
			}
		}
		if (strcmp("h", argv) == 0) {
			return -1;
		}
		return -2;
	}
	void help() const {
		std::cout << "usage: Program ";
		char CNP[N];
		size_t Nsym = 15, dsym = 0;
		if (noname) {
			std::cout << p[0].paramlist << " ";
			Nsym += 1 + std::strlen(p[0].paramlist);
		}
		for (size_t i = noname ? 1 : 0; i < N; i++)
		{
			CNP[i] = 0;
			if (p[i].shortparam[0] != 0)
				CNP[i] |= 1;
			if (p[i].longparam[0] != 0)
				CNP[i] |= 2;
			switch (CNP[i]) {
			case 1:
				dsym = (p[i].paramlist[0] == 0 ? 4 : std::strlen(p[i].paramlist) + 5) + std::strlen(p[i].shortparam);
				if (dsym + Nsym > 80) {
					std::cout << "\n               ";
					Nsym = 15;
				}
				std::cout << "[-" << p[i].shortparam;
				break;
			case 2:
				dsym = (p[i].paramlist[0] == 0 ? 5 : std::strlen(p[i].paramlist) + 6) + std::strlen(p[i].longparam);
				if (dsym + Nsym > 80) {
					std::cout << "\n               ";
					Nsym = 15;
				}
				std::cout << "[--" << p[i].longparam;
				break;
			case 3:
				dsym = (p[i].paramlist[0] == 0 ? 7 : std::strlen(p[i].paramlist) + 8) + std::strlen(p[i].shortparam) + std::strlen(p[i].longparam);
				if (dsym + Nsym > 80) {
					std::cout << "\n               ";
					Nsym = 15;
				}
				std::cout << "[-" << p[i].shortparam << "/--" << p[i].longparam;
				break;
			}
			Nsym += dsym;
			if (p[i].paramlist[0] != 0)
				std::cout << " " << p[i].paramlist << "] ";
			else
				std::cout << "] ";
		}
		if (Nsym > 69) {
			std::cout << "\n               ";
		}
		std::cout << "[-h/--help]";
		std::cout << "\n\nList of options:\n\n";

		if (noname) {
			std::cout << "  " << p[0].paramlist << "\n      " << p[0].desc << "\n\n";
		}
		for (size_t i = noname ? 1 : 0; i < N; i++)
		{
			switch (CNP[i]) {
			case 1:
				std::cout << "  [-" << p[i].shortparam;
				break;
			case 2:
				std::cout << "  [--" << p[i].longparam;
				break;
			case 3:
				std::cout << "  [-" << p[i].shortparam << ", --" << p[i].longparam;
				break;
			}
			if (p[i].paramlist[0] != 0) {
				std::cout << "]  " << p[i].paramlist;
			}
			else
				std::cout << ']';
			std::cout << "\n      " << p[i].desc << "\n\n";
		}

		std::cout << "  [-h, --help]\n      Show this help\n";
	}
};
#endif
