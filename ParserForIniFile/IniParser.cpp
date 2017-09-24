#include "stdafx.h"
#include "IniParser.h"

#include <iostream>
#include <string> // подключаем строки
#include <fstream> // подключаем файлы
#include<map>

using std::string;
using std::vector;
using std::map;
using std::ifstream;


struct Field {
	string key;
	string value;

	Field(string key = "", string value = "") {
		this->key = key;
		this->value = value;
	}

	bool isEmpty() {
		return key.empty() || value.empty();
	}
};

map<string, vector<Field>> sections;
ifstream file;

void parse(const string);
Field getField(const string);
string trim(const string);
bool isValid(const string);
string getNameOfSection(const string);
bool isCommnetOrSpace(const string);
bool isSection(const string);
string defineString(const string);


IniParser::IniParser(const  string fileName)
{

	ifstream file(fileName);
	string str;
	while (getline(file, str))
	{
		defineString(str);
	}

	file.close();
}

template<typename T>
T IniParser::getValue(const string section, const  string field, const T type_default) {
	for (auto item : sections.find(section)->second)
	{
		if (item.Key.compare(field) == 0) return item.Value;
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
		fieldsReturn.push_back(item.key);
	}
	return fieldsReturn;
}

bool IniParser::isContainingSection(const string section) {
	return sections.find(section) != sections.end();
}

bool IniParser::isContainingFieldInSection(const string section, const string field) {
	for (auto item : sections.find(section)->second)
	{
		if (item.key.compare(field) == 0) return true;
	}
	return false;
}

string IniParser::toString() {
	string str = "";
	for each (auto section in sections)
	{
		str = str + "[" + section.first + "]" + "\n";
		for each (Field field in section.second)
		{
			str = str + "\t" + field.key + " = " + field.value + "\n";
		}
	}
	return str;
}

IniParser::~IniParser()
{
	sections.clear();
	file.close();
}

string defineString(const string str) {
	if (isSection(str)) return "section";
	if (isField(str)) return "field";
	return "comment_or_space";
}



Field getField(const string str) {
	Field field = Field();

	string keyBuf = "";

	int i = 0;
	while (str[i] != '=') i++;
	keyBuf = trim(str.substr(0, i));

	if (!isValid(keyBuf)) throw _exception(); // данные не валидные есть пробел

	i++;



	return field;
}

string trim(const string str) {

	int i = 0;
	while (str[i] == ' ' || i != str.size())
	{
		i++;
	}
	int j = str.size();
	while (j == ' ' || i != 0)
	{
		j--;
	}

	if (j <= i) return "";

	string forReturned((str.size() - (str.size() - j + i)),' ');

	for (int k = 0; k < forReturned.size(); k++)
	{
		forReturned[k] = str[i + k];
	}

	return forReturned;

}

bool isValid(const string str) {
	for (auto var : str)
	{
		if (var == ' ' || ';') return false;
	}
	return true;
}

string getNameOfSection(const string str) {
	int iteratorForOpenedBracket = 0;
	int iteratorForClosedBracket = 0;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == ' ' || str[i] == ';') throw _exception();
		if (str[i] == ']') {
			iteratorForClosedBracket = i;
			break;
		}
	}
	return str.substr(1, iteratorForClosedBracket - 1);
}

bool isCommnetOrSpace(const string str) {
	for (auto item : str)
	{
		if (item != ' ' || item != ';') return false;
		else if (item == ';') return true;

	}
	return true;
}

bool isField(const string str) {

}

bool isSection(const string str) {


	if (str[0] != '[' || str[0] == '[' && str[1] == ']') return false;

	int i = 1;
	while (i != str.size())
	{
		if (str[i] == ']') return true;
		if (str[i] == ' ' || str[i] == ';') return false;
		i++;
	}

	return false;

	/*int countOpenedBrackets = 0;
	int countClosedBrackets = 0;

	for (auto item : str)
	{
	if (item == '[') countOpenedBrackets++;
	if (item == ']') countClosedBrackets++;

	if (((item == ' ' || item == ';' ) && countOpenedBrackets==1)|| countOpenedBrackets == 2 || countClosedBrackets == 2) return false;
	else if(countOpenedBrackets - countClosedBrackets == 0) return true;
	}*/
}





