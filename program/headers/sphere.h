
#pragma once


#include "point.h"
#include "vector.h"
#include "metrics.h"
#include "trans_matrix.h"
#include "bz_curve.h"
#include "log.h"

#include <cstdarg>
#include <vector>
#include <array>
#include <utility>
#include <stdexcept>
#include <cmath>



namespace earth {
	double radius();

	Conversion flatting_conv();

	Vector norm(Point p);
	Vector norm(double lat, double lon);

	double local_R(double lat, double lon);
	double local_R(Point p);

	double H(Point p);

	Point geo(double h, double lat, double lon);

	double course(Point p, Vector vec);
	Vector course_to_vec(Point p, double course);


	double Fg(double h);

	double k(double h);
}


// ngs.noaa.gov/PUBS_LIB/inverse.pdf
//
// Vol. XXIII		No. 176
//
// SURVEY REVIEW
//
// April, 1975

//a, b - major and minor semiaxes of ellipsoid
//f - flattening = (a - b) / a;
//lat - geodetic latitude
//L - difference in longitude
//s - length of the geodesic
//z1, z2 - azimuths of the geodesic. z2 in the direction P1 P2 produced
//z - azimuth of the geodesic at the equatur
//u^2 = cos(z)^2 * (a^2-b^2)/b^2
//U - reduced latitude defined by tan(U) = (1 - f) * tan(lat)
//k - difference in longitude on an auxiliary sphere
//o - angular distance P1 P2 on the sphere
//o1 - angular distance from the equatur to P1
//om - angular distance from the equatur to the midpoint of the line

//Direct Formula
//in : lat1, z1, s
//out : lat2, L, z2
bool direct(const double& lat1, const double& z1, const double& s, double& lat2, double& L, double& z2);

//Inverse Formula
//in : lat1, lat2, L
//out : s, z1, z2
bool inverse(const double& lat1, const double& lat2, const double& L, double& s, double& z1, double& z2);


std::vector<BzCurve> orthodoxy(const Point& first, const Point& second, Vector* direction = nullptr);

