#include "stdafx.h"
#include "IniParserException.h"
#include <string> // подключаем строки

using std::string;


string object;
string message;

ValidException::ValidException(const string _object, const string _message) {
	object = _object;
	message = _message;
}
string ValidException::getObject() {
	return object;
}
string ValidException::getMessage() {
	return message;
}
string ValidException::toString()
{
	return "It is invalid." + getMessage() + "\nObject:" + getObject();
}

ValidSectionException::ValidSectionException(const string object, const string message) : ValidException(object, message) {}
string ValidSectionException::toString() {
	return "Section is invalid." + getMessage() + "\nObject:" + getObject();
}



ValidFieldException::ValidFieldException(const string object, const string message) : ValidException(object, message) {}
string ValidFieldException::toString() {
	return "Field is invalid." + getMessage() + "\nObject:" + getObject();
}


ValidFileException::ValidFileException(const string object, const string message) : ValidException(object, message) {}
string ValidFileException::toString() {
	return "File is invalid." + getMessage() + "\nObject:" + getObject();
}
