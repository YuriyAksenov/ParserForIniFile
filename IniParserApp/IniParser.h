#ifndef INI_PARSER_H
#define INI_PARSER_H
#include<string>
#include <vector>
#include<map>

#include "IniParserExceptions.h"

class IniParser
{
private:
	enum FieldKeyType
	{
		none = -1,
		comment_or_space = 0,
		section = 1,
		field = 2
	};
	struct Field {
	private:
		std::string key;
		std::string value;
	public:
		Field(std::string, std::string);
		std::string getKey();
		std::string getValue();
		bool isEmpty();
		std::string toString();
	};
	std::map<std::string, std::vector<Field>> IniParser::sections;
	FieldKeyType IniParser::defineString(const std::string);
	Field IniParser::parseField(const std::string);
	std::string IniParser::parseSection(const std::string);
	bool IniParser::isCommnetOrSpace(const std::string);
	bool IniParser::isSection(const std::string);
	bool IniParser::isField(const std::string);
	bool IniParser::isValid(const std::string);
	std::string IniParser::trim(const std::string);

public:
	IniParser::IniParser(const std::string);
	IniParser::~IniParser();

	std::string getValue(const std::string, const std::string, const std::string);
	std::vector<std::string> IniParser::getSections();
	std::vector<std::string> IniParser::getFields(const std::string);
	bool IniParser::isContainingSection(const std::string);
	bool IniParser::isContainingFieldInSection(const std::string, const std::string);
	std::string IniParser::toString();
};

#endif //INI_PARSER_H
