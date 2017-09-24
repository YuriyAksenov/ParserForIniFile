// ParserForIniFile.cpp: определяет точку входа для консольного приложения.
//

/*
[section1]
variable1=X
Var2=Y
[section2]

[server]
host=27.0.0.1
post=9090
provolo=UDP

Хотим иметь 
1. Список секций
2. generic<-get(sec,key,defaul_tvalue)

*/

#include "stdafx.h"
#include "IniParser.h"
#include <iostream>
using namespace std;

int main()
{
	IniParser ini = IniParser("E:\\Kronshtadt\\C++\\lab01_input_correct_09.ini");

	cout << ini.toString();
    return 0;
}


/*

public:
template<type>
auto getField(string sec, string, type_default)

vector<string> Sections()
vector<string> PrintFields(<string> section)


СТРУКТУРЫ ДАННЫХ в си++

*/