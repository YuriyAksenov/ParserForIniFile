#ifndef INI_PARSER_H
#define INI_PARSER_H
#include<string>
#include <vector>

#include "IniParserException.h"

class IniParser
{
public:
	IniParser::IniParser(const std::string);
	IniParser::~IniParser();

	template<typename T>
	T getValue(const std::string, const std::string, const T);
	std::vector<std::string> IniParser::getSections();
	std::vector<std::string> IniParser::getFields(const std::string);
	bool IniParser::isContainingSection(const std::string);
	bool IniParser::isContainingFieldInSection(const std::string, const std::string);
	std::string IniParser::toString();
};

#endif //INI_PARSER_H
