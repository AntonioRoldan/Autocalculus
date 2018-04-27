#pragma once
#include <string>
#include <vector>
#include <map>
#include <deque>

class expression {
private:
	std::string _expression;
	std::deque<std::tuple<int, int>> _function_ranges;
	std::vector<std::string> _expressionarr;
	std::vector<char> _symbols_stored;
	bool _use_multimap = false;
	std::vector<int> _numbers = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'x', 'y', 'z', '^' };
	std::vector<int> _symbols = { '+', '-', '*', '/', '(', ')' };
	std::vector<std::string> _func = { "log", "ln", "sin", "cos", "tan", "sec", "cosec", "cotan", "e", "arcsin", "arcos", "arctan" };
	std::pair<std::string, std::vector<int>> _function_to_rangep; //Stores function given as a string and a vector containing the chain rule iterations and the indices for the argument
	std::map<std::string, std::vector<int>> _function_to_rangem;
	std::multimap<std::string, std::vector<int>> _function_to_rangemr; //If there are repeated values
	std::pair<std::size_t, bool> _index_to_bracketsp; //Stores position of brackets and a boolean (false for '(' true for ')')
	std::map<std::size_t, bool> _index_to_bracketsm;
	std::pair<char, std::vector<int>> _symbols_classificationp; //Keys are the symbols, they are mapped to their indices
	std::map<char, std::vector<int>> _symbols_classificationm;
	std::pair<int, char> _index_to_symbolsp;
	std::map<int, char> _index_to_symbolsm;
	std::pair<int, std::string> _index_to_functionp; //Stores each function as a key with its corresponding index in the expression
	std::map<int, std::string> _index_to_functionm;
	std::multimap<int, std::string> _index_to_functionmr; //Same for repeated values
	std::pair<std::tuple<int, int>, std::vector<std::tuple<int, int>>> _function_to_inside_functionp; //Keys: function's range Values: ranges of the functions inside it
	std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>> _function_to_inside_functionm;
	std::pair<std::tuple<int, int>, std::tuple<int, int>> _SE_to_ODp;
	std::map<std::tuple<int, int>, std::tuple<int, int>> _SE_to_OD;
	std::vector<std::tuple<int, int>> _sorted_SB_EB;
	bool _repeated_values = false;

public:
	bool isNumber(char &number);
	bool isSymbol(char &symbol);
	void subvector_to_string(std::tuple<int, int> &indices, std::string &expression); //TODO: Test this mess
	bool initialise_classification(char symbol, size_t &index);
	void set_index_to_symbols(char &symbol, size_t &index);
	void insert_index_to_symbols();
	void insert_index_to_function();
	void insert_SE_OD(std::tuple<int, int> &SE, std::tuple<int, int> OD);
	void insert_symbol(char &symbol, size_t &index);
	void fill_function_ranges();
	std::string give_function(int &index);
	std::vector<int> brackets_bag();
	void find_scopes();
	void find_functions_inside();
	void fill_function_to_inside(std::tuple<int, int> &pivot);
	std::string differentiate();
	void sort_tuples_vector(std::vector<std::tuple<int, int>> &tuples_vector);
	void function_parser();
	void test1();
	void test2();
	void test3();
	void sort_tuples_queue(std::deque<std::tuple<std::tuple<int, int>, std::tuple<int, int>>> &tuples_queue);
	void differentiate_level_by_level(std::map<std::tuple<int, int>, bool> &differentiated_functions, std::map<std::tuple<int, int>, std::tuple<std::string, std::string>> &scope_to_expression_to_derivative, std::deque<std::string> &polynomial_derivatives);
	expression(std::string expression); //Copy constructor
	expression(expression&&) = default; //Move constructor
	expression& operator = (const expression&) & = default; //Move constructor
	expression& operator = (expression&&) & = default; //Copy assignment operator
	~expression(); //Destructor
};