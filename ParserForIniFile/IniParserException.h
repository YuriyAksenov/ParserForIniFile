#ifndef INI_PARSER_EXCEPTION_H
#define INI_PARSER_EXCEPTION_H
#include<string>

class ValidException {
public:
	ValidException::ValidException(const std::string, const std::string);
	std::string ValidException::getObject();
	std::string ValidException::getMessage();
	std::string virtual ValidException::toString();
};

class ValidSectionException : public ValidException {
public:
	ValidSectionException::ValidSectionException(const std::string, const std::string);
	std::string ValidSectionException::toString();
};

class ValidFieldException : public ValidException {
public:
	ValidFieldException::ValidFieldException(const std::string, const std::string);
	std::string ValidFieldException::toString();
};

class ValidFileException : public ValidException {
public:
	ValidFileException::ValidFileException(const std::string, const std::string);
	std::string ValidFileException::toString();
};

#endif //INI_PARSER_EXCEPTION_H