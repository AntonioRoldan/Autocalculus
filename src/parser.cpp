#include "../include/parser.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Parser::Parser(std::string &expression) {
	_expression = expression;
}

bool Parser::isNumber(char &number) {
	auto pos = std::find(_numbers.begin(), _numbers.end(), number);
	return pos != _numbers.end();
}

bool Parser::isFunction(std::string &pfunction) {
	auto isFunction = std::find(_func.begin(), _func.end(), pfunction);
	return isFunction != _func.end();
}

bool Parser::isSymbol(char &symbol) {
	auto pos = std::find(_symbols.begin(), _symbols.end(), symbol);
	return pos != _symbols.end();
}

void Parser::insert_index_to_function() {
	for (size_t i = 0; i < _expressionarr.size(); i++) {
		auto isfunction = std::find(_func.begin(), _func.end(), _expressionarr[i]);
		if (isfunction != _func.end()) {
			_index_to_functionp.first = i;
			_index_to_functionp.second = _expressionarr[i];
			if (!_repeated_values)
				_index_to_functionm.insert(_index_to_functionp);
			else
				_index_to_functionmr.insert(_index_to_functionp);
		}
		else
			continue;
	}
}

void Parser::insert_index_to_expression(int &index, std::string &expression) {
	_index_to_expressionp.first = index;
	_index_to_expressionp.second = expression;
	_index_to_expression.insert(_index_to_expressionp);
}


std::tuple<std::vector<int>, std::map<int, std::string>, std::vector<std::string>, std::map<int, std::string>> Parser::parse() {
	std::tuple<std::vector<int>, std::map<int, std::string>, std::vector<std::string>, std::map<int, std::string>> tokens;
	std::string function;
	std::string monomial;
	bool catch_argument = false;
	std::string symbol;
	int index = 0;
	_expressionarr = detect_functions();
	_use_multimap = true;
	insert_index_to_function();
	for (size_t i = 0; i < _expressionarr.size(); i++) {
		//We prepare a map, mapping indices to expressions
		if (_expressionarr[i] == "(" || _expressionarr[i] == ")") {
			catch_argument = _expressionarr[i] == "(";
		}
		else if (isFunction(_expressionarr[i])) {
			function = _expressionarr[i] + "(" + ")"; ////////Instead we add the sharp to it
			std::cout << "Function: " << function << std::endl;
		}
		else if (!isSymbol(_expressionarr[i][0]) && !isFunction(_expressionarr[i]) && catch_argument) {///////We don't need to catch arguments since they will be stored in the # map
			std::string monomial_inside = _expressionarr[i];
			std::cout << "Monomial inside: " << monomial_inside << std::endl;
			auto starting_brackets = function.find(")");
			function.insert(starting_brackets, monomial_inside); //We insert the argument
			insert_index_to_expression(index, function);
			index += 1;
		}
		else if (!isSymbol(_expressionarr[i][0]) && !isFunction(_expressionarr[i]) && !catch_argument) {
			monomial = _expressionarr[i];
			std::cout << "Monomial: " << monomial << std::endl;
			insert_index_to_expression(index, monomial);
			index += 1;
		}
		else if (isSymbol(_expressionarr[i][0])) {
			symbol = _expressionarr[i];
			std::cout << "Symbol: " << symbol << std::endl;
			insert_index_to_expression(index, symbol);
			index += 1;
		}
	}
	typedef std::map<int, std::string>::const_iterator it;
	it iterator;
	for (iterator = _index_to_expression.begin(); iterator != _index_to_expression.end(); iterator++) {
		parsed_argument.push_back(iterator->first);
		poly.push_back(iterator->second);
	}
	tokens = std::make_tuple(parsed_argument, _index_to_expression, poly, _index_to_functionm);
	return tokens;
}

std::vector<std::string> Parser::detect_functions() { //Refactoring needed
	std::vector<std::string> parsed_expression;
	int log = 0, ln = 0, sin = 0, cos = 0, tan = 0, sec = 0, cosec = 0, cotan = 0, arcsin = 0, arcos = 0, arctan = 0, e = 0;
	std::string exponent;
	bool catch_exponent = false;
	bool polynomial_raised = false;
	bool integer_raised = false;
	bool done = false;
	bool previous_number;
	std::string number;
	std::string function;
	while (!done) {
		for (size_t i = 0; i < _expression.size(); i++) { //TODO: This was originally <=, change back if needed but I doubt it

			switch (_expression[i]) {

			case 'l':
				if (_expression[i + 1] == 'n') {
					function = "ln";
					_expressionarr.push_back(function);
					ln++;
				}
				else if (_expression[i + 1] == 'o') {
					function = "log";
					_expressionarr.push_back(function);
					log++;
				}
				break;

			case 's':
				if (_expression[i + 1] == 'i') {
					if (_expression[i - 3] == 'a');
					else {
						function = "sin";
						_expressionarr.push_back(function);
						sin++;
					}
				}
				else if (_expression[i - 2] == 'c') {
					if (_expression[i + 1] == 'e') {
						;
					}
					else if (_expression[i - 1] == 'c') {
						function = "arcsin";
						_expressionarr.push_back(function);
						arcsin++;
					}
					else {
						function = "cos";
						_expressionarr.push_back(function);
						cos++;
					}
				}
				else if (_expression[i + 1] == 'e') {
					function = "sec";
					_expressionarr.push_back(function);
					sec++;
				}
				break;

			case 'c':
				if (_expression[i + 1] == 'o') {
					if (_expression[i - 2] == 'a');
					else if (_expression[i + 3] == 'e') {
						function = "cosec";
						_expressionarr.push_back(function);
						cosec++;
					}
					else if (_expression[i + 2] == 's') {
						function = "cos";
						_expressionarr.push_back(function);
						cos++;
					}
				}
				else if (_expression[i + 1] == 's') //If it is arcsin we will cause a redundance
					;
				else if (_expression[i - 1] == 'e');
				else;
				break;

			case 'a':
				switch (_expression[i + 3]) {

				case 'o':
					function = "arcos";
					_expressionarr.push_back(function);
					arcos++;
					break;

				case 's':
					function = "arcsin";
					_expressionarr.push_back(function);
					arcsin++;
					break;


				case 't':
					function = "arctan";
					_expressionarr.push_back(function);
					arctan++;
					break;
				}
				break;

			case 't':
				if (_expression[i - 3] == 'a');
				else if (_expression[i - 2] == 'c') {
					function = "cotan";
					_expressionarr.push_back(function);
					cotan++;
					break;
				}
				else {
					function = "tan";
					_expressionarr.push_back(function);
					tan++;
					break;
				}
				break;

			case 'e':
				if (_expression[i - 1] == 's');
				else if (_expression[i - 3] == 'c');
				else {
					function = "e";
					_expressionarr.push_back(function);
					e++;
					break;
				}
				break;
			default:
				if (isspace(_expression[i]));
				else if (isNumber(_expression[i]) && !previous_number) {
					if (!catch_exponent) {
						number += _expression[i];
					}
					else {
						exponent += _expression[i];
					}
					previous_number = true;
				}
				else if (isNumber(_expression[i]) && previous_number) {
					if (!catch_exponent)
						number += _expression[i];
					else
						exponent += _expression[i];
				}
				else if (!isNumber(_expression[i]) && previous_number) {
					if (_expression[i] == '^') {
						if (_expression[i - 1] == 'x' || _expression[i - 1] == 'y') {//If it is a polynomial raised to some power
							number += _expression[i]; //Number will be added its exponent symbol
							catch_exponent = true;
							polynomial_raised = true;
						}
						else {
							catch_exponent = true;
							integer_raised = true;
						}
					}
					else if (catch_exponent && integer_raised) {
						number = std::to_string(std::pow(std::stoi(number), std::stoi(exponent)));
						_expressionarr.push_back(number);
						exponent.clear();
						integer_raised = false;
					}
					else if (catch_exponent && polynomial_raised) {
						number += exponent; //Number will be added its exponent so it will later be processed by differentiate_monomial
						_expressionarr.push_back(number);
						exponent.clear();
						polynomial_raised = false;
					}
					else {
						previous_number = false;
						_expressionarr.push_back(number);
						number.clear();
					}
				}
				if (isSymbol(_expression[i])) { //TODO: Get the program to process exponents to expressions between brackets as functions
					if (_expression[i] == '^' && _expression[i - 1] == ')') //If we have an exponent to an expression between brackets, we can interpret it as a function
						; //with its respective derivative
					else
						_expressionarr.push_back(std::string(1, _expression[i])); //We convert char into a string of length one
				}
			}
		}
		if (_use_multimap)
			done = true;
		else {
			_use_multimap = true; //We perform another set of iterations
			_expressionarr.clear();
		}
	}
	for (size_t i = 0; i < _expressionarr.size(); i++) {
		std::cout << _expressionarr[i] << std::endl;
	}
	parsed_expression = _expressionarr;
	return parsed_expression;
}