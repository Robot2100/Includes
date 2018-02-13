#pragma once
#ifndef INS_H
#define INS_H
#include <sstream>
#include <istream>
#include <vector>
#include <cmath>
#include "Point.h"
#include "Matrix.h"
namespace nsShelxFile {
	struct SYMM
	{
		Matrix mat;
		Point point;
		unsigned int mult;
		bool LATT;
		std::string sstr;
		SYMM() {}
		SYMM MirrorSymm() const {
			SYMM out;
			out.mat = this->mat.Invert();
			out.point = -(this->point);
			out.mult = this->mult;
			return out;
		}
		void Multiplicity() {
			Point c(static_cast<flo>(0.1f), static_cast<flo>(0.2f), static_cast<flo>(0.3f));
			Point t(GenSymmNorm(c));
			for (mult = 1; (t - c).r() > (flo)0.001; mult++) {
				t = GenSymmNorm(t);
			}
		}
		explicit SYMM(const char * str, bool isLATT = false) : mat(3u, 3u), point(static_cast<flo>(0.0f), static_cast<flo>(0.0f), static_cast<flo>(0.0f)), LATT(isLATT), sstr(str)
		{
			bool minus = false;
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
			return (point + (mat * in));
		}		
		Point RetroGenSymm(const Point & in) const
		{
			return (mat *(point +  in));
		}

		Point GenSymmNorm(const Point & in) const
		{
			Point temp = GenSymm(in);
			for (int i = 0; i < 3; i++) {
				if (temp.a[i] >= 1) temp.a[i] -= (int)temp.a[i];
				if (temp.a[i] < 0) temp.a[i] += 1 - (int)temp.a[i];
			}
			return temp;
		}
		Point GenSymm(const Point & in, std::vector<SYMM*> & vsym) {
			vsym.push_back(this);
			return point + (mat * in);
		}
		Point GenSymmNorm(const Point & in, std::vector<SYMM*> & vsym) {
			Point temp = GenSymm(in, vsym);
			for (int i = 0; i < 3; i++) {
				if (temp.a[i] >= 1) temp.a[i] -= (int)temp.a[i];
				if (temp.a[i] < 0) temp.a[i] += 1 - (int)temp.a[i];
			}
			return temp;
		}
		static void MoveToCell(Point & p) noexcept {
			for (int i = 0; i < 3; i++) {
				if (p.a[i] >= 1) p.a[i] -= (int)p.a[i];
				if (p.a[i] < 0) p.a[i] += 1 - (int)p.a[i];
			}
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
		Atom() noexcept : type(0u), occup(flo(0)) {}
		Atom(const char *lab, const unsigned int typ, const flo occupancy, Cell & cell, const std::vector<Point> & vecPoints, bool is_fractal = true) : type(typ), occup(occupancy) {
			strcpy_s(label, lab);
			size_t size = vecPoints.size();
			if (size == 0)
				throw std::invalid_argument("Size of vecPoints == 0");
			point = Point(0, 0, 0);
			for (size_t i = 0; i < size; i++)
				point += vecPoints[i];
			point *= static_cast<flo> (1.0 / size);
			if (is_fractal == false) {
				point = cell.CartToFrac() * point;
			}
			dinmat = Dinmat();
			Point Center(point);
			if (is_fractal == false) {
				Center = cell.FracToCart() * Center;
			}
			for (size_t i = 0; i < size; i++) {
				Point d(vecPoints[i].a[0] - Center.a[0],
					vecPoints[i].a[1] - Center.a[1],
					vecPoints[i].a[2] - Center.a[2]);
				if (is_fractal == true) {
					d = cell.FracToCart() * d;
				}
				dinmat.U[0] += d.a[0] * d.a[0];
				dinmat.U[1] += d.a[1] * d.a[1];
				dinmat.U[2] += d.a[2] * d.a[2];
				dinmat.U[3] += d.a[0] * d.a[1];
				dinmat.U[4] += d.a[0] * d.a[2];
				dinmat.U[5] += d.a[1] * d.a[2];
			}
			for (int i = 0; i < 6; i++) {
				dinmat.U[i] /= size;
			}
		}
	};
	struct ShelxData {
	private:
		
		static constexpr bool _MoveNothrow = std::is_nothrow_move_constructible<Cell>::value && std::is_nothrow_move_constructible<std::vector<SYMM> >::value && std::is_nothrow_move_constructible<std::vector<Atom> >::value && std::is_nothrow_move_constructible<std::vector<std::string> >::value && std::is_nothrow_move_constructible<std::vector<size_t> >::value;
		void Cleaning(std::vector<Atom> & atoms) const {
			const float cutter = 0.3f / (float)(cell.FracToCart().Trace());
			for (int k = 0; k < 2; k++) {
				const size_t s = atoms.size();
				for (size_t i = 0; i < s - 1; i++) {
					if (atoms[i].type == 0) continue;
					for (size_t j = i; j < s; j++) {
						if (i == j || atoms[j].type==0) continue;
						if ( (float)(atoms[i].point - atoms[j].point).r() < cutter) {
							atoms[j].type = 0;
						}
					}
				}
				std::vector<Atom> temp;
				temp.swap(atoms);
				for (size_t i = 0; i < s; i++) {
					if (temp[i].type == 0)
						continue;
					atoms.push_back(std::move(temp[i]));
				}
				for (size_t i = 0; i < atoms.size(); i++) {
					(atoms.back()).point += (flo)0.5;
					SYMM::MoveToCell(atoms[i].point);
				}
			}
		}
	
	public:
		Cell cell;
		std::vector<SYMM> symm;
		std::vector<Atom> atom;
		std::vector<std::string> sfac;
		std::vector<size_t> unit;
		int LATT;

		std::vector<nsShelxFile::Atom> GenerateSymmAtom() const noexcept {
			const size_t size_s = symm.size();
			std::vector<Atom> atomcopy(atom);
			for (size_t j = 0; j < size_s; j++) {
				size_t na = atomcopy.size();
				for (size_t i = 0; i < na; i++) {
					Atom temp(atomcopy[i]);
					for (size_t m = 1; m < symm[j].mult; m++) {
						temp.point = symm[j].GenSymmNorm(temp.point);
						temp.dinmat = symm[j].mat*temp.dinmat.ToMatrix();
						temp.dinmat.U[0] = std::abs(temp.dinmat.U[0]);
						temp.dinmat.U[1] = std::abs(temp.dinmat.U[1]);
						temp.dinmat.U[2] = std::abs(temp.dinmat.U[2]);
						atomcopy.push_back(temp);
					}
				}
				if (j % 5 == 4 && j != size_s - 1)
					Cleaning(atomcopy);
			}
			Cleaning(atomcopy);
			return atomcopy;
		}
		std::vector<nsShelxFile::Atom> GenerateSymmAtom(const Atom & a) const noexcept {
			const size_t size_s = symm.size();
			std::vector<Atom> atomcopy;
			atomcopy.push_back(a);
			for (size_t j = 0; j < size_s; j++) {
				size_t na = atomcopy.size();
				for (size_t i = 0; i < na; i++) {
					Atom temp(atomcopy[i]);
					for (size_t m = 1; m < symm[j].mult; m++) {
						temp.point = symm[j].GenSymmNorm(temp.point);
						temp.dinmat = symm[j].mat*temp.dinmat.ToMatrix();
						temp.dinmat.U[0] = std::abs(temp.dinmat.U[0]);
						temp.dinmat.U[1] = std::abs(temp.dinmat.U[1]);
						temp.dinmat.U[2] = std::abs(temp.dinmat.U[2]);
						atomcopy.push_back(temp);
					}
				}
				if (j % 5 == 4 && j != size_s - 1)
					Cleaning(atomcopy);
			}
			Cleaning(atomcopy);
			atomcopy.shrink_to_fit();
			return atomcopy;
		}
		void OutIns(std::ofstream & out) const {
			size_t syms = symm.size();
			out.precision(5);
			out.setf(std::stringstream::fixed);
			out << "TITL imported by nsShelxData by FEELIN" << std::endl;
			out << "CELL 0.71073 " << cell.Lat_dir(0) << ' ' << cell.Lat_dir(1) << ' ' << cell.Lat_dir(2) << ' '
				<< cell.Angle_grad(0) << ' ' << cell.Angle_grad(1) << ' ' << cell.Angle_grad(2) << std::endl;
			out << "ZERR 4 0.0000 0.0000 0.0000 0 0 0" << std::endl;
			out << "LATT " << LATT << std::endl;
			for (size_t i = 0; i < syms; i++) {
				if (symm[i].LATT == true) continue;
				out << "SYMM " << symm[i].sstr << std::endl;
			}
			out << "SFAC";
			const size_t size_sfac = sfac.size();
			for (size_t i = 0; i < size_sfac; i++) {
				out << " " << sfac[i];
			}
			out << std::endl << "UNIT";
			for (size_t i = 0; i < size_sfac; i++) {
				out << " " << unit[i];
			}
			out << std::endl
				<< "WGHT 0.1" << std::endl
				<< "FVAR 1" << std::endl;
			const size_t a_size = atom.size();
			for (size_t i = 0; i < a_size; i++) {
				out << atom[i].label << " " << atom[i].type << " "
					<< atom[i].point.a[0] << " "
					<< atom[i].point.a[1] << " "
					<< atom[i].point.a[2] << " ";
				out << atom[i].occup << " " << atom[i].dinmat.U[0] << " " << atom[i].dinmat.U[1] << " =" << std::endl
					<< atom[i].dinmat.U[2] << " " << atom[i].dinmat.U[5] << " " << atom[i].dinmat.U[4] << " " << atom[i].dinmat.U[3] << std::endl;
			}
			out << "END";
		}
		void OutPOSCAR() {
			std::ofstream out("POSCAR");
			
		}

		ShelxData() noexcept : LATT(-1) {}
		explicit ShelxData(std::istream & in)
		{
			char buf[128], buf4[5];
			unsigned int check(0u);
			while (in.eof() == false) {
				in.getline(buf, 127);
				std::stringstream input(buf);
				input >> buf4;
				_strupr_s(buf4);
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
					input >> LATT;
					if (LATT > 0)
						symm.push_back(SYMM("-x,-y,-z", true));
					switch (abs(LATT)) {
					case 1:
						break;
					case 2:
						symm.push_back(SYMM("x+0.5,y+0.5,z+0.5", true));
						break;
					case 3:
						symm.push_back(SYMM("x+2/3,y+1/3,z+1/3", true));
						break;
					case 4:
						symm.push_back(SYMM("x+0.5,y,z", true));
						symm.push_back(SYMM("x,y+0.5,z", true));
						symm.push_back(SYMM("x,y,z+0.5", true));
						break;
					case 5:
						symm.push_back(SYMM("x+0.5,y,z", true));
						break;
					case 6:
						symm.push_back(SYMM("x,y+0.5,z", true));
						break;
					case 7:
						symm.push_back(SYMM("x,y,z+0.5", true));
						break;
					}
				}
					check |= 0x0002;
					break;
				case 2: // SYMM
					input.getline(buf, 127);
					symm.push_back(SYMM(buf));
					break;
				case 3: // UNIT
					unit.push_back(0);
					for (int i = 1;; i++) {
						input >> buf;
						unit.push_back(atoi(buf));
						if (unit[i] == size_t(0)) break;
					}
					unit.pop_back();
					unit.shrink_to_fit();
					check |= 0x0004;
					break;
				case 4: // SFAC
					input >> buf4;
					sfac.push_back("");
					while (std::strcmp(buf4, "") != 0) {
						_strupr_s(buf4);
						sfac.push_back(buf4);
						input >> buf4;
					}
					sfac.shrink_to_fit();
					check |= 0x0008;
					break;
				case 5: // END
					symm.shrink_to_fit();
					atom.shrink_to_fit();
					return;
				default:
					if (M == 78) {
						if (check != 0x000F) {
							throw std::runtime_error(buf);
						}
						if (sfac.size() != unit.size()) {
							throw std::runtime_error("SFAC and/or UNIT contain error");
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
							point.a[i] = (flo)atof(temp);
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
									input.flush();
									in.getline(buf, 127);
									input.str(std::string());
									input.clear();
									input << buf;
									input >> temp;
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
		ShelxData(ShelxData && in) noexcept(_MoveNothrow) : cell(std::move(in.cell)), symm(std::move(in.symm)),	
															atom(std::move(in.atom)), sfac(std::move(in.sfac)) {}
		void operator=(const ShelxData & in) = delete;
		void operator=(ShelxData && in) noexcept(_MoveNothrow) {
			cell = (std::move(in.cell));
			symm = (std::move(in.symm));
			atom = (std::move(in.atom));
			sfac = (std::move(in.sfac));
			unit = (std::move(in.unit));
			LATT = in.LATT;
		}
	};
}
#endif