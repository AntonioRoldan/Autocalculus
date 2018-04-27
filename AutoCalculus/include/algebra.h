#pragma once
#include <string>
#include <vector>

class algebra {

private:

	std::vector<char> _numbers = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

public:

	std::string mulMonomials(std::string &a, std::string &b);
	std::string divMonomials(std::string &a, std::string &b);
	bool isNumber(char &number);
	bool hasExponent(const std::string & str);
	algebra(bool &double_subtraction);
};