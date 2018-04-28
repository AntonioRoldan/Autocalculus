#include "../include/simplifier.h"
#include "../include/parser.h"
#include <algorithm>

simplifier::simplifier(std::string &expression_argument) {
	_expression = expression_argument;
}

bool simplifier::isNumber(char &number) {
	auto pos = std::find(_numbers.begin(), _numbers.end(), number);
	return pos != _numbers.end();
}

bool simplifier::quotient_exists() {
	auto find_quotient = std::find(polynomial.polynomial.begin(), polynomial.polynomial.end(), "/");
	if (find_quotient != polynomial.polynomial.end()) {
		polynomial.quotient_exists = true;
		return true;
	}
	else {
		return false;
	}
}

bool simplifier::isFunction(std::string &pfunction) {
	auto isFunction = std::find(_func.begin(), _func.end(), pfunction);
	return isFunction != _func.end();
}

bool simplifier::product_exists() {
	auto find_product = std::find(polynomial.polynomial.begin(), polynomial.polynomial.end(), "*");
	if (find_product != polynomial.polynomial.end()) {
		polynomial.product_exists = true;
		return true;
	}
	else {
		return false;
	}
}

void simplifier::set_quotients_and_products(std::vector<std::string> &expressionarr) {
	;
}

void simplifier::set_simple_arithmetic() {
	;
}

std::string simplifier::perform_simplification() {
	std::string simplification;
	std::string simple_arithmetic_simplification;
	std::string quotient_products_simplification;
	if (polynomial.product_and_quotients_exist) {
		//If there are any product or quotient rules, they will be deleted from the map and added to a secondary map for their processing later
		;//First we simplify products
		 //Secondly we simplify quotients Note: we need the algebra class to work properly for this
	}
	else if (polynomial.product_exists) {
		;
	}
	else if (polynomial.quotient_exists) {
		;
	}
	else {
		;
	}
	return simplification;
}

bool simplifier::isSymbol(char &symbol) {
	auto pos = std::find(_symbols.begin(), _symbols.end(), symbol);
	return pos != _symbols.end();
}

std::string simplifier::get_exponent(std::string &monomial) {
	typedef std::tuple<int, int> a_b_int;
	auto found_exponent = monomial.find('^');
	std::string exponent;
	if (found_exponent != std::string::npos) {
		exponent = monomial.substr(found_exponent - 1, monomial.size());
	}
	else if (monomial.back() == 'x') {
		exponent = "x";
	}
	else if (monomial.back() == '+' or monomial.back() == '-' or monomial.back() == '*' or monomial.back() == '/') {
		exponent = "null";
	}
	else {
		exponent = "integer";
	}
	return exponent;
}

void simplifier::collect_exponents(std::deque<std::string> &exponents) { //TODO
}

std::vector<std::tuple<int, int>> simplifier::gather_up_common_exponents(std::string &exponent_pivot) {
	bool isInteger; //TODO: Unused variable
	int count = 0;
	int a_index; // And here
	int b_index; // And here
	int int_a_index;
	int int_b_index;
	std::string exponent;
	std::vector<std::tuple<int, int>> common_exponents;
	typedef std::tuple<std::string, std::string> a_and_b;
	typedef std::tuple<int, int> indices_pair;
	auto last = std::unique(polynomial.parsed_argument.begin(), polynomial.parsed_argument.end());
	polynomial.parsed_argument.erase(last, polynomial.parsed_argument.end());
	for (int index : polynomial.parsed_argument) {
		if (get_exponent(index_to_expression[index]) == exponent_pivot) {
			if (count == 0) {
				a_index = index;
				count += 1;
			}
			else if (count == 1) {
				b_index = index;
				common_exponents.push_back(std::make_tuple(a_index, b_index));
				count += 1;
			}
			else {
				count = 0;
			}
		}
		else
			continue;
	}
	return common_exponents;
}

void simplifier::parse_argument() { //TODO: Test the argument parsing
	std::tuple<std::vector<int>, std::map<int, std::string>, std::vector<std::string>, std::map<int, std::string>> tokens;
	Parser parser = Parser(_expression);
	_expressionarr = parser.detect_functions();
	tokens = parser.parse();
	polynomial.parsed_argument = std::get<0>(tokens); ////////S TEST 1
	index_to_expression = std::get<1>(tokens);
	polynomial.poly = std::get<2>(tokens); //////S TEST 2 ///print elements one by one
	_index_to_functionm = std::get<3>(tokens);
}

std::string simplifier::perform_polynomial_simplification_helper(bool simple_arithmetic) {
	return "";//TODO
}

std::string simplifier::perform_mixed_simplification_helper(bool simple_arithmetic) {
	return "";//TODO
}

std::string simplifier::simplification() {
	std::string simplification;
	if (quotient_exists() and product_exists())
		polynomial.quotient_and_product_exist = true;
	simplification = perform_simplification();
	return simplification;
}

std::string simplifier::simplify_expression() {
	parse_argument();
	return simplification();
}