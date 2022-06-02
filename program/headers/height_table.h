
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>
#include <unordered_map>
#include <functional>
#include <utility>

#include "log.h"
#include "metrics.h"

#ifdef USE_BOOST_LOG
#include <boost/functional/hash.hpp>
#endif
#define HASH_TABLE

class HashForTable
{
public:
	std::size_t operator()( const std::pair<double, double>& pair ) const;
protected:
	uint _round_d = 100000;
};
class CompareForTable
{
public:
	bool operator()( const std::pair<double, double>& lhs, const std::pair<double, double>& rhs ) const;
protected:
	const double _delta = 0.004;
};

class HeightTable
{
public:
	HeightTable(std::string file_name);
	~HeightTable();

#ifdef HASH_TABLE
	double height_at(double lat, double lon);
#else
	double height_at(double lat, double lon, double delta = 0.001);
#endif
protected:
#ifdef HASH_TABLE
	//using lon_map = std::unordered_map<double, double, std::hash<double>, CompareForTable>;
	//using lat_lon_map = std::unordered_map<double, lon_map, std::hash<lon_map>, CompareForTable>;
	//using lon_map = std::unordered_map<double, double>;
	//using lat_lon_map = std::unordered_map<double, lon_map>;
	using lat_lon_map = std::unordered_map<std::pair<double, double>, double, HashForTable, CompareForTable>;
	lat_lon_map _hash_table;
#else
	std::ifstream _file;
	std::vector<std::streampos> _lines;
#endif
};


