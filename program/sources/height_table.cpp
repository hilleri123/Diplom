
#include "height_table.h"


std::size_t HashForTable::operator()( const std::pair<double, double>& pair) const
{
	std::size_t seed = 0;
#ifdef USE_BOOST_HASH
	boost::hash_combine(seed, ceil(pair.first*_round_d));
	boost::hash_combine(seed, ceil(pair.second*_round_d));
#else
	std::hash<double> h;
	seed = h(ceil(pair.first*_round_d))^h(ceil(pair.second*_round_d));
#endif
	return seed;
}

bool CompareForTable::operator()( const std::pair<double, double>& lhs, const std::pair<double, double>& rhs ) const
{
	//return more(lhs.first+_delta, rhs.first) && less(lhs.first-_delta, rhs.first) && more(lhs.second+_delta, rhs.second) && less(lhs.second-_delta, rhs.second);
	return equal(lhs.first, rhs.first) && equal(lhs.second, rhs.second);
}


HeightTable::HeightTable(std::string file_name)
{
#ifdef HASH_TABLE
	std::ifstream _file;
#else
	std::streampos pos = 0;
	_lines.push_back(pos);
#endif
	_file.open(file_name);
	if (_file.is_open()) {
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "file ("+file_name+") opened");
	} else {
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "cant open file ("+file_name+")");
		return ;
	}
	std::string buf;
	//std::cout << "push_back" << std::endl;
	while (std::getline(_file, buf)) {
#ifdef HASH_TABLE
		buf.erase(std::remove_if(buf.begin(), buf.end(), [](char c){ return std::isspace(c); }), buf.end());
		std::vector<std::string> parts;
		auto begin = buf.begin();
		auto end = buf.begin();
		for (; end < buf.end(); end++) {
			//std::cout << *end << std::endl;
			if (*end == ',') {			// csv
				std::string tmp(begin, end);
				parts.emplace_back(tmp);
				begin = end + 1;
			}
		}
		if (begin != end) {
			std::string tmp(begin, end);
			parts.emplace_back(tmp);
		}
		assert(parts.size() == 3);
		if (parts.size() == 3) {
			static const double cof = atan(1) / 45.;
			double lat = std::stod(parts[0])*cof;
			double lon = std::stod(parts[1])*cof;
			double h = std::stod(parts[2])/1000.;
			
			//if (_hash_table.find(std::make_pair(lat, lon)) == _hash_table.end()) {
				//_hash_table[lat] = lon_map();
			//}
			_hash_table[std::make_pair(lat, lon)] = h;
			my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "set data for lat("+std::to_string(lat)+
					") lon("+std::to_string(lon)+") h("+std::to_string(h)+")");
					//") lat_size("+std::to_string(_hash_table.size())+
					//") lon_size("+std::to_string(_hash_table[lat].size())+")");
		}
#else
		pos = _file.tellg();
		_lines.push_back(pos);
#endif
	}
#ifdef HASH_TABLE
#if 0
	for (auto i = _hash_table.begin(); i != _hash_table.end(); ++i) {
		std::cout << i->first << " !!! ";
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
			std::cout << "(" << j->first << "," << j->second << ") ";
		std::cout << std::endl;
	}
#endif
#if 0
	for (auto i = _hash_table.begin(); i != _hash_table.end(); ++i) {
		std::cout << "(" << i->first.first << "," << i->first.second << ") = " << i->second << std::endl;
	}
#endif
#else
	_lines.pop_back();
#endif
	//std::cout << "pop_back" << std::endl;
	_file.clear();
}


HeightTable::~HeightTable()
{
#ifndef HASH_TABLE
	_file.close();
#endif
}

#ifdef HASH_TABLE
double HeightTable::height_at(double lat, double lon)
{
#if 0
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << _hash_table.size() << " " << _hash_table[lat].size() << std::endl;
	for (auto i = _hash_table[lat].begin(); i != _hash_table[lat].end(); ++i)
		std::cout << "[" << lat << "," << lon << ",(" << i->second << "/" << _hash_table[lat][lon] << ")] ";
	std::cout << std::endl;
	std::cout << "in (" << lat << "," << lon << ") out " << _hash_table[lat][lon] << std::endl;
	auto lat_it = _hash_table.find(lat);
	if (lat_it != _hash_table.end()) {
		auto lon_it = lat_it->second.find(lon);
		if (lon_it != lat_it->second.end()) {
			my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "data for lat("+std::to_string(lat_it->first)+
					") lon("+std::to_string(lon_it->first)+" h("+std::to_string(lon_it->second)+") found");
			return lon_it->second;
		}
	}
#endif
#if 0
	for (auto i = _hash_table.begin(); i != _hash_table.end(); ++i) {
		if (equal(i->first.first, lat) && equal(i->first.second, lon))
			std::cout << "!!!(" << i->first.first << "," << i->first.second << ") = " << i->second << std::endl;
	}
#endif
	
	auto it = _hash_table.find(std::make_pair(lat, lon));
	if (it != _hash_table.end()) {
		return it->second;
	}
	my_log::log_it(my_log::level::warning, __FUNCTION_NAME__, "cant find data for lat("+std::to_string(lat)+
			") lon("+std::to_string(lon)+")");
	return 0;
}
#else
double HeightTable::height_at(double lat, double lon, double delta) 
{
	_file.clear();
	if (!_file.is_open()) {
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "height table is't opened");
		return 0;
	}
	std::string buf;
	for (auto pos : _lines) {
		_file.seekg(pos, _file.beg);
		//std::cout << "pos " << pos << std::endl;
		if (!std::getline(_file, buf)) {
			std::string err_str = "failed read line from " + std::to_string(pos) + " pos buf("+buf+")"; 
			my_log::log_it(my_log::level::error, __FUNCTION_NAME__, err_str);
			return 0;
		}
		buf.erase(std::remove_if(buf.begin(), buf.end(), [](char c){ return std::isspace(c); }), buf.end());

		std::vector<std::string> parts;
		auto begin = buf.begin();
		auto end = buf.begin();
		for (; end < buf.end(); end++) {
			//std::cout << *end << std::endl;
			if (*end == ',') {			// csv
				std::string tmp(begin, end);
				parts.emplace_back(tmp);
				begin = end + 1;
			}
		}
		if (begin != end) {
			std::string tmp(begin, end);
			parts.emplace_back(tmp);
		}
		assert(parts.size() == 3);

		//double cof = 1;

		double cof = atan(1)/45.;
		// -10000 - такого значения точно не будет
		double glat = -10000, glon = -10000, gh = -10000;

		std::string err_str;
		if (0 < parts.size())
			glat = std::stod(parts[0]) * cof;
		else
			err_str += "cant read latitude from pos (" + std::to_string(pos) + ") ";
		if (1 < parts.size())
			glon = std::stod(parts[1]) * cof;
		else
			err_str += "cant read longitude from pos (" + std::to_string(pos) + ") ";
		if (2 < parts.size())
			gh = std::stod(parts[2])/1000;
		else
			err_str += "cant read height from pos (" + std::to_string(pos) + ") ";
		if (err_str.size() > 0)
			my_log::log_it(my_log::level::error, __FUNCTION_NAME__, err_str);
		if (equal(lat+delta, glat) || equal(lat-delta, glat))
			if (equal(lon+delta, glon) || equal(lon-delta, glon))
				return gh;
		//if (equal(lat, glat) && equal(lon, glon)) {
			//return gh;
		//}
	}
	my_log::log_it(my_log::level::warning, __FUNCTION_NAME__, "cant find data for lat("+std::to_string(lat)+
			") lon("+std::to_string(lon)+")");
	return 0;
}
#endif

