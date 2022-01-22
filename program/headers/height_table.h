
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include "log.h"
#include "metrics.h"


class HeightTable
{
public:
	HeightTable(std::string file_name);
	~HeightTable();

	double height_at(double lat, double lon);
protected:
	std::ifstream _file;
	std::vector<std::streampos> _lines;
};


