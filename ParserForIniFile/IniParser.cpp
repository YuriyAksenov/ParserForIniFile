#include "stdafx.h"
#include "IniParser.h"

#include <iostream>
#include <string> // подключаем строки
#include <fstream> // подключаем файлы
#include<map>

using std::cout;

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::ifstream;


struct Field {
	string key;
	string value;

	Field(string key = "", string value = "") {
		this->key = key;
		this->value = value;
	}

	bool isEmpty() {
		return key.empty();
	}

	string toString() {
		return !isEmpty() ? key + " = " + value : "";
	}
};

enum FieldKeyType
{
	none = -1,
	comment_or_space = 0,
	section = 1,
	field = 2
};

map<string, vector<Field>> sections;

FieldKeyType defineString(const string);
Field parseField(const string);
string parseSection(const string);
bool isCommnetOrSpace(const string);
bool isSection(const string);
bool isField(const string);
bool isValid(const string);
string trim(const string);



IniParser::IniParser(const  string fileName)
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
		for (Field field : section.second)
		{
			if (!field.isEmpty())str = str + "\t" + field.toString()+ "\n";
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

FieldKeyType defineString(const string str) {
	if (isCommnetOrSpace(str)) return FieldKeyType::comment_or_space;
	if (isSection(str)) return FieldKeyType::section;
	if (isField(str)) return FieldKeyType::field;
	return FieldKeyType::none;
}

string parseSection(const string str) {
	if (str[0] == '[' && str[1] == ']') throw _exception(); //NOTVALIDSECTIONEXCEPTION NULL LENGTH OF NAME SECTION
	int iteratorForClosedBracket = 0;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == ' ' || str[i] == ';') throw _exception(); //NOTVALIDSECTIONEXCEPTION SOME SPACES OR SPECIAL SYMBOLS LIKE ; 
		if (str[i] == ']') {
			iteratorForClosedBracket = i;
			break;
		}
	}
	return str.substr(1, iteratorForClosedBracket - 1);
}

Field parseField(const string str) {


	int equalPos = (str.find('=') != string::npos ? str.find('=') : throw _exception()); // NOT FIELD

	int commentPos = str.find(';') != string::npos ? str.find(';') : str.length() + 1;

	if (commentPos < equalPos)  throw _exception(); // Comment is places before equating
	
	string keyBuf = trim(str.substr(0, equalPos-1));

	if (keyBuf.empty() || !isValid(keyBuf)) throw _exception(); // данные не валидные есть пробел или пустой

	int iteratorForLastValuePosition = str.length() - 1;
	for (int i = equalPos + 1; i < str.length(); i++)
	{
		if (str[i] == ';') {
			iteratorForLastValuePosition = i - 1;
			break;
		}
	}

	 string valueBuf = trim(str.substr(equalPos + 1, iteratorForLastValuePosition - equalPos));

	if (!valueBuf.empty() && !isValid(valueBuf)) throw _exception(); // данные не валидные есть пробел или пустой

	Field field = Field(keyBuf, valueBuf);
	return field;
}

string trim(const string str) {

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


bool isCommnetOrSpace(const string str) {
	return str[0] == ' ' || str[0] == ';';
}

bool isField(const string str) {
	return str.find('=') != string::npos;
}

bool isSection(const string str) {
	return str[0] == '[';
}

bool isValid(const string str) {
	for (auto var : str)
	{
		if (var == ' ' || var == ';') return false;
	}
	return true;
}


//#include "stdafx.h"
//#include "IniParser.h"
//
//#include <iostream>
//#include <string> // подключаем строки
//#include <fstream> // подключаем файлы
//#include<map>
//
//using std::string;
//using std::vector;
//using std::map;
//using std::pair;
//using std::ifstream;
//
//
//struct Field {
//string key;
//string value;
//
//Field(string key = "", string value = "") {
//this->key = key;
//this->value = value;
//}
//
//bool isEmpty() {
//return key.empty();
//}
//};
//
//enum FieldKeyType
//{
//none = -1,
//comment_or_space = 0,
//section = 1,
//field = 2
//};
//
//map<string, vector<Field>> sections;
//ifstream file;
//
//FieldKeyType defineString(const string);
//Field parseField(const string);
//string parseSection(const string);
//bool isCommnetOrSpace(const string);
//bool isSection(const string);
//bool isField(const string);
//bool isValid(const string);
//string trim(const string);
//
//
//
//IniParser::IniParser(const  string fileName)
//{
//
//ifstream file(fileName);
//string str;
//
//string section = "";
//vector<Field> fields = vector<Field>();
//
//while (getline(file, str))
//{
//switch (defineString(str)) {
//case(FieldKeyType::section): {
//string newSection = parseSection(str);
//if (section != newSection) {
//if (!section.empty()) {
//sections.insert(pair<string, vector<Field>>(section,fields));
//}
//section = newSection;
//}
//break;
//}
//case(FieldKeyType::field): {
//Field field = parseField(str);
//fields.push_back(field);
//break;
//}
//default: {
//break;
//}
//}
//}
//
//
//file.close();
//}
//
//template<typename T>
//T IniParser::getValue(const string section, const  string field, const T type_default) {
//for (auto item : sections.find(section)->second)
//{
//if (item.Key.compare(field) == 0) return item.Value;
//}
//return type_default;
//}
//
//vector<string> IniParser::getSections() {
//vector<string> sectionsReturn = vector<string>();
//for (auto item : sections)
//{
//sectionsReturn.push_back(item.first);
//}
//return sectionsReturn;
//}
//
//vector<string> IniParser::getFields(const string section) {
//vector<string> fieldsReturn = vector<string>();
//for each (auto item in sections.find(section)->second)
//{
//fieldsReturn.push_back(item.key);
//}
//return fieldsReturn;
//}
//
//bool IniParser::isContainingSection(const string section) {
//return sections.find(section) != sections.end();
//}
//
//bool IniParser::isContainingFieldInSection(const string section, const string field) {
//for (auto item : sections.find(section)->second)
//{
//if (item.key.compare(field) == 0) return true;
//}
//return false;
//}
//
//string IniParser::toString() {
//string str = "";
//for each (auto section in sections)
//{
//str = str + "[" + section.first + "]" + "\n";
//for (Field field : section.second)
//{
//if (!field.isEmpty())str = str + "\t" + field.key + " = " + field.value + "\n";
//}
//}
//return str;
//}
//
//IniParser::~IniParser()
//{
//for (auto section : sections)
//{
//section.second.clear();
//}
//sections.clear();
//file.close();
//}
//
//FieldKeyType defineString(const string str) {
//if (isCommnetOrSpace(str)) return FieldKeyType::comment_or_space;
//if (isSection(str)) return FieldKeyType::section;
//if (isField(str)) return FieldKeyType::field;
//return FieldKeyType::none;
//}
//
//Field parseField(const string str) {
//
//int equalPos = (str.find('=') != string::npos ? str.find('=') : throw _exception()); // NOT FIELD
//int commentPos = str.find(';') != string::npos ? str.find(';') : -1;
//
//if (commentPos < equalPos)  throw _exception(); // Comment is places before equating
//
//string keyBuf = trim(str.substr(0, equalPos - 1));
//
//if (keyBuf.empty() || !isValid(keyBuf)) throw _exception(); // данные не валидные есть пробел или пустой
//
//string valueBuf;
//
//int iteratorForLastValuePosition = str.length();
//for (int i = equalPos + 1; i < str.length(); i++)
//{
//if (str[i] == ';') {
//iteratorForLastValuePosition = i - 1;
//break;
//}
//}
//
//valueBuf = trim(str.substr(equalPos + 1, iteratorForLastValuePosition - equalPos));
//if (!valueBuf.empty() || !isValid(valueBuf)) throw _exception(); // данные не валидные есть пробел или пустой
//
//Field field = Field(keyBuf, valueBuf);
//return field;
//}
//
//string trim(const string str) {
//
//int i = 0;
//while (str[i] == ' ' || i != str.size())
//{
//i++;
//}
//int j = str.size() - 1;
//while (str[j] == ' ' || i != 0)
//{
//j--;
//}
//
//if (j <= i) return "";
//
//string forReturned((str.size() - (str.size() - j + i)), ' ');
//
//for (int k = 0; k < forReturned.size(); k++)
//{
//forReturned[k] = str[i + k];
//}
//
//return forReturned;
//
//}
//
//string parseSection(const string str) {
//if (str[0] == '[' && str[1] == ']') throw _exception(); //NOTVALIDSECTIONEXCEPTION NULL LENGTH OF NAME SECTION
//
////int iteratorForOpenedBracket = 0;
//int iteratorForClosedBracket = 0;
//
//for (int i = 0; i < str.size(); i++)
//{
//if (str[i] == ' ' || str[i] == ';') throw _exception(); //NOTVALIDSECTIONEXCEPTION SOME SPACES OR SPECIAL SYMBOLS LIKE ;
//if (str[i] == ']') {
//iteratorForClosedBracket = i;
//break;
//}
//}
//return str.substr(1, iteratorForClosedBracket - 1);
//}
//
//bool isCommnetOrSpace(const string str) {
//return str[0] == ' ' || str[0] == ';';
///*for (auto item : str)
//{
//if (item != ' ' || item != ';') return false;
//else if (item == ';') return true;
//
//}
//return true;*/
//}
//
//bool isField(const string str) {
//	return str.find('=') != string::npos;
//	/*for(auto item : str)
//	{
//	if (item = '=') return true;
//	}
//	return false;*/
//}
//
//bool isSection(const string str) {
//
//	return str[0] == '[';
//
//	/*if (str[0] != '[' || str[0] == '[' && str[1] == ']') return false;
//
//	int i = 1;
//	while (i != str.size())
//	{
//	if (str[i] == ']') return true;
//	if (str[i] == ' ' || str[i] == ';') return false;
//	i++;
//	}
//
//	return false;*/
//}
//
//bool isValid(const string str) {
//	for (auto var : str)
//	{
//		if (var == ' ' || ';') return false;
//	}
//	return true;
//}
//
//
//
//
//
//




