#pragma once
#include<string>
#include <vector>

class IniParser
{
public:
	IniParser(const std::string);
	~IniParser();

	template<typename T>
	T getValue(const std::string, const std::string, const T);
	std::vector<std::string> getSections();
	std::vector<std::string> getFields(const std::string);
	bool IniParser::isContainingSection(const std::string);
	bool IniParser::isContainingFieldInSection(const std::string, const std::string);
	std::string IniParser::toString();
};

