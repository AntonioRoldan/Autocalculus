#pragma once
#include <string>
#include <vector>
#include <map>
#include <deque>

class simplifier {
private:

	std::string expression;
	std::vector<std::string> _expressionarr;
	std::string _expression;
	std::vector<int> _numbers = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	std::vector<std::string> _func = { "sin", "cos", "tan", "log", "ln", "sec", "cosec", "cotan", "arcos", "arcsin", "arctan" };
	std::vector<char> _symbols = { '+', '-', '/', '*', '(', ')' };
	bool isPolynomial;
	bool _repeated_values;
	bool _use_multimap;
	std::pair<int, std::string> _index_to_functionp;
	std::map<int, std::string> _index_to_functionm;
	std::multimap<int, std::string> _index_to_functionmr;
	std::pair<int, std::string> index_to_expressionp;
	std::map<int, std::string> index_to_expression;
	struct Argument {
		bool function_on_function;
		bool monomial_on_function;
		bool function_on_monomial;
		bool monomial_on_monomial;
		bool double_subtraction;
		std::string expression_a;
		std::string expression_b;
	};
	Argument argument;
	struct Polynomial {
		std::string expression_a;
		std::string expression_b;
		std::vector<std::string> polynomial;
		std::vector<int> parsed_arguments;
		bool product_exists;
		bool quotient_exists;
		bool quotient_and_product_exist = false;
		bool product_and_quotients_exist;
		std::vector<int> add_sub_indices;
		std::vector<int> parsed_argument;
		std::vector<std::string> poly;
		std::vector<std::tuple<int, int>> indices_tuples;
		std::vector<std::tuple<std::string, std::string>> simple_arithmetic_operations;
		std::vector<std::tuple<std::string, std::string>> quotients_and_products;
		std::vector<std::tuple<int, int>> integers;
	};
	Polynomial polynomial;

public:

	std::string simplify_expression();
	bool hasExponent(const std::string & str);
	bool isSymbol(char &symbol);
	bool isNumber(char &number);
	bool isFunction(std::string &pfunction);
	void parse_argument();
	void collect_exponents(std::deque<std::string> &exponents);
	std::string simplification();
	std::string perform_simplification();
	std::string perform_polynomial_simplification_helper(bool simple_arithmetic);
	std::string perform_mixed_simplification_helper(bool simple_arithmetic);
	std::string get_exponent(std::string &monomial);
	std::vector<std::tuple<int, int>> gather_up_common_exponents(std::string &exponent_pivot);
	void set_quotients_and_products(std::vector<std::string> &expressionarr);
	void set_simple_arithmetic();
	bool quotient_exists();
	bool product_exists();
	simplifier(std::string &expression_argument);
};
