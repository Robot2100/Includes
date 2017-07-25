#pragma once
#ifndef PARAM_H
#define PARAM_H

#include <string>
#include <vector>
#include <iostream>
#include "Error.h"
class Param
{
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
	Param() {}
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
//					std::cout << "Not found '-' before parameter" << std::endl;
//					std::cout << "Program terminated!" << std::endl;
//					system("pause");
//					exit(-1);
					Error::StopFatalError("Didn't find \"-\" before parameter");

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
				//not found in list
//				std::cout << "Unknown parameter: " << argv[i] << std::endl;
//				std::cout << "Program terminated!" << std::endl;
//				system("pause");
//				exit(-1);
				Error::StopFatalError("Unknown parameter: " + std::string(argv[i]));
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
				std::cout << "Parameter ";
				for (int j = 0; j < (int)names[i].size(); j++) {
					std::cout << names[i][j];
					if (j != names[i].size() - 1)
						std::cout << "/";
				}
				std::cout << " has wrong number of args!" << std::endl;
				std::cout << "Program terminated!" << std::endl;
				system("pause");
				exit(-1);
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
};


#endif
