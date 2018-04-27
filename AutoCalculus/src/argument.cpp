#include <map>
#include <vector>

#include "../include/argument.h"
#include "../include/parser.h"
#include "../include/simplifier.h"

void Argument::parse_argument() {
	std::tuple<std::vector<int>, std::map<int, std::string>, std::vector<std::string>, std::map<int, std::string>> tokens;
	Parser parse = Parser(_argument);
	tokens = parse.parse();
}

//3x + 2x - [(4sin(3x) * 7x) * (5 * 8x^2) * (3x * 2sin(5x)) * (4x * 5x) * (4x * 5) * (7x * 9)
// 6x - [(4cos(3x) * 7 + 7x * 4sin(3x)) * (40x^2) * (6sin(5x) + 3x * 2cos(5x)) * (20x^2) * (20x) * (63x)] //Also having, n product rules, the resulting expression is going to be splitted into n subexpressions with additions
//If both are functions //Given a number of possible operations n, there will always be a certain number of possible product rules, and therefore possible expansions
//3x + 2x - [(4sin(3x) * 7x) * (3x * 2sin(5x)) * (4x * 5x) * (4x * 5) * (7x * 9)]

std::string Argument::perform_polynomial_differentiation_helper() {
	return "";//TODO
}

std::string Argument::perform_differentiation() {
	std::string derivative;
	std::string simplification;
	simplifier simplify = simplifier(_argument);
	return derivative;
}

std::string Argument::differentiate() {
	std::string derivative;
	parse_argument(); //It stores an index with its corresponding expression in a map
	derivative = perform_differentiation();
	return derivative;
}

std::string Argument::differentiate_monomial(std::string &monomial) {//////////THIS FUNCTION WILL BELONG TO THE DERIVATIVE CLASS
	std::string exponent;
	std::string derivative;
	std::string coefficient;
	if (monomial == "x") { //We handle the case where we have x
		derivative = "1";
		return derivative;
	}
	else {
		auto partition = monomial.find('^');
		if (partition != std::string::npos) {
			exponent = monomial.substr(partition, monomial.size());
			monomial = monomial.substr(0, partition - 2);
		}
		else if (monomial.back() == 'x') {
			derivative = monomial.substr(0, monomial.size() - 1);
		}
	}
	if (exponent.front() == '^') //Handles the case where we have Nx^N
		if (monomial != "x") {
			if (exponent.front() == '2' and exponent.size() == 2) { //Nx^2
				derivative = std::to_string(std::stoi(exponent.substr(1, exponent.size())) * std::stoi(
					monomial.substr(0, monomial.size() - 1))); //We subtract one to get rid of x
				derivative += 'x';
			}
			else { //Nx^N
				derivative = std::to_string(std::stoi(exponent.substr(1, exponent.size())) * std::stoi(monomial.substr(0, monomial.size() - 1))); //We subtract one to get rid of x
				exponent.erase(exponent.begin());
				derivative += "x^" + std::to_string(std::stoi(exponent) - 1);
			}
		}
		else { //Else we have x^N or x^2
			if (exponent.front() == '2' and exponent.size() == 2) {
				derivative = "2x";
			}
			else {
				coefficient = exponent.substr(1, exponent.size());
				exponent = std::to_string(std::stoi(exponent.substr(1, exponent.size())) - 1);
				derivative = derivative + '^' + exponent;
			}
		}
	else { //We just have a coefficient N
		derivative = "0";
	}
	return derivative;
}

Argument::Argument(std::string &argument, std::deque<std::string> &derivatives_inside) {
	_isPolynomial = derivatives_inside.empty() ? true : false;
	_argument = argument;
}
