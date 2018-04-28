#pragma once
#include <string>
#include <vector>
#include <deque>

class Argument { //It is from here that we perform simplification and differentiation operations
private:
	bool _isPolynomial;
	std::string _argument;
	struct argument {
		bool product_and_quotients_exist = false;
		bool product_exists = false;
		bool quotient_exists = false;
		std::vector<std::string> arguments;//TODO: Was renamed to arguments to avoid conflict with struct type
	};
	argument argument;

public:

	std::string differentiate();
	std::string differentiate_monomial(std::string &monomial);
	void parse_argument();
	bool product_exists();
	bool quotient_exists();
	std::string perform_differentiation();
	std::string differentiation();
	std::string perform_polynomial_differentiation_helper();
	Argument(std::string &argument, std::deque<std::string> &derivatives_inside);

};