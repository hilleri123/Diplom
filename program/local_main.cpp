
#include <iostream>
#include <fstream>
#include <cstring>

//#include "function.h"
//#include "csv_parser.h"
//#include "space/functions/velocity.h"
#include "function.h"
#include "csv_parser.h"
#include "velocity.h"

#include <boost/program_options.hpp>

const Vector ox(Point(1,0,0));
const Vector oy(Point(1,0,0));
const Vector oz(Point(0,0,1));

void print(Rotate a, std::string out)
{
	double h = 0.01;
	//std::string out = "kaka.txt";
	out += ".txt";
	std::ofstream stream;

	if (out.size() > 0) {
		std::cout << out << std::endl;
		stream.open(out, std::ios_base::app);
		if (!stream.is_open()) {
			throw std::ofstream::failure("outfile not open");
		}
	}
	std::cout << h << std::endl;

	for (double time = 0; time < a.max_time()+h; time+=h) {
		if (stream.is_open()) {
			//stream << time << " " << a(time).radius() << " " << a(time).latitude() << " " << a(time).longitude() << std::endl;
			//stream << time << " " << a(time).x() << " " << a(time).y() << " " << a(time).z() << std::endl;
			stream << a(time).x() << " " << a(time).y() << " " << a(time).z() << std::endl;
		} else {
			//std::cout << time << " " << a(time).radius() << " " << a(time).latitude() << " " << a(time).longitude() << std::endl;
			//std::cout << time << " " << a(time).x() << " " << a(time).y() << " " << a(time).z() << std::endl;
			std::cout << a(time).x() << " " << a(time).y() << " " << a(time).z() << std::endl;
		}
	}
	if (stream.is_open()) {
		stream.close();
	}
	//delete &data;
}

void get_AB(const Vector& dir, Matrix& to, Matrix& from)
{
	to = Matrix(Matrix::move, &dir) * Matrix();
	from = Matrix() * Matrix(Matrix::move, -1*dir);
}

void get_BC(const Vector& dir, Matrix& to, Matrix& from)
{
	to = Matrix::move(dir) * Matrix();
	from = Matrix() * Matrix::move(-1*dir);
	to = Matrix::rotate(ox, atan(1)*2) * to;
	from *= Matrix::rotate(-1*ox, atan(1)*2);
}

void get_CD(Vector dir, Matrix& to, Matrix& from)
{
	to = Matrix::move(dir) * Matrix();
	from = Matrix() * Matrix::move(-1*dir);
	//to = Matrix::rotate(oz, -atan(1)*2) * to;
	//from *= Matrix::rotate(-1*oz, -atan(1)*2);
	//to *= Matrix::rotate(oz, atan(1)*2);
	//from = Matrix::rotate(-1*oz, atan(1)*2) * from;
}

void get_DE(Vector dir, Matrix& to, Matrix& from)
{
	//to = Matrix::move(dir) * Matrix();
	//from = Matrix() * Matrix::move(-1*dir);
	to = Matrix() * Matrix::move(dir);
	from = Matrix::move(-1*dir) * Matrix();
	//to = Matrix::rotate(oz, -atan(1)*4) * to;
	//from *= Matrix::rotate(-1*oz, -atan(1)*4);
	to *= Matrix::rotate(oz, atan(1)*3);
	from = Matrix::rotate(-1*oz, atan(1)*3) * from;
}

void get_EF(Vector dir, Matrix& to, Matrix& from)
{
	to = Matrix::move(dir) * Matrix();
	from = Matrix() * Matrix::move(-1*dir);
	//to = Matrix::rotate(oz, -atan(1)*6) * to;
	//from *= Matrix::rotate(-1*oz, -atan(1)*6);
	to *= Matrix::rotate(oz, atan(1)*5);
	from = Matrix::rotate(-1*oz, atan(1)*5) * from;
}


int main(int argc, char** argv)	
{	
	
	std::string out = "kaka";

	std::ofstream _stream0(out+"0"+".txt");
	std::ofstream _stream1(out+"1"+".txt");
	std::ofstream _stream2(out+"2"+".txt");
	std::ofstream _stream3(out+"3"+".txt");

	//DOESNT WORK
	Vector d(Point(0,0,0));
	Vector dd(Point(0,0,0));
	
	Velocity v(3);

	Point A(0,0,0);
	Point B(5,0,0);
	Point C(60,0,50);
	Point D(60,40,50);
	Point E(-40,40,50);
	Point F(-40,-40,50);
	Point G(60,-40,50);

	Point S0(150,10,50);
	Point S1(80,80,50);
	Point S2(-90,0,50);
	Point S3(-10,-90,50);



	Vector dir;
	double angl;
	Matrix to, from;

	//TRAJ 0
	std::vector<Point> v0;

	v0.push_back(A+dd);
	v0.push_back(B+dd);
	v0.push_back(C+dd);
	v0.push_back(S0+dd);

	dd = dd + d;
	//TRAJ 1
	std::vector<Point> v1;

	v1.push_back(A+dd);
	v1.push_back(B+dd);
	v1.push_back(C+dd);
	v1.push_back(D+dd);
	v1.push_back(S1+dd);

	dd = dd + d;

	//TRAJ 2
	std::vector<Point> v2;

	v2.push_back(A+dd);
	v2.push_back(B+dd);
	v2.push_back(C+dd);
	v2.push_back(D+dd);
	v2.push_back(E+dd);
	v2.push_back(S2+dd);

	dd = dd + d;

	//TRAJ 3
	std::vector<Point> v3;

	v3.push_back(A+dd);
	v3.push_back(B+dd);
	v3.push_back(C+dd);
	v3.push_back(D+dd);
	v3.push_back(E+dd);
	v3.push_back(F+dd);
	v3.push_back(S3+dd);

	dd = dd + d;


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	dir = Vector(v0[0], v0[1]);
	get_AB(Vector(Point(0,0,0), v0[0]), to, from);
	print(Rotate(from(v0[1]), from(dir), v, to), out+"0");

	get_BC(dir, to, from);
	print(Rotate(from(v0[2]), from(dir), v, to), out+"0");

	dir = Vector(v0[0], v0[2]);
	angl = Vector(C,S0)^ox;

	Matrix::multiplay_foreward_backward(from, to, Matrix::move, &dir);
	//to = Matrix::move(dir) * Matrix();
	//from = Matrix() * Matrix::move(-1*dir);
	//to = Matrix::rotate(ox, angl) * to;
	//from *= Matrix::rotate(-1*ox, angl);
	print(Rotate(from(v0[3]), from(Vector(C, S0)), v, to), out+"0");
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	dir = Vector(v1[0], v1[1]);
	get_AB(Vector(Point(0,0,0), v1[0]), to, from);
	print(Rotate(from(v1[1]), from(dir), v, to), out+"1");

	get_BC(dir, to, from);
	print(Rotate(from(v1[2]), from(dir), v, to), out+"1");

	dir = Vector(v1[0], v1[2]);
	get_CD(dir, to, from);
	print(Rotate(from(v1[3]), from(Vector(D, S1)), v, to), out+"1");

	dir = Vector(v1[0], v1[3]);
	angl = Vector(D,S1)^ox;
	Matrix::multiplay_foreward_backward(from, to, Matrix::move, &dir);
	//to = Matrix::move(dir) * Matrix();
	//from = Matrix() * Matrix::move(-1*dir);
	//to = Matrix::rotate(oz, angl) * to;
	//from *= Matrix::rotate(-1*oz, angl);
	Matrix::multiplay_foreward_backward(to, from, Matrix::rotate, &dir, angl);
	//to *= Matrix::rotate(oz, angl);
	//from = Matrix::rotate(-1*oz, angl) * from;
	print(Rotate(from(v1[4]), from(Vector(D,S1)), v, to), out+"1");
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	dir = Vector(v2[0], v2[1]);
	get_AB(Vector(Point(0,0,0), v2[0]), to, from);
	print(Rotate(from(v2[1]), from(dir), v, to), out+"2");

	get_BC(dir, to, from);
	print(Rotate(from(v2[2]), from(dir), v, to), out+"2");

	dir = Vector(v2[0], v2[2]);
	get_CD(dir, to, from);
	print(Rotate(from(v2[3]), from(Vector(D, E)+Vector(C, D)), v, to), out+"2");

	dir = Vector(v2[0], v2[3]);
	get_DE(dir, to, from);
	print(Rotate(from(v2[4]), from(Vector(E, S2)), v, to), out+"2");
	
	dir = Vector(v2[0], v2[4]);
	angl = Vector(E,S2)^ox;
	Matrix::multiplay_foreward_backward(from, to, Matrix::move, &dir);
	//to = Matrix::move(dir) * Matrix();
	//from = Matrix() * Matrix::move(-1*dir);
	//to = Matrix::rotate(oz, angl) * to;
	//from *= Matrix::rotate(-1*oz, angl);
	Matrix::multiplay_foreward_backward(to, from, Matrix::rotate, &oz, angl);
	//to *= Matrix::rotate(oz, -1*angl);
	//from = Matrix::rotate(-1*oz, -1*angl) * from;
	print(Rotate(from(v2[5]), from(Vector(E,S2)), v, to), out+"2");
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	dir = Vector(v3[0], v3[1]);
	get_AB(Vector(Point(0,0,0), v3[0]), to, from);
	print(Rotate(from(v3[1]), from(dir), v, to), out+"3");

	get_BC(dir, to, from);
	print(Rotate(from(v3[2]), from(dir), v, to), out+"3");

	dir = Vector(v3[0], v3[2]);
	get_CD(dir, to, from);
	//print(Rotate(from(v3[3]), from((-1*ox)+oy), v, to), out+"3");
	print(Rotate(from(v3[3]), from(Vector(D, E)+Vector(C, D)), v, to), out+"3");

	dir = Vector(v3[0], v3[3]);
	get_DE(dir, to, from);
	print(Rotate(from(v3[4]), from(-1*ox+(-1)*oy), v, to), out+"3");

	dir = Vector(v3[0], v3[4]);
	get_EF(dir, to, from);
	print(Rotate(from(v3[5]), from(Vector(F, S3)), v, to), out+"3");
	
	dir = Vector(v3[0], v3[5]);
	angl = Vector(F,S3)^ox;
	Matrix::multiplay_foreward_backward(from, to, Matrix::move, &dir);
	//to = Matrix::move(dir) * Matrix();
	//from = Matrix() * Matrix::move(-1*dir);
	//to = Matrix::rotate(oz, angl) * to;
	//from *= Matrix::rotate(-1*oz, angl);
	Matrix::multiplay_foreward_backward(to, from, Matrix::rotate, &oz, -1*angl);
	//to *= Matrix::rotate(oz, -1*angl);
	//from = Matrix::rotate(-1*oz, -1*angl) * from;
	print(Rotate(from(v3[6]), from(Vector(F,S3)), v, to), out+"3");
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	std::cout << "---------------------------" << std::endl;

	out = "popa";

	std::vector<std::pair<Point, Vector>> dists;
	dists.push_back(std::make_pair(Point(10, 10, 0), Vector(Point(0,-10,0))));
	dists.push_back(std::make_pair(Point(-10, -10, 0), Vector(Point(10,10,0))));
	dists.push_back(std::make_pair(Point(10, 0, 0), Vector(Point(10,0,0))));

	for (auto i = dists.begin(); i < dists.end(); i++) {
		std::string str = std::to_string(i-dists.begin());
		std::ofstream _stream_tmp(out+str+".txt");
		Point& p = std::get<0>(*i);
		Vector& vv = std::get<1>(*i);
		print(Rotate(p, vv, v, Matrix()), out+str);
	}

	return 0;
};

