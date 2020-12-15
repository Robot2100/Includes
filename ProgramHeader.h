#pragma once
#ifndef PROGRAMHEADER_H
#define PROGRAMHEADER_H
#include <array>
#include <string>
#include <iostream>


struct ProgramHeader {
	ProgramHeader() noexcept {}
	inline static void View(const char* Name) {
		std::cout << Name << ".\n" << std::flush;
	}
	inline static void View(const char* Name, const char* Version) {
		std::cout << Name << " (version " << Version << ").\n" << std::flush;
	}
	template <int N = 80>
	static consteval std::array<char, N> _PrepareView(const char * nam, const char * ver) {
		std::array<char, N> line;
		for (int i = 0; i < N; ++i) {
			line[i] = 0;
		}
		int x = 0;
		for (; x < N && nam[x] != '\0'; ++x) {
			line[x] = nam[x];
		}
		if (ver != 0) {
			constexpr char t[] = " (version ";
			for (int i = 0; i < 11; ++i) {
				line[x+i] = t[i];
			}
			x += 10;
			for (int i = 0; x < N && ver[i] != '\0'; ++x,++i) {
				line[x] = ver[i];
			}
			line[x] = ')';
		}
		++x;
		line[x] = '.';
		++x;
		line[x] = '\n';
		return line;
	}

};
#endif
