#include "../include/algebra.h"

bool algebra::hasExponent(const std::string & str){
	const char c = '^';
	return str.find(c) != std::string::npos;
}

//addition or subtraction,
std::string algebra::mulMonomials(std::string &a, std::string &b) {
	char exponent = '^';
	int pos;
	int posb; //Only used to store the position where character ^ is found in the b string in case both monomials are raised to some power
	int a_integer;
	int b_integer;
	int a_exponentint;
	int b_exponentint;
	std::string a_exponent;
	std::string b_exponent;
	int result_exponent;
	int result_integer;
	std::string result;
	if (a == "null" or b == "null")
		return "null";
	if (hasExponent(a) and hasExponent(b)) { //3x^2 * 4x^2 will be equal to 12x^4
		pos = (int)a.find(exponent);
		posb = (int)b.find(exponent);
		a_integer = std::stoi(a.substr(0, (unsigned long)pos - 2));
		b_integer = std::stoi(b.substr(0, (unsigned long)posb - 2));
		a_exponentint = std::stoi(a.substr((unsigned long)(pos + 1), a.size()));
		b_exponentint = std::stoi(b.substr((unsigned long)(posb + 1), b.size()));
		result_exponent = a_exponentint + b_exponentint;
		result_integer = a_integer * b_integer;
		result = std::to_string(result_integer) + 'x' + '^' + std::to_string(result_exponent);
	}
	else if (hasExponent(a) and !hasExponent(b)) { //3x^2 and 4x or 4
		pos = (int)a.find(exponent);
		a_integer = std::stoi(a.substr(0, (unsigned long)pos - 2)); //We store 3 here
		a_exponent.assign(a.begin() + pos - 1, a.begin() + pos); //pos - 1 to ensure we get the variable as well it will store x^ here
		a_exponentint = std::stoi(a.substr(pos + 1, a.size())); //We store the 2 for the exponent here
		if (isNumber(b.back())) { // 3x^2 * 4
			b_integer = std::stoi(b);
			result = std::to_string(a_integer * b_integer) + a_exponent + std::to_string(a_exponentint); //is 12x^2
		}
		else { // 3x^2 * 4x
			b_integer = std::stoi(b.substr(0, b.size() - 1));
			a_exponentint += 1;
			a_exponent += std::to_string(a_exponentint);
			result = std::to_string(a_integer * b_integer) + a_exponent;
		}
	}
	else if (!hasExponent(a) and hasExponent(b)) {
		pos = (int)b.find(exponent);
		b_integer = std::stoi(b.substr(0, (unsigned long)pos - 1));
		b_exponent.assign(b.begin() + pos - 1, b.begin() + pos);
		b_exponentint = std::stoi(b.substr((unsigned long)(pos + 1), b.size()));
		if (isNumber(a.back())) { //4 * 3x^2
			a_integer = std::stoi(a);
			result = std::to_string(a_integer * b_integer) + b_exponent + std::to_string(b_exponentint);
		}
		else { //4x * 3x^2
			a_integer = std::stoi(a.substr(0, a.size() - 1));
			b_exponentint += 1;
			a_exponent += std::to_string(b_exponentint);
			result = std::to_string(a_integer * b_integer) + b_exponent;
		}
	}
	else if (isNumber(a.back()) and b.back() == 'x') { //4 * 3x
		a_integer = std::stoi(a);
		b_integer = std::stoi(b.substr(0, b.size() - 1));
		result = std::to_string(a_integer * b_integer) + b.back();
	}
	else if (a.back() == 'x' and b.back() == 'x') { //3x * 4x
		a_integer = std::stoi(a.substr(0, a.size() - 1));
		b_integer = std::stoi(b.substr(0, b.size() - 1));
		result = std::to_string(a_integer * b_integer) + a.back(); //a.front() is 'x'
	}
	else if (a.back() == 'x' and isNumber(b.back())) { //3x * 4
		a_integer = std::stoi(a.substr(0, a.size() - 1));
		b_integer = std::stoi(b);
		result = std::to_string(a_integer * b_integer) + a.back();
	}
	else {
		a_integer = std::stoi(a);
		b_integer = std::stoi(b);
		result = std::to_string(a_integer * b_integer);
	}
	return result;
}

bool algebra::isNumber(char &number) {
	auto pos = std::find(_numbers.begin(), _numbers.end(), number);
	return pos != _numbers.end();
}

algebra::algebra(bool &double_subtraction) {
	//binomial.double_subtraction = double_subtraction; //TODO: Binomial is undefined
}
