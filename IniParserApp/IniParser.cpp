#include "stdafx.h"
#include "IniParser.h"

#include <iostream>
#include <string> // подключаем строки
#include <fstream> // подключаем файлы
#include<map>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::ifstream;



IniParser::Field::Field(string key = "", string value = "") {
	this->key = key;
	this->value = value;
}

string IniParser::Field::getKey() {
	return this->key;
}

string IniParser::Field::getValue() {
	return this->value;
}


bool IniParser::Field::isEmpty() {
	return key.empty();
}

string IniParser::Field::toString() {
	return !isEmpty() ? key + " = " + value : "";
}

IniParser::IniParser(const string fileName)
{

	ifstream file(fileName);
	string str;

	string section = "";
	vector<Field> fields = vector<Field>();

	while (getline(file, str))
	{
		switch (defineString(str)) {
		case(FieldKeyType::section): {
			string newSection = parseSection(str);
			if (section != newSection) {
				if (!section.empty()) {
					sections.insert(pair<string, vector<Field>>(section, fields));
				}
				section = newSection;
				fields.clear();
			}
			break;
		}
		case(FieldKeyType::field): {
			Field field = parseField(str);
			fields.push_back(field);
			break;
		}
		default: {
			break;
		}
		}
	}

	if (!section.empty() && file.eof()) {
		sections.insert(pair<string, vector<Field>>(section, fields));
		fields.clear();
	}
	else if(section.empty())
	{
		throw ValidSectionException("filename", "File does not contain any section.");
	}

	file.close();
}


string IniParser::getValue(const string section, const  string field, const string type_default) {
	for (auto item : sections.find(section)->second)
	{
		if (item.getKey().compare(field) == 0) return item.getValue();
	}
	return type_default;
}

vector<string> IniParser::getSections() {
	vector<string> sectionsReturn = vector<string>();
	for (auto item : sections)
	{
		sectionsReturn.push_back(item.first);
	}
	return sectionsReturn;
}

vector<string> IniParser::getFields(const string section) {
	vector<string> fieldsReturn = vector<string>();
	for each (auto item in sections.find(section)->second)
	{
		fieldsReturn.push_back(item.getKey());
	}
	return fieldsReturn;
}

bool IniParser::isContainingSection(const string section) {
	return sections.find(section) != sections.end();
}

bool IniParser::isContainingFieldInSection(const string section, const string field) {
	for (auto item : sections.find(section)->second)
	{
		if (item.getKey().compare(field) == 0) return true;
	}
	return false;
}

string IniParser::toString() {
	string str = "";
	for each (auto section in sections)
	{
		str = str + "[" + section.first + "]" + "\n";
		for (Field field : section.second)
		{
			if (!field.isEmpty())str = str + "\t" + field.toString() + "\n";
		}
	}
	return str;
}

IniParser::~IniParser()
{
	for (auto section : sections)
	{
		section.second.clear();
	}
	sections.clear();
}

IniParser::FieldKeyType IniParser::defineString(const string str) {
	if (isCommnetOrSpace(str)) return FieldKeyType::comment_or_space;
	if (isSection(str)) return FieldKeyType::section;
	if (isField(str)) return FieldKeyType::field;
	return FieldKeyType::none;
}

string IniParser::parseSection(const string str) {
	if (str[0] == '[' && str[1] == ']') throw  ValidSectionException::ValidSectionException(str, "It is null length of section.");//NOTVALIDSECTIONEXCEPTION NULL LENGTH OF NAME SECTION
	int iteratorForClosedBracket = 0;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == ' ' || str[i] == ';') throw ValidSectionException::ValidSectionException(str, "Spaces or special symbols are between '[' and ']'."); //NOTVALIDSECTIONEXCEPTION SOME SPACES OR SPECIAL SYMBOLS LIKE ; 
		if (str[i] == ']') {
			iteratorForClosedBracket = i;
			break;
		}
	}
	return str.substr(1, iteratorForClosedBracket - 1);
}

IniParser::Field IniParser::parseField(const string str) {


	int equalPos = (str.find('=') != string::npos ? str.find('=') : throw ValidFieldException::ValidFieldException(str, "It is not a field.")); // NOT FIELD

	int commentPos = str.find(';') != string::npos ? str.find(';') : str.length() + 1;

	if (commentPos < equalPos)  throw ValidFieldException::ValidFieldException(str, "It is not a comment and field, because ';' is placed before '='."); // Comment is places before equating

	string keyBuf = trim(str.substr(0, equalPos - 1));

	if (keyBuf.empty() || !isValid(keyBuf)) throw ValidFieldException::ValidFieldException(str, "The field (key of the property) is spaced."); // данные не валидные есть пробел или пустой

	int iteratorForLastValuePosition = str.length() - 1;
	for (int i = equalPos + 1; i < str.length(); i++)
	{
		if (str[i] == ';') {
			iteratorForLastValuePosition = i - 1;
			break;
		}
	}

	string valueBuf = trim(str.substr(equalPos + 1, iteratorForLastValuePosition - equalPos));

	if (!valueBuf.empty() && !isValid(valueBuf)) throw ValidFieldException::ValidFieldException(str, "The field (value of the property) is spaced."); // данные не валидные есть пробел или пустой

	Field field = Field(keyBuf, valueBuf);
	return field;
}

string IniParser::trim(const string str) {

	int i = 0;
	while (str[i] == ' ' && i != str.size())
	{
		i++;
	}

	int j = str.size() - 1;
	while (str[j] == ' ' && j != 0)
	{
		j--;
	}

	if (j < i) return "";

	string forReturned((str.size() - (str.size() - j + i)) + 1, ' ');

	for (int k = 0; k < forReturned.size(); k++)
	{
		forReturned[k] = str[i + k];
	}
	return forReturned;
}


bool IniParser::isCommnetOrSpace(const string str) {
	return str[0] == ' ' || str[0] == ';';
}

bool IniParser::isField(const string str) {
	return str.find('=') != string::npos;
}

bool IniParser::isSection(const string str) {
	return str[0] == '[';
}

bool IniParser::isValid(const string str) {
	for (auto var : str)
	{
		if (var == ' ' || var == ';') return false;
	}
	return true;
}
