#pragma once
#ifndef INS_H
#define INS_H
#include <sstream>
#include <istream>
#include <vector>
#include "Point.h"
#include "Matrix.h"
namespace nsShelxFile {
	struct SYMM
	{
		Matrix mat;
		Point point;
		unsigned int mult;
		void Multiplicity() {
			Point c(static_cast<flo>(0.1f), static_cast<flo>(0.2f), static_cast<flo>(0.3f));
			Point t(GenSymmNorm(c));
			for (mult = 1; (t - c).r() > (flo)0.001; mult++) {
				t = GenSymmNorm(t);
			}
		}
		explicit SYMM(const char * str) : mat(3u, 3u), point(static_cast<flo>(0.0f), static_cast<flo>(0.0f), static_cast<flo>(0.0f))
		{
			bool minus(false);
			unsigned int j(0);
			for (unsigned int i = 0; str[i] != '\0'; i++) {
				switch (str[i]) {
				case 'x':
				case 'X':
					if (minus == true) mat.El(j, 0) = static_cast<flo>(-1.0f);
					else mat.El(j, 0) = static_cast<flo>(1.0f);
					minus = false;
					break;
				case 'y':
				case 'Y':
					if (minus == true) mat.El(j, 1) = static_cast<flo>(-1.0f);
					else mat.El(j, 1) = static_cast<flo>(1.0f);
					minus = false;
					break;
				case 'z':
				case 'Z':
					if (minus == true) mat.El(j, 2) = static_cast<flo>(-1.0f);
					else mat.El(j, 2) = static_cast<flo>(1.0f);
					minus = false;
					break;
				case ',':
					j++;
				case ' ':
				case '+':
					break;
				case '-':
					minus = true;
					break;
				default:
				{
					unsigned int k(i);
					for (; (str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == '/'; i++);

					//constexpr char const const p3[8][4] { "1/2","1/3","1/4","1/6","2/3","3/4","5/6","0.5" };
					//constexpr char const const p4[2][5] { "0.25","0.75" };
					switch (i - k) {
					case 3:
						if (str[k] == '1') {
							switch (str[k + 2]) {
							case '2':
								point.a[j] = static_cast<flo>(0.5);
								break;
							case '3':
								if (minus == true) point.a[j] = static_cast<flo>(2.0 / 3.0);
								else point.a[j] = static_cast<flo>(1.0 / 3.0);
								break;
							case '4':
								if (minus == true) point.a[j] = static_cast<flo>(0.75);
								else point.a[j] = static_cast<flo>(0.25);
								break;
							case '6':
								if (minus == true) point.a[j] = static_cast<flo>(5.0 / 6.0);
								else point.a[j] = static_cast<flo>(1.0 / 6.0);
								break;
							}
						}
						else {
							switch (str[k + 2]) {
							case '3':
								if (minus == true) point.a[j] = static_cast<flo>(1.0 / 3.0);
								else point.a[j] = static_cast<flo>(2.0 / 3.0);
								break;
							case '4':
								if (minus == true) point.a[j] = static_cast<flo>(0.25);
								else point.a[j] = static_cast<flo>(0.75);
								break;
							case '5':
								point.a[j] = static_cast<flo>(0.5);
								break;
							case '6':
								if (minus == true) point.a[j] = static_cast<flo>(1.0 / 6.0);
								else point.a[j] = static_cast<flo>(5.0 / 6.0);
								break;
							}

						}
						break;
					case 4:
						if ((minus == true && str[k + 2] == '7') || (minus == false && str[k + 2] != '7')) point.a[j] = static_cast<flo>(0.25);
						else point.a[j] = static_cast<flo>(0.75);
						break;
					default:
						throw std::invalid_argument(str);
					}
					minus = false;
					i--;
				}
				}
			}
			Multiplicity();
		}
		Point GenSymm(const Point & in) const
		{
			return point + (mat * in);
		}
		Point GenSymmNorm(const Point & in) const
		{
			Point temp = GenSymm(in);
			for (int i = 0; i < 3; i++) {
				if (temp.a[i] >= 1) temp.a[i] -= (int)temp.a[i];
				if (temp.a[i] < 0) temp.a[i] += (int)abs(temp.a[i]) + 1;
			}
			return temp;
		}
	};
	constexpr char ShelInstr[][5]{ "CELL", "LATT", "SYMM", "UNIT", "SFAC", "END", "ABIN", "ACTA", "AFIX", "ANIS", "ANSC", "ANSR", "BASF", "BIND", "BLOC", "BOND", "BUMP", "CGLS", "CHIV", "CONF", "CONN", "DAMP", "DANG", "DEFS", "DELU", "DFIX", "DISP", "EADP", "EQIV", "EXTI", "EXYZ", "FEND", "FLAT", "FMAP", "FRAG", "FREE", "FVAR", "GRID", "HFIX", "HKLF", "HTAB", "ISOR", "LAUE", "LIST", "L.S.", "MERG", "MORE", "MOVE", "MPLA", "NCSY", "NEUT", "OMIT", "PART", "PLAN", "PRIG", "REM", "RESI", "RIGU", "RTAB", "SADI", "SAME", "SHEL", "SIMU", "SIZE", "SPEC", "STIR", "SUMP", "SWAT", "TEMP", "TITL", "TWIN", "TWST", "WGHT", "WIGL", "WPDB", "XNPD", "ZERR","" };
	struct Atom {
		char label[5];
		unsigned int type;
		Point point;
		flo occup;
		Dinmat dinmat;
		Atom(const char *lab, const unsigned int typ, const Point & p, const flo occupancy, const Dinmat & din) noexcept : type(typ), point(p), occup(occupancy), dinmat(din) {
			strcpy_s(label, lab);
		}
	};
	struct ShelxData {
	private:
		static constexpr bool _MoveNothrow = std::is_nothrow_move_constructible<Cell>::value && std::is_nothrow_move_constructible<std::vector<SYMM> >::value && std::is_nothrow_move_constructible<std::vector<Dinmat> >::value && std::is_nothrow_move_constructible<std::vector<Point> >::value;
	public:
		Cell cell;
		std::vector<SYMM> symm;
		std::vector<Atom> atom;
		
		explicit ShelxData(std::istream & in)
		{
			char buf[128], buf4[128];
			unsigned int check(0u);
			while (in.eof() == false) {
				in.getline(buf, 127);
				std::stringstream input(buf);
				input >> buf4;
				strupr(buf4);
				unsigned int M(0u);
				for (; std::strcmp(ShelInstr[M], buf4) != 0; M++) {
					if (M == 78)
						break;
				}
				switch (M) {
				case 0: // CELL
				{
					float a, b, c, an, bn, cn;
					input >> a >> a >> b >> c >> an >> bn >> cn;
					cell.Create(a, b, c, an, bn, cn);
				}
					check |= 0x0001;
					break;
				case 1: // LATT
				{
					int temp;
					input >> temp;
					if (temp > 0) 
						symm.push_back(SYMM("-x,-y,-z"));
				}
					check |= 0x0002;
					break;
				case 2: // SYMM
					input.getline(buf, 127);
					symm.push_back(SYMM(buf));
					check |= 0x0004;
					break;
				case 3: // UNIT
					check |= 0x0008;
					break;
				case 4: // SFAC
					check |= 0x0010;
					break;
				case 5: // END
					symm.shrink_to_fit();
					atom.shrink_to_fit();
					return;
				default:
					if (M == 78) {
						if (check != 0x001F) {
							throw std::runtime_error(buf);
						}
						char temp[128];
						input >> temp;
						unsigned int type = atoi(temp);
						if(type == 0) {
							throw std::runtime_error(buf);
						}
						Point point;
						for (int i = 0; i < 3; i++) {
							input >> temp;
							point.a[i] = atoi(temp);
						}

						input >> temp;
						flo occup = (flo)atof(temp);

						Dinmat dinmat;
						input >> temp;
						dinmat.U[0] = (flo)atof(temp);
						input >> temp;
						if (strcmp(temp, "") == 0) {
							dinmat.U[1] = dinmat.U[0];
							dinmat.U[2] = dinmat.U[0];
							dinmat.U[3] = static_cast<flo>(0.0);
							dinmat.U[4] = static_cast<flo>(0.0);
							dinmat.U[5] = static_cast<flo>(0.0);
						}
						else {

							for (int i = 1; i < 6; i++) {
								if (strcmp(temp, "=") == 0) {
									input.clear();
									in.getline(buf, 127);
									input << buf;
								}
								dinmat.U[i] = (flo)atof(temp);
								input >> temp;
							}
						}
						atom.push_back(Atom(buf4, type, point, occup, dinmat));
					}
				}
			}
			symm.shrink_to_fit();
			atom.shrink_to_fit();
		}
		ShelxData(const ShelxData & in) = delete;
		ShelxData(ShelxData && in) noexcept(_MoveNothrow) : cell(std::move(in.cell)), symm(std::move(in.symm)), atom(std::move(in.atom)) {}
	};
}
#endif