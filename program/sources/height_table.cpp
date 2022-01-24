
#include "height_table.h"

HeightTable::HeightTable(std::string file_name)
{
	_file.open(file_name);
	if (_file.is_open()) {
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "file ("+file_name+") opened");
	} else {
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "cant open file ("+file_name+")");
		return ;
	}
	std::string buf;
	std::streampos pos = 0;
	_lines.push_back(pos);
	std::cout << "pos" << std::endl;
	while (std::getline(_file, buf)) {
		pos = _file.tellg();
		_lines.push_back(pos);
	}
	_lines.pop_back();
	_file.clear();
}


HeightTable::~HeightTable()
{
	_file.close();
}


double HeightTable::height_at(double lat, double lon) 
{
	_file.clear();
	if (!_file.is_open()) {
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "height table is't opened");
		return 0;
	}
	std::string buf;
	for (auto pos : _lines) {
		_file.seekg(pos, _file.beg);
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
		double glat, glon, gh;

		std::string err_str;
		if (0 < parts.size())
			glat = std::stod(parts[0]);
		else
			err_str += "cant read latitude from pos (" + std::to_string(pos) + ") ";
		if (1 < parts.size())
			glon = std::stod(parts[1]);
		else
			err_str += "cant read longitude from pos (" + std::to_string(pos) + ") ";
		if (2 < parts.size())
			gh = std::stod(parts[2])/1000;
		else
			err_str += "cant read height from pos (" + std::to_string(pos) + ") ";
		if (err_str.size() > 0)
			my_log::log_it(my_log::level::error, __FUNCTION_NAME__, err_str);
		if (equal(lat, glat) && equal(lon, glon)) {
			return gh;
		}
	}
}

