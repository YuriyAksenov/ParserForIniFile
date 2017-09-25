// IniParserApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>

#include"IniParser.h"

using namespace std;

int main()
{
	try
	{
		IniParser ini = IniParser("E:\\Kronshtadt\\C++\\IniParser\\IniFiles\\correct_09.ini");

		cout << "Sections\n------------------------------\n";

		for each (string section in ini.getSections())
		{
			cout << "Section:" + section << endl;
		}

		cout << "\n\nFields in DEMODS section\n";
		cout << "------------------------------\n";

		for each (string field in ini.getFields("DEMODS"))
		{
			cout << "Field:" + field << endl;
		}
		cout << "------------------------------\n";
		

		cout << "\n\nValue for field with key = StartDataPort is " + ini.getValue("DEMODS", "StartDataPort", "");
		cout << "\n------------------------------\n";
		cout << "\nAdditional interface for checking of containing current section or field\n";

		cout << "Is DEMODS section exists in INI FILE:"<<ini.isContainingSection("DEMODS")<<"\n";
		cout << "Is StartDataPort field exists in INI FILE:"<<ini.isContainingFieldInSection("DEMODS", "StartDataPort")<<"\n";

	}
	catch (ValidException e)
	{
		cout << e.getMessage();
	}

	return 0;
}
