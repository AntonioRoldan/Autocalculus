#pragma once
#include <string>
#include <vector>
#include <map>

class Parser {
private:
	std::string _expression;
	std::vector<std::string>_expressionarr;
	bool _use_multimap;
	bool _repeated_values;
	std::vector<int> _numbers = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'x', 'y', 'z', '^' };
	std::vector<int> _symbols = { '+', '-', '*', '/', '(', ')' };
	std::vector<std::string> _func = { "log", "ln", "sin", "cos", "tan", "sec", "cosec", "cotan", "e", "arcsin", "arcos", "arctan" };
	std::pair<int, std::string> _index_to_functionp;
	std::map<int, std::string> _index_to_functionm;
	std::pair<int, std::string> _index_to_expressionp;
	std::map<int, std::string> _index_to_expression;
	std::multimap<int, std::string> _index_to_functionmr;
	std::vector<int> parsed_argument;
	std::vector<std::string> poly;
public:
	std::vector<std::string> detect_functions();
	bool isNumber(char &number);
	bool isSymbol(char &symbol);
	bool isFunction(std::string &pfunction);
	void insert_index_to_function();
	void insert_index_to_expression(int &index, std::string &expression);
	std::tuple<std::vector<int>, std::map<int, std::string>, std::vector<std::string>, std::map<int, std::string>> parse();
	Parser(std::string &expression);
};