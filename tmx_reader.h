#pragma once
#ifndef TMX_reader
#define TMX_reader


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <stdlib.h>

class tage
{
private:
	std::string tage_name;
	std::string body;
	std::vector <tage> v_subtages;
	std::map <std::string, std::string> characteristics;
public:
	bool init(std::string);

	std::string get_tage_name();
	std::string get_characteristic(std::string);
	std::string get_body();
	std::vector <tage> get_v_subtages();
	std::vector <tage> get_v_subtages_with_name(std::string);
};

class tmx_file
{
private:
	std::vector <tage> v_general_tages;
	std::string version;
	std::string encoding;
public:
	bool load_from_file(std::string);

	std::vector <tage> get_v_general_tages();
	std::vector <tage> get_v_general_tages_with_tage_name(std::string);
	std::string get_version();
	std::string get_encoding();
};

#endif