#pragma once
#include <string>
#include <json.hpp>
#include <fstream>

inline nlohmann::json load_json_from_file(std::string path)
{
	std::ifstream file(path);
	std::string str;

	file.seekg(0, std::ios::end);
	str.reserve(file.tellg());
	file.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return nlohmann::json::parse(str);
}

