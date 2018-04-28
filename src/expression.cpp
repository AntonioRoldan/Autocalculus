#include <algorithm>
#include <iostream>
#include <stack>

#include "../include/expression.h"
#include "../include/parser.h"
#include "../include/argument.h"

bool expression::isNumber(char &number) {
	auto pos = std::find(_numbers.begin(), _numbers.end(), number);
	return pos != _numbers.end(); //TODO: Is this needed? Parser has an identical function
}

bool expression::isSymbol(char &symbol) {
	auto pos = std::find(_symbols.begin(), _symbols.end(), symbol);
	return pos != _symbols.end(); //TODO: Is this needed? Parser has an identical function
}

void expression::insert_index_to_function() {
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

bool expression::initialise_classification(char symbol, size_t &index) {
	std::vector<int> symbols;
	auto is_stored = std::find(_symbols_stored.begin(), _symbols_stored.end(), symbol);
	if (is_stored != _symbols_stored.end())
		return false;
	else if (symbols.empty() || is_stored == _symbols_stored.end()) {
		_symbols_stored.push_back(symbol);
		symbols.push_back(index);
		_symbols_classificationp.first = symbol; //Symbol
		_symbols_classificationp.second = symbols; //Symbols indices
		_symbols_classificationm.insert(_symbols_classificationp);
		return true;
	}
	return false;
}

void expression::set_index_to_symbols(char &symbol, size_t &index) {
	_index_to_symbolsp.first = index;
	_index_to_symbolsp.second = symbol;
	_index_to_symbolsm.insert(_index_to_symbolsp);
}

void expression::insert_index_to_symbols() {
	for (size_t i = 0; i < _expressionarr.size(); i++) {
		auto issymbol = std::find(_symbols.begin(), _symbols.end(), _expressionarr[i][0]);
		if (issymbol != _symbols.end()) {
			insert_symbol(_expressionarr[i][0], i); //We index the string so as tu turn it into a character
		}
	}
}

void expression::insert_symbol(char &symbol, size_t &index) {
	set_index_to_symbols(symbol, index);
	switch (symbol) {

	case '+': if (initialise_classification('+', index)); else _symbols_classificationm['+'].push_back(index); break;

	case '-': if (initialise_classification('-', index)); else _symbols_classificationm['-'].push_back(index); break;

	case '*': if (initialise_classification('*', index)); else _symbols_classificationm['*'].push_back(index); break;

	case '/': if (initialise_classification('/', index)); else _symbols_classificationm['/'].push_back(index); break;

	default:;

	}
}

std::string expression::give_function(int &index) {
	//It will return a function given the index of its starting brackets
	//It is given by calculating the distances between the brackets and all other functions lying behind them.
	//The minimum distance is used to find the index to the function that matches the brackets
	std::vector<int> distances;
	int distance;
	std::size_t at;
	if (!_repeated_values) { //If there are no repeated values we iterate through a map
		typedef  std::map<int, std::string>::const_iterator indices; //It extracts the indices to a function from the _index_to_function map (see above)
		for (indices iter = _index_to_functionm.begin(); iter != _index_to_functionm.end(); iter++) {
			distance = index - iter->first;
			if (distance > 0)
				distances.push_back(distance);
			else
				continue;
		}
	}
	else { //Else we iterate through a multimap
		typedef std::multimap<int, std::string>::const_iterator indicesr;
		for (indicesr iter = _index_to_functionmr.begin(); iter != _index_to_functionmr.end(); iter++) {
			distance = index - iter->first; //We calculate distance
			if (distance > 0) //Filter out negative values
				distances.push_back(distance); //and store the results in a vector
			else
				continue;
		}
	}
	auto min = std::min_element(std::begin(distances), std::end(distances));
	at = std::distance(distances.begin(), min);
	return _index_to_functionm[index - distances[static_cast<int>(at)]]; //Function is returned from a hash table
}

std::vector<int> expression::brackets_bag() {
	//It collects the positions of starting brackets in order of appereance
	std::vector<int> brackets_positions;
	std::vector<std::vector<std::size_t>> positions;
	std::size_t bracket_index; //TODO: Unused variable
	for (size_t i = 0; i < _expressionarr.size(); i++) {
		if (_expressionarr[i] == "(") {
			brackets_positions.push_back(i); //We need to cast a size_t value to int
			_index_to_bracketsp.first = i;
			_index_to_bracketsp.second = true; //true stands for '('
			_index_to_bracketsm.insert(_index_to_bracketsp);
		}
		else if (_expressionarr[i] == ")") {
			brackets_positions.push_back(i);
			_index_to_bracketsp.first = i;
			_index_to_bracketsp.second = false; //false stands for ')'
			_index_to_bracketsm.insert(_index_to_bracketsp);
		}
	}
	return brackets_positions;
}

void expression::test3() {
	typedef std::multimap<std::string, std::vector<int>>::const_iterator MapIterator;
	for (MapIterator iter = _function_to_rangemr.begin(); iter != _function_to_rangemr.end(); iter++) {
		std::cout << "Function: " << iter->first << "\nArguments:" << std::endl;
		typedef std::vector<int>::const_iterator VectorIterator;
		for (VectorIterator vect_iter = iter->second.begin(); vect_iter != iter->second.end(); vect_iter++) {
			std::cout << " " << *vect_iter << std::endl;
		}
	}
}

void expression::test2() {
	typedef std::multimap<std::tuple<int, int>, std::tuple<int, int>>::const_iterator Iterator;
	for (Iterator iter = _SE_to_OD.begin(); iter != _SE_to_OD.end(); iter++) {
		std::cout << "SE: " << std::get<0>(iter->first) << "\t" << std::get<1>(iter->first) << "\nOD: " << std::get<0>(iter->second) << "\t" << std::get<1>(iter->second) << std::endl;
	}
}

void expression::test1() {
	typedef std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>>::const_iterator Iterator;
	for (Iterator iter = _function_to_inside_functionm.begin(); iter != _function_to_inside_functionm.end(); iter++) {
		std::cout << std::get<0>(iter->first) << "\t" << std::get<1>(iter->first) << "\n" << "Inside: ";
		for (const auto &i : iter->second) {
			std::cout << std::get<0>(i) << "\t" << std::get<1>(i) << "\n" << std::endl;
		}
	}
}

void expression::find_scopes() {
	//It stores each function along with its argument range, defined by the brackets position in a map
	Parser parse = Parser(_expression);
	_expressionarr = parse.detect_functions();
	_repeated_values = true;
	_use_multimap = true;
	insert_index_to_function();
	insert_index_to_symbols();
	std::vector<int> brackets_positions = brackets_bag();
	std::stack<int> pending_brackets; //Brackets whose potential partners were already taken:(
	int single_bracket; //A bracket looking for a partner
	std::vector<int> arguments_range; //It will serve as a temporary container to store values in a hash table
	for (unsigned i = (int)brackets_positions.size(); i-- > 0;) {
		if (!_index_to_bracketsm[brackets_positions[i]]) { //If brackets are ending brackets ')'
			if (!_index_to_bracketsm[brackets_positions[i - 1]]) { //If its previous brackets are also ending brackets
				pending_brackets.push(brackets_positions[i]); //We push it into a stack
			} //The logic might seem counter-intuitive at first glance, keep in mind we are iterating backwards
			else {
				arguments_range.push_back(brackets_positions[i]); //We push the position for ending brackets
				arguments_range.push_back(brackets_positions[i - 1]); //We push the position for starting brackets
				_function_to_rangep.first = give_function(brackets_positions[i]); //Values for ending and starting brackets are swapped when being stored
				_function_to_rangep.second = arguments_range; //That way the computer stores them in a start-to-end order
				if (!_repeated_values) //We store in a map
					_function_to_rangem.insert(_function_to_rangep);
				else //We store in a multimap
					_function_to_rangemr.insert(_function_to_rangep);
				arguments_range.clear(); //We clear for the next value to be stored
			}
		}
		else if (!_index_to_bracketsm[brackets_positions[i + 1]]) { //If the previous bracket to our current bracket is an ending bracket ')' i.e if ')))...)'
			; //We ignore it, this way we make sure that when the next condition is met there will be a '(' to ')' relationship
		}
		else { //If our current bracket is a starting bracket '(' i.e if we have '()'
			single_bracket = pending_brackets.top(); //We get our previously stored value from the stack
			arguments_range.push_back(single_bracket); //We push the position for ending brackets
			arguments_range.push_back(brackets_positions[i]); //We push the position for starting brackets
			pending_brackets.pop(); //And free our friend from the stack
			_function_to_rangep.first = give_function(single_bracket); //Values for ending and starting brackets are swapped when being stored
			_function_to_rangep.second = arguments_range; //That way the computer stores them in a start-to-end order
			if (!_repeated_values) //We store in a map
				_function_to_rangem.insert(_function_to_rangep);
			else //We store in a multimap
				_function_to_rangemr.insert(_function_to_rangep);
			arguments_range.clear();
		}
	}
}

void expression::insert_SE_OD(std::tuple<int, int> &SE, std::tuple<int, int> OD) {
	_SE_to_ODp.first = SE;
	_SE_to_ODp.second = OD;
	_SE_to_OD.insert(_SE_to_ODp);
}

void expression::fill_function_to_inside(std::tuple<int, int> &pivot) {
	int S1 = std::get<0>(pivot);
	int E1 = std::get<1>(pivot);
	int S2;
	int E2;
	std::vector<std::tuple<int, int>> functions_inside;
	bool inside = false;
	_function_to_inside_functionp.first = pivot;
	for (auto &i : _function_ranges) {
		S2 = std::get<0>(i);
		E2 = std::get<1>(i);
		if (S1 < S2 && E1 > E2) {
			inside = true;
			functions_inside.push_back(std::make_tuple(S2, E2));
		}
		else
			continue;
	}
	if (!inside)
		functions_inside.push_back(pivot);
	else
		;
	_function_to_inside_functionp.second = functions_inside;
	_function_to_inside_functionm.insert(_function_to_inside_functionp);
}

void expression::find_functions_inside() { //Refactoring needed
	bool inside;
	int depth;
	int order;
	std::vector<std::tuple<int, int>> order_depth;
	std::tuple<int, int> pivot;
	while (!_function_ranges.empty()) {
		depth = 0;
		order = 0;
		pivot = _function_ranges.front();
		_sorted_SB_EB.push_back(pivot);
		_function_ranges.pop_front();
		if (!_function_to_inside_functionm.empty()) {
			typedef std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>>::const_iterator MapIterator; //We calculate the depth by checking if the function exists in other functions
			for (MapIterator iter = _function_to_inside_functionm.begin(); iter != _function_to_inside_functionm.end(); iter++) {
				auto found = std::find(iter->second.begin(), iter->second.end(), pivot);
				if (found != iter->second.end()) {
					inside = true;
					depth += 1;
				}
				else
					continue;
			}
			if (!inside) { //If function is not inside any function we have depth 0
						   //If it is level 0 we can check its order without caring about other functions being stored previously
				for (size_t i = 0; i < order_depth.size(); i++) {
					if (std::get<0>(order_depth[i]) == 0)
						order = std::get<1>(order_depth[i]) + 1;
					else
						continue;
				}
				order_depth.push_back(std::make_tuple(depth, order));
				insert_SE_OD(pivot, std::make_tuple(depth, order));
			}
			else { //If it is not inside, the order_depth vector has already been filled
				for (unsigned i = (int)order_depth.size(); i-- > 0;) { //We make sure that the new tuple is stored with the right order value, comparing with previously stored tuples
					if (std::get<0>(order_depth[i]) > depth) {
						continue;
					}
					else if (std::get<0>(order_depth[i]) < depth) { //If previous value has lower depth, our new order value will always be 1
						order = 1;
						order_depth.push_back(std::make_tuple(depth, order));
						insert_SE_OD(pivot, std::make_tuple(depth, order));
						break;
					}
					else {
						order = std::get<1>(order_depth[i]) + 1;
						order_depth.push_back(std::make_tuple(depth, order));
						insert_SE_OD(pivot, std::make_tuple(depth, order));
						break;
					}
				}
			}
		}
		else {
			order = 1;
			order_depth.push_back(std::make_tuple(depth, order));
			insert_SE_OD(pivot, std::make_tuple(depth, order));
		}
		fill_function_to_inside(pivot);
	}
}

void expression::fill_function_ranges() {
	std::vector<int> indices_buffer;
	if (!_repeated_values) {
		typedef std::map<std::string, std::vector<int>>::const_iterator iterator;
		for (iterator it = _function_to_rangem.begin(); it != _function_to_rangem.end(); it++) {
			indices_buffer.push_back(it->second[1]);
		}
		std::sort(indices_buffer.begin(), indices_buffer.end());
		for (int sindex : indices_buffer) {
			for (iterator it = _function_to_rangem.begin(); it != _function_to_rangem.end(); it++) {
				if (it->second[1] == sindex) {
					_function_ranges.push_back(std::make_tuple(it->second[1], it->second[0]));
					break;
				}
				else
					continue;
			}
		}
	}
	else {
		typedef std::multimap<std::string, std::vector<int>>::const_iterator iteratorr;
		for (iteratorr it = _function_to_rangemr.begin(); it != _function_to_rangemr.end(); it++) {
			indices_buffer.push_back(it->second[1]);
		}
		std::sort(indices_buffer.begin(), indices_buffer.end());
		for (int sindex : indices_buffer) {
			for (iteratorr it = _function_to_rangemr.begin(); it != _function_to_rangemr.end(); it++) {
				if (it->second[1] == sindex) {
					_function_ranges.push_back(std::make_tuple(it->second[1], it->second[0]));
					break;
				}
				else
					continue;
			}
		}
	}
}

void expression::function_parser() {
	std::vector<std::tuple<int, int>> order_depth;
	std::tuple<int, int> pivot;
	fill_function_ranges();
	find_functions_inside();
	test1();
	test2();
	test3();
}

void expression::sort_tuples_vector(std::vector<std::tuple<int, int>> &tuples_vector) {//////////////FIRST TEST
	std::sort(std::begin(tuples_vector), std::end(tuples_vector), [](std::tuple<int, int> const &t1, std::tuple<int, int> const &t2) {
		return std::get<0>(t1) < std::get<0>(t2);
	});
}

void expression::sort_tuples_queue(std::deque<std::tuple<std::tuple<int, int>, std::tuple<int, int>>> &tuples_queue) {///////////////SECOND TEST
	/*std::sort(std::begin(tuples_queue), std::end(tuples_queue), [](std::tuple<std::tuple<int, int>, std::tuple<int, int>> const &t1) {
		return std::get<1>(std::get<0>(t1)) < std::get<1>(std::get<1>(t1));
	});*/ //TODO: FIX ME!!!!
}

void expression::differentiate_level_by_level(std::map<std::tuple<int, int>, bool> &differentiated_functions, std::map<std::tuple<int, int>, std::tuple<std::string, std::string>> &scope_to_expression_to_derivative, std::deque<std::string> &polynomial_derivatives) {
	/* As if we were opening a russian doll from the inside, that's how this function is processing each function argument, from the inner (which always contains a polynomial) to the outer-most degrees of depth*/
	typedef std::tuple<std::string, std::string> argument_to_derivative;
	typedef std::tuple<std::tuple<int, int>, std::tuple<int, int>> SE_OD; //Starting and ending brackets' indices
	typedef std::string sharp_standard;
	std::map<sharp_standard, argument_to_derivative> sharp_parsing; //
	std::deque<SE_OD> orders_of_depth; //How deep the functions are
	std::deque<std::tuple<int, int>> current_level; //Functions which are currently to be processed
	std::deque<std::string> previous_derivatives; //We fill it with derivatives to functions
	int sharp_index;
	sharp_standard sharpStandard;
	std::string derivative_inside;
	std::string derivative_outside;
	std::string previous_derivative;
	std::string expression;
	std::tuple<int, int> cur;
	std::tuple<int, int> to_be_deleted; //This container will store
	sort_tuples_vector(_sorted_SB_EB); ////////Test 1
	int deepest_level;
	while (!_sorted_SB_EB.empty()) { //While there is some function we haven't processed yet
		sharp_index = 1;
		for (std::tuple<int, int> SB_EB : _sorted_SB_EB) { //First we find out what arguments are lying at the deepest not-yet-processed layers
			orders_of_depth.push_back(std::make_tuple(SB_EB, _SE_to_OD[SB_EB]));
		}
		sort_tuples_queue(orders_of_depth);  //They are always being stored in a left to right(increasing) order //TEST 1 are elements sorted?
		deepest_level = std::get<0>(std::get<1>(orders_of_depth.front())); //The deepest level is always the last element in the queue, since we have sorted the queue in increasing order
																		   ///////Test 2 is it really the deepest level? it will be if the tuple queue is properly sorted
		orders_of_depth.pop_front();
		while (std::get<0>(std::get<1>(orders_of_depth.front())) == deepest_level) { //While the functions being extracted from the deque are at the same level of depth (we check the first elemement of the second tuple for that)
																					 //We fill up current level, it will contain all functions that can be found at the same degree of depth
			cur = std::get<0>(orders_of_depth.front()); //We pick up the starting and ending brackets of a function at the current level
			current_level.push_back(cur); //We push them into the queue containing all functions found at the same level of depth
			orders_of_depth.pop_front();
			to_be_deleted = (cur); //TODO: std::get<0> removed here  //Since we are going to process this function already before the loop reaches it, we can delete it
			_sorted_SB_EB.erase(std::remove(_sorted_SB_EB.begin(), _sorted_SB_EB.end(), to_be_deleted), _sorted_SB_EB.end()); //We get rid of the already differentiated function
		}////////////////////Test 3
		 //We revert the current level
		for (std::tuple<int, int> function : current_level) { //We are going from right to left in order to pick up the previous polynomials as we go
			if (std::get<0>(function) == 0) { //When we have the outer most function
				Argument argument = Argument(expression, previous_derivatives);
				derivative_inside = argument.differentiate();
				//Derivative_outside
				//return Derivative_inside * derivative outside
			}
			else {
				subvector_to_string(function, expression); //We set up the expression to be differentiated
														   /////////Test 4 it should make sure that the brackets won't be added to the expression to be differentiated
				Argument argument = Argument(expression, previous_derivatives);
				//Previous derivatives must be tokenised so that the expressions can be parsed without taking the trigonometric functions' arguments into account
				//They'll be parsed in order by the # standard (i.e #1, #2, #3) they will be extracted from a map when needed
				//The previous derivatives are going to replace our current functions, after the expression has been simplified
				derivative_inside = argument.differentiate(); //The derivative inside is always definitive
															  //derivative_outside = simple derivative function returns in this case 1 * -sin hint: use get_function
															  //We would always have to get rid of the one in cases like this
															  //Derivative outside is a potential derivative we can use unless there is a simplification that changes our original functions
															  //That means original values won't be replaced unless there is a valid simplification
															  //In this example we would have a product rule which means that derivative outside would be used in the product rule formula
				if (derivative_inside != "1") {//TODO: 1 changed to "1" ////////Test 5 it should get rid of the one in this case
					sharp_index += 1;
					sharpStandard = "#" + std::to_string(sharp_index);
					previous_derivative = derivative_inside + "*" + derivative_outside;
					previous_derivatives.push_back(previous_derivative);
				}
				else {
					sharp_index += 1;
					sharpStandard = "#" + std::to_string(sharp_index);
					previous_derivatives.push_back(derivative_outside);//TODO: Instead of a queue it should be a map (sharp to argument(the expression variable in this case) and its derivative
				}
				//The purpose to previous derivative is so we store the derivative to our previous elements for a potential product, quotient rule to be performed at an upper level
			}
		}
		sharpStandard.clear();
		//TODO: Write a module that handles simple derivatives (i.e sin(#1) or ln(#1)
		//Example 1: tan(3x+ sin(x + cos(2x)*ln(2x)) - 2 * ln(x)) here outside derivative is indeed valid since the result of our simplification leaves the functions intact, it would be used in the product rule
		//the program reads at the second level x + cos(#1)*ln(#2) using the map we extract the derivative and the expression to perform the product rule here

		//Example 2: tan(3x + sin(x + cos(x)*cos(x)) - 2 * ln(x)) here outside derivative won't be valid since there is a simplification with cos also since derivative inside is one we don't add anything
		//because cos(x) can be operated since functions and arguments are same for both, giving us cos square
		//The derivative to cos^2(x) would then be calculated
		//Note: The original argument to the function must be stored in a map where #1: argument_to_derivative, #2: argument_to_derivative
		//Once we have it, we pick up the function that contains them and process the argument using the methods of the argument class
		//TODO: Write on paper the algorithm steps
		/////////////////////////////////1st example tan(3x+ sin(x + cos(x)*ln(x)) - 2 * ln(x))

		//First iteration Note: the first iteration is always going to have a polynomial as argument, the next will always include a function
		//outside derivative = -sin(1#)
		//previous_derivatives.push_back(outside_derivative))

		//Second iteration
		//outside_derivative = 1/2#
		//previous_derivatives.push_back(outside_derivative)) Note: Since it is a queue, we know elements will always be processed in order in the next upper level

		////////////////UPPER LEVEL
		///////////next we have as argument (x + cos(x)*ln(x)) and previous_derivatives(queue) = {1/2#, -sin(1#)} tokenised_arguments #1: x, #2: x
		//////////when the simplify function is called we know there is a product rule and so we leave it there before it has to be differentiated
		/////////that's when previous derivatives will come into play


		/////////////////////////////////2nd example tan(3x + sin(x + cos(x)*cos(x)) - 2 * ln(x))


		///////////We will have the same here but the simplify function must handle the cos(x)*cos(x) case another map will be needed that maps tokenised arguments to functions #1: cos #2: ln
		//////////Leaving us with previous_derivatives(queue) = {-sin(#2), -sin(#1)} tokenised arguments #1: x, #2: x and another map #1: cos #2: cos
		//////////When elements are passed into the argument, they will sequentially be tokenised in the same way i.e x + cos(#1)*cos(#2)
		/////////The function will handle the special cases where functions and arguments (through the # tokenisation map) are the same
		////////The map to original function will let us know what elements from previous derivatives must be deleted
	}
	//Only at this point will the outside derivative variable be valid since there are no more derivatives to be processed
}


//Welcome to hell...
std::string expression::differentiate() {
	typedef std::map<std::tuple<int, int>, std::tuple<std::string, std::string>>::const_iterator iter;
	typedef std::tuple<int, int> SB_EB;
	typedef std::tuple<std::string, std::string> expression_to_derivative;
	typedef bool derivative_found;
	std::vector<std::tuple<int, int>> derivatives_buffer;
	std::deque<std::string> polynomial_derivatives;
	std::map<SB_EB, expression_to_derivative> scope_to_expression_to_derivative;
	std::pair<SB_EB, derivative_found> differentiated_functionsp;
	std::map<SB_EB, derivative_found> differentiated_functions;
	std::string derivative;
	if (!_function_to_inside_functionm.empty()) { //If there are any trigonometric functions
		differentiate_level_by_level(differentiated_functions, scope_to_expression_to_derivative, polynomial_derivatives); //TODO: polynomial_derivatives added
	}
	else { //We just differentiate a polynomial
		; ///////Start from here, building small simpler blocks
	}
	return derivative;
}

void expression::subvector_to_string(std::tuple<int, int> &indices, std::string &expression) {
	//Assigns string elements from a vector in a given indices range to a string variable
	for (int i = std::get<0>(indices); i <= std::get<1>(indices); i++) { //////////////////////////FOURTH TEST
		expression += _expressionarr[i];
	}
}

expression::expression(std::string expression) {
	_expression = expression;
}

expression::~expression() {

}
