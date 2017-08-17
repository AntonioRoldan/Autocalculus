#include <iostream>
#include <map>
#include <array>
#include <vector>
#include <set>
#include <boost/algorithm/string/find.hpp>
#include<cmath>
#include <numeric>
#include<deque>
#include <stack>
#include <sstream>

class Parser{
    
private:
    std::string _expression;
    std::vector<std::string>_expressionarr;
    bool _use_multimap;
    std::vector<int> _numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'x', 'y', 'z', '^'};
    std::vector<int> _symbols = {'+', '-', '*', '/', '(', ')'};
public:
    std::vector<std::string> detect_functions();
    bool isNumber(char &number);
    bool isSymbol(char &symbol);
    Parser(std::string &expression);
};

Parser::Parser(std::string &expression) {
    _expression = expression;
}

bool Parser::isNumber(char &number) {
    auto pos = std::find(_numbers.begin(), _numbers.end(), number);
    return pos != _numbers.end();
}

bool Parser::isSymbol(char &symbol) {
    auto pos = std::find(_symbols.begin(), _symbols.end(), symbol);
    return pos != _symbols.end();
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
        for (int i = 0; i <= _expression.size(); i++) {
            
            switch (_expression[i]) {
                    
                case 'l' :
                    if (_expression[i + 1] == 'n') {
                        function = "ln";
                        _expressionarr.push_back(function);
                        ln++;
                    } else if (_expression[i + 1] == 'o') {
                        function = "log";
                        _expressionarr.push_back(function);
                        log++;
                    }
                    break;
                    
                case 's' :
                    if (_expression[i + 1] == 'i') {
                        if (_expression[i - 3] == 'a');
                        else {
                            function = "sin";
                            _expressionarr.push_back(function);
                            sin++;
                        }
                    } else if (_expression[i - 2] == 'c') {
                        if (_expression[i + 1] == 'e') { ;
                        } else if (_expression[i - 1] == 'c') {
                            function = "arcsin";
                            _expressionarr.push_back(function);
                            arcsin++;
                        } else {
                            function = "cos";
                            _expressionarr.push_back(function);
                            cos++;
                        }
                    } else if (_expression[i + 1] == 'e') {
                        function = "sec";
                        _expressionarr.push_back(function);
                        sec++;
                    }
                    break;
                    
                case 'c' :
                    if (_expression[i + 1] == 'o') {
                        if (_expression[i - 2] == 'a');
                        else if (_expression[i + 3] == 'e') {
                            function = "cosec";
                            _expressionarr.push_back(function);
                            cosec++;
                        } else if (_expression[i + 2] == 's') {
                            function = "cos";
                            _expressionarr.push_back(function);
                            cos++;
                        }
                    } else if (_expression[i + 1] == 's') //If it is arcsin we will cause a redundance
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
                    } else {
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
                    else if (isNumber(_expression[i]) and !previous_number) {
                        if (!catch_exponent) {
                            number += _expression[i];
                        } else {
                            exponent += _expression[i];
                        }
                        previous_number = true;
                    } else if (isNumber(_expression[i]) and previous_number) {
                        if (!catch_exponent)
                            number += _expression[i];
                        else
                            exponent += _expression[i];
                    } else if (!isNumber(_expression[i]) and previous_number) {
                        if (_expression[i] == '^') {
                            if (_expression[i - 1] == 'x' or
                                _expression[i - 1] == 'y') {//If it is a polynomial raised to some power
                                number += _expression[i]; //Number will be added its exponent symbol
                                catch_exponent = true;
                                polynomial_raised = true;
                            } else {
                                catch_exponent = true;
                                integer_raised = true;
                            }
                        } else if (catch_exponent and integer_raised) {
                            number = std::to_string(std::pow(std::stoi(number), std::stoi(exponent)));
                            _expressionarr.push_back(number);
                            exponent.clear();
                            integer_raised = false;
                        } else if (catch_exponent and polynomial_raised) {
                            number += exponent; //Number will be added its exponent so it will later be processed by differentiate_monomial
                            _expressionarr.push_back(number);
                            exponent.clear();
                            polynomial_raised = false;
                        } else {
                            previous_number = false;
                            _expressionarr.push_back(number);
                            number.clear();
                        }
                    }
                    if (isSymbol(_expression[i])) { //TODO Get the program to process exponents to expressions between brackets as functions
                        if (_expression[i] == '^' and _expression[i - 1] ==
                            ')') //If we have an exponent to an expression between brackets, we can interpret it as a function
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
    for (int i = 0; i < _expressionarr.size(); i++) {
        std::cout << _expressionarr[i] << std::endl;
    }
    parsed_expression = _expressionarr;
    return parsed_expression;
}

class algebraic_manipulation {
private:
    
    std::vector<int> _numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    struct Binomial{
        bool double_subtraction;
        bool equivalent_exponentiation;
        bool non_exponentiated_monomials;
        bool both_integers;
        bool addition_or_subtraction;
        std::string expression_a;
        std::string expression_b;
    };
    Binomial binomial;
    
public:
    
    std::string addMonomials(std::string &a, std::string &b);
    int perform_arithmetic(int a_integer, int b_integer);
    void check_arithmetic_equivalency(std::string &a, std::string &b);
    std::string add_sub(std::string &a, std::string &b);
    std::string arithmetic(std::string &a, std::string &b);
    std::string mulMonomials(std::string &a, std::string &b);
    std::string subMonomials(std::string &a, std::string &b);
    bool hasExponent(const std::string & str);
    bool isNumber(char &number);
    void reset_binomial();
    algebraic_manipulation(bool &double_subtraction);
};

bool algebraic_manipulation::isNumber(char &number) {
    auto pos = std::find(_numbers.begin(), _numbers.end(), number);
    return pos != _numbers.end();
}

bool algebraic_manipulation::hasExponent(const std::string & str)
{
    const char c = '^';
    return str.find(c) != std::string::npos;
}

std::string algebraic_manipulation::subMonomials(std::string &a, std::string &b) {
    std::string result;
    binomial.addition_or_subtraction = false;
    result = arithmetic(a, b);
    if(std::stoi(result) < 0)
        result = "-" + result;
    else
        result = "+" + result;
    return result;
}

//addition or subtraction,
std::string algebraic_manipulation::mulMonomials(std::string &a, std::string &b) {
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
    if(a == "null" or b == "null")
        return "null";
    if(hasExponent(a) and hasExponent(b)){ //3x^2 * 4x^2 will be equal to 12x^4
        pos = (int) a.find(exponent);
        posb = (int) b.find(exponent);
        a_integer = std::stoi(a.substr(0, (unsigned long) pos - 2));
        b_integer = std::stoi(b.substr(0, (unsigned long) posb - 2));
        a_exponentint = std::stoi(a.substr((unsigned long) (pos + 1), a.size()));
        b_exponentint = std::stoi(b.substr((unsigned long) (posb + 1), b.size()));
        result_exponent = a_exponentint + b_exponentint;
        result_integer = a_integer * b_integer;
        result = std::to_string(result_integer) + 'x' + '^' + std::to_string(result_exponent);
    }
    else if(hasExponent(a) and !hasExponent(b)) { //3x^2 and 4x or 4
        pos = (int) a.find(exponent);
        a_integer = std::stoi(a.substr(0, (unsigned long) pos - 2)); //We store 3 here
        a_exponent.assign(a.begin() + pos - 1, a.begin() + pos); //pos - 1 to ensure we get the variable as well it will store x^ here
        a_exponentint = std::stoi(a.substr(pos + 1, a.size())); //We store the 2 for the exponent here
        if (isNumber(b.back())) { // 3x^2 * 4
            b_integer = std::stoi(b);
            result = std::to_string(a_integer * b_integer) + a_exponent + std::to_string(a_exponentint); //is 12x^2
        } else { // 3x^2 * 4x
            b_integer = std::stoi(b.substr(0, b.size() - 1));
            a_exponentint += 1;
            a_exponent += std::to_string(a_exponentint);
            result = std::to_string(a_integer * b_integer) + a_exponent;
        }
    }
    else if(!hasExponent(a) and hasExponent(b)){
        pos = (int) b.find(exponent);
        b_integer = std::stoi(b.substr(0, (unsigned long) pos - 1));
        b_exponent.assign(b.begin() + pos - 1, b.begin() + pos);
        b_exponentint = std::stoi(b.substr((unsigned long) (pos + 1), b.size()));
        if(isNumber(a.back())){ //4 * 3x^2
            a_integer = std::stoi(a);
            result = std::to_string(a_integer * b_integer) + b_exponent + std::to_string(b_exponentint);
        }
        else{ //4x * 3x^2
            a_integer = std::stoi(a.substr(0, a.size() - 1));
            b_exponentint += 1;
            a_exponent += std::to_string(b_exponentint);
            result = std::to_string(a_integer * b_integer) + b_exponent;
        }
    }
    else if(isNumber(a.back()) and b.back() == 'x') { //4 * 3x
        a_integer = std::stoi(a);
        b_integer = std::stoi(b.substr(0, b.size() - 1));
        result = std::to_string(a_integer * b_integer) + b.back();
    }
    else if(a.back() == 'x' and b.back() == 'x'){ //3x * 4x
        a_integer = std::stoi(a.substr(0, a.size() - 1));
        b_integer = std::stoi(b.substr(0, b.size() - 1));
        result = std::to_string(a_integer * b_integer) + a.back(); //a.front() is 'x'
    }
    else if(a.back() == 'x' and isNumber(b.back())){ //3x * 4
        a_integer = std::stoi(a.substr(0, a.size() - 1));
        b_integer = std::stoi(b);
        result = std::to_string(a_integer * b_integer) + a.back();
    }
    else{
        a_integer = std::stoi(a);
        b_integer = std::stoi(b);
        result = std::to_string(a_integer * b_integer);
    }
    return result;
}


std::string algebraic_manipulation::addMonomials(std::string &a, std::string &b){
    std::string result;
    binomial.addition_or_subtraction = true;
    result = arithmetic(a, b);
    if(binomial.double_subtraction)
        result += "-";
    return result;
}

int algebraic_manipulation::perform_arithmetic(int a_integer, int b_integer){
    int result_integer;
    result_integer = binomial.addition_or_subtraction ? a_integer + b_integer : a_integer - b_integer;
    return result_integer;
}

void algebraic_manipulation::reset_binomial(){
    binomial.equivalent_exponentiation = false;
    binomial.non_exponentiated_monomials = false;
    binomial.both_integers = false;
}

void algebraic_manipulation::check_arithmetic_equivalency(std::string &a, std::string &b){
    reset_binomial();
    std::string a_exponent;
    std::string b_exponent;
    if(hasExponent(a) and hasExponent(b)) { //If they both have exponents
        auto pos = a.find('^');
        auto posb = b.find('^');
        a_exponent.assign(a.begin() + pos + 1, a.begin() + a.size());
        b_exponent.assign(b.begin() + posb + 1, b.begin() + b.size());
    }
    binomial.equivalent_exponentiation = a_exponent != b_exponent ? true : false; //if equivalent exponentiation is set to false the operation will stop
    if((hasExponent(a) and !hasExponent(b)) or (!hasExponent(a) and hasExponent(b))) {
        binomial.equivalent_exponentiation = false; //and we gather the other elements
    }
    else if(a.front() == 'x' and a.front() == b.front()){
        binomial.non_exponentiated_monomials = true;
    }
    else if(a.front() != 'x' and b.front() != 'x'){
        binomial.both_integers = true;
    }
}


std::string algebraic_manipulation::add_sub(std::string &a, std::string &b){
    std::string a_exponent;
    std::string b_exponent;
    int a_integer;
    int b_integer;
    int result_integer;
    std::string result;
    if(binomial.equivalent_exponentiation){ //3x^2 + 2x^2
        auto pos = a.find('^');
        auto posb = b.find('^');
        a_exponent.assign(a.begin() + pos + 1, a.begin() + a.size());
        b_exponent.assign(b.begin() + posb + 1, b.begin() + b.size());
        a_integer = std::stoi(a.substr(0, pos - 2)); //-2 to make sure we get rid of the x
        b_integer = std::stoi(b.substr(0, posb - 2));
        result_integer = perform_arithmetic(a_integer, b_integer);
        result = std::to_string(result_integer) + 'x' + '^' + a_exponent;
    }
    else if(binomial.non_exponentiated_monomials){ //3x + 2x
        a_integer = std::stoi(a.substr(0, a.size() - 1));
        b_integer = std::stoi(b.substr(0, b.size() - 1));
        result_integer = perform_arithmetic(a_integer, b_integer);
        result = std::to_string(result_integer) + 'x';
    } else if(binomial.both_integers){ //3 + 2
        a_integer = std::stoi(a);
        b_integer = std::stoi(b);
        result_integer = perform_arithmetic(a_integer, b_integer);
        result = std::to_string(result_integer);
    } else{
        ;//Provided that we have gathered up our elements properly this block should activate a boolean that stops the processing
    }
    return result;
}


std::string algebraic_manipulation::arithmetic(std::string &a, std::string &b) {
    std::string result;
    check_arithmetic_equivalency(a, b);
    result = add_sub(a, b);
    return result;
}

algebraic_manipulation::algebraic_manipulation(bool &double_subtraction) {
    binomial.double_subtraction = double_subtraction;
}


class derivative{
    
    friend class argument;
    
private:
    
    std::string _expression;
    std::deque<std::tuple<int, int>> _function_ranges;
    std::vector<std::string> _expressionarr;
    std::vector<char> _symbols_stored;
    bool _use_multimap = false;
    std::vector<int> _numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'x', 'y', 'z', '^'};
    std::vector<int> _symbols = {'+', '-', '*', '/', '(', ')'};
    std::vector<std::string> _func = {"log", "ln", "sin", "cos", "tan", "sec", "cosec", "cotan", "e", "arcsin", "arcos", "arctan"};
    std::pair<std::string, std::vector<int>> _function_to_rangep; //Stores function given as a string and a vector containing the chain rule iterations and the indices for the argument
    std::map<std::string, std::vector<int>> _function_to_rangem;
    std::multimap<std::string, std::vector<int>> _function_to_rangemr; //If there are repeated values
    std::pair<std::size_t, bool> _index_to_bracketsp; //Stores position of brackets and a boolean (false for '(' true for ')')
    std::map<std::size_t, bool> _index_to_bracketsm;
    std::pair<char, std::vector<int>> _symbols_classificationp; //Keys are the symbols, they are mapped to their indices
    std::map<char,std::vector<int>> _symbols_classificationm;
    std::pair<int , char> _index_to_symbolsp;
    std::map<int, char> _index_to_symbolsm;
    std::pair<int, std::string> _index_to_functionp; //Stores each function as a key with its corresponding index in the expression
    std::map<int, std::string> _index_to_functionm;
    std::multimap<int, std::string> _index_to_functionmr; //Same for repeated values
    std::pair<std::tuple<int, int>, std::vector<std::tuple<int, int>>> _function_to_inside_functionp; //Keys: function's range Values: ranges of the functions inside it
    std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>> _function_to_inside_functionm;
    std::pair<std::tuple<int, int>, std::tuple<int , int>> _SE_to_ODp;
    std::map<std::tuple<int, int>, std::tuple<int, int>> _SE_to_OD;
    bool _repeated_values = false;
    
public:
    
    void detect_functions();
    bool isNumber(char &number);
    bool isSymbol(char &symbol);
    bool initialise_classification(char symbol, int &index);
    void set_index_to_symbols(char &symbol, int &index);
    void insert_index_to_symbols();
    void insert_index_to_function();
    void insert_SE_OD(std::tuple<int, int> &SE, std::tuple<int, int> OD);
    void insert_symbol(char &symbol, int &index);
    void fill_function_ranges();
    std::string give_function(int &index);
    std::vector<int> brackets_bag();
    void find_scopes();
    void find_functions_inside();
    void fill_function_to_inside(std::tuple<int, int> &pivot);
    std::string differentiate_argument(int &SB, int &EB, bool is_polynomial);
    std::string differentiate();
    std::string differentiate_function(std::string function);
    bool isFunction(std::string &pfunction);
    void function_parser();
    void test1();
    void test2();
    void test3();
    derivative(std::string expression); //Copy constructor
    derivative(derivative&&) = default; //Move constructor
    derivative& operator = (const derivative&) & = default; //Move constructor
    derivative& operator = (derivative&&) & = default; //Copy assignment operator
    ~derivative(); //Destructor
};


bool derivative::isNumber(char &number) {
    auto pos = std::find(_numbers.begin(), _numbers.end(), number);
    return pos != _numbers.end();
}

bool derivative::isSymbol(char &symbol) {
    auto pos = std::find(_symbols.begin(), _symbols.end(), symbol);
    return pos != _symbols.end();
}

void derivative::insert_index_to_function(){
    for(int i = 0; i < _expressionarr.size(); i++){
        auto isfunction = std::find(_func.begin(), _func.end(), _expressionarr[i]);
        if(isfunction != _func.end()){
            _index_to_functionp.first = i;
            _index_to_functionp.second = _expressionarr[i];
            if(!_repeated_values)
                _index_to_functionm.insert(_index_to_functionp);
            else
                _index_to_functionmr.insert(_index_to_functionp);
        }
        else
            continue;
    }
}


bool derivative::initialise_classification(char symbol, int &index) {
    std::vector<int> symbols;
    auto is_stored = std::find(_symbols_stored.begin(), _symbols_stored.end(), symbol);
    if(is_stored != _symbols_stored.end())
        return false;
    else if(symbols.empty() or is_stored == _symbols_stored.end()){
        _symbols_stored.push_back(symbol);
        symbols.push_back(index);
        _symbols_classificationp.first = symbol;
        _symbols_classificationp.second = symbols;
        _symbols_classificationm.insert(_symbols_classificationp);
        return true;
    }
    return false;
}

void derivative::set_index_to_symbols(char &symbol, int &index){
    _index_to_symbolsp.first = index;
    _index_to_symbolsp.second = symbol;
    _index_to_symbolsm.insert(_index_to_symbolsp);
}

void derivative::insert_index_to_symbols() {
    for(int i = 0; i < _expressionarr.size(); i++){
        auto issymbol = std::find(_symbols.begin(), _symbols.end(), _expressionarr[i][0]);
        if(issymbol != _symbols.end()){
            insert_symbol(_expressionarr[i][0], i); //We index the string so as tu turn it into a character
        }
    }
}

void derivative::insert_symbol(char &symbol, int &index) {
    set_index_to_symbols(symbol, index);
    switch(symbol){
            
        case '+' : if(initialise_classification('+', index)); else _symbols_classificationm['+'].push_back(index); break;
            
        case '-' : if(initialise_classification('-', index)); else _symbols_classificationm['-'].push_back(index); break;
            
        case '*' : if(initialise_classification('*', index)); else _symbols_classificationm['*'].push_back(index); break;
            
        case '/' : if(initialise_classification('/', index)); else _symbols_classificationm['/'].push_back(index); break;
            
        default : ;
            
    }
}


std::string derivative::give_function(int &index){
    //It will return a function given the index of its starting brackets
    //It is given by calculating the distances between the brackets and all other functions lying behind them.
    //The minimum distance is used to find the index to the function that matches the brackets
    std::vector<int> distances;
    int distance;
    std::size_t at;
    if(!_repeated_values){ //If there are no repeated values we iterate through a map
        typedef  std::map<int, std::string>::const_iterator indices; //It extracts the indices to a function from the _index_to_function map (see above)
        for( indices iter = _index_to_functionm.begin(); iter != _index_to_functionm.end(); iter++){
            distance = index - iter->first;
            if(distance > 0)
                distances.push_back(distance);
            else
                continue;
        }
    }
    else{ //Else we iterate through a multimap
        typedef std::multimap<int, std::string>::const_iterator indicesr;
        for(indicesr iter = _index_to_functionmr.begin(); iter != _index_to_functionmr.end(); iter++){
            distance = index - iter->first; //We calculate distance
            if(distance > 0) //Filter out negative values
                distances.push_back(distance); //and store the results in a vector
            else
                continue;
        }
    }
    auto min = std::min_element(std::begin(distances), std::end(distances));
    at = std::distance(distances.begin(), min);
    return _index_to_functionm[index - distances[static_cast<int>(at)]]; //Function is returned from a hash table
}


std::vector<int> derivative::brackets_bag(){
    //It collects the positions of starting brackets in order of appereance
    //Find function returns the last index of a repeated element
    //We need to use the find_nth function from boost and keep track of occurrences
    std::vector<int> brackets_positions;
    std::vector<std::vector<std::size_t>> positions;
    boost::iterator_range<std::string::iterator> pos;
    std::size_t bracket_index;
    for(int i = 0; i < _expressionarr.size(); i++){
        if(_expressionarr[i] == "(") {
            brackets_positions.push_back(i); //We need to cast a size_t value to int
            _index_to_bracketsp.first = i;
            _index_to_bracketsp.second = true; //true stands for '('
            _index_to_bracketsm.insert(_index_to_bracketsp);
        }
        else if(_expressionarr[i] == ")")  {
            brackets_positions.push_back(i);
            _index_to_bracketsp.first = i;
            _index_to_bracketsp.second = false; //false stands for ')'
            _index_to_bracketsm.insert(_index_to_bracketsp);
        }
    }
    return brackets_positions;
}

void derivative::test3(){
    typedef std::multimap<std::string, std::vector<int>>::const_iterator MapIterator;
    for(MapIterator iter = _function_to_rangemr.begin(); iter != _function_to_rangemr.end(); iter++){
        std::cout << "Function: " << iter->first << "\nArguments:" << std::endl;
        typedef std::vector<int>::const_iterator VectorIterator;
        for(VectorIterator vect_iter = iter ->second.begin(); vect_iter != iter -> second.end(); vect_iter++){
            std::cout << " " << *vect_iter << std::endl;
        }
    }
}


void derivative::test2(){
    typedef std::multimap<std::tuple<int, int>, std::tuple<int, int>>::const_iterator Iterator;
    for(Iterator iter = _SE_to_OD.begin(); iter != _SE_to_OD.end(); iter++){
        std::cout<<"SE: "<< std::get<0>(iter->first) << "\t" << std::get<1>(iter->first) << "\nOD: " << std::get<0>(iter->second) << "\t" << std::get<1>(iter->second) << std::endl;
    }
}

void derivative::test1(){
    typedef std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>>::const_iterator Iterator;
    for(Iterator iter = _function_to_inside_functionm.begin(); iter != _function_to_inside_functionm.end(); iter++){
        std::cout<<std::get<0>(iter->first) << "\t" << std::get<1>(iter->first)<<"\n" << "Inside: ";
        for(const auto &i : iter->second){
            std::cout<<std::get<0>(i) << "\t" << std::get<1>(i) << "\n" << std::endl;
        }
    }
}


void derivative::find_scopes(){
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
    for( unsigned i = (int) brackets_positions.size(); i-- > 0;){
        if(!_index_to_bracketsm[brackets_positions[i]]){ //If brackets are ending brackets ')'
            if(!_index_to_bracketsm[brackets_positions[i - 1]]){ //If its previous brackets are also ending brackets
                pending_brackets.push(brackets_positions[i]); //We push it into a stack
            } //The logic might seem counter-intuitive at first glance, keep in mind we are iterating backwards
            else{
                arguments_range.push_back(brackets_positions[i]); //We push the position for ending brackets
                arguments_range.push_back(brackets_positions[i - 1]); //We push the position for starting brackets
                _function_to_rangep.first = give_function(brackets_positions[i]); //Values for ending and starting brackets are swapped when being stored
                _function_to_rangep.second = arguments_range; //That way the computer stores them in a start-to-end order
                if(!_repeated_values) //We store in a map
                    _function_to_rangem.insert(_function_to_rangep);
                else //We store in a multimap
                    _function_to_rangemr.insert(_function_to_rangep);
                arguments_range.clear(); //We clear for the next value to be stored
            }
        }
        else if(!_index_to_bracketsm[brackets_positions[i + 1]]){ //If the previous bracket to our current bracket is an ending bracket ')' i.e if ')))...)'
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


void derivative::insert_SE_OD(std::tuple<int, int> &SE, std::tuple<int, int> OD){
    _SE_to_ODp.first = SE;
    _SE_to_ODp.second = OD;
    _SE_to_OD.insert(_SE_to_ODp);
}


bool derivative::isFunction(std::string &pfunction) {
    auto isFunction = std::find(_func.begin(), _func.end(), pfunction);
    return isFunction != _func.end();
}


void derivative::fill_function_to_inside(std::tuple<int, int> &pivot){
    int S1 = std::get<0>(pivot);
    int E1 = std::get<1>(pivot);
    int S2;
    int E2;
    std::vector<std::tuple<int, int>> functions_inside;
    bool inside = false;
    _function_to_inside_functionp.first = pivot;
    for (auto &i : _function_ranges){
        S2 = std::get<0>(i);
        E2 = std::get<1>(i);
        if(S1 < S2 and E1 > E2){
            inside = true;
            functions_inside.push_back(std::make_tuple(S2, E2));
        }
        else
            continue;
    }
    if(!inside)
        functions_inside.push_back(pivot);
    else
        ;
    _function_to_inside_functionp.second = functions_inside;
    _function_to_inside_functionm.insert(_function_to_inside_functionp);
}

void derivative::find_functions_inside() { //Refactoring needed
    bool inside;
    int depth;
    int order;
    std::vector<std::tuple<int, int>> order_depth;
    std::tuple<int, int> pivot;
    while(!_function_ranges.empty()){
        depth = 0;
        order = 0;
        pivot = _function_ranges.front();
        _function_ranges.pop_front();
        if(!_function_to_inside_functionm.empty()){
            typedef std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>>::const_iterator MapIterator; //We calculate the depth by checking if the function exists in other functions
            for(MapIterator iter = _function_to_inside_functionm.begin(); iter != _function_to_inside_functionm.end(); iter++){
                auto found = std::find(iter->second.begin(), iter->second.end(), pivot);
                if(found != iter->second.end()){
                    inside = true;
                    depth += 1;
                }
                else
                    continue;
            }
            if(!inside){ //If function is not inside any function we have depth 0
                //If it is level 0 we can check its order without caring about other functions being stored previously
                for(int i = 0; i < order_depth.size(); i++){
                    if(std::get<0>(order_depth[i]) == 0)
                        order = std::get<1>(order_depth[i]) + 1;
                    else
                        continue;
                }
                order_depth.push_back(std::make_tuple(depth, order));
                insert_SE_OD(pivot, std::make_tuple(depth, order));
            }
            else { //If it is not inside, the order_depth vector has already been filled
                for (unsigned i = (int) order_depth.size(); i-- > 0;) { //We make sure that the new tuple is stored with the right order value, comparing with previously stored tuples
                    if(std::get<0>(order_depth[i]) > depth){
                        continue;
                    }
                    else if(std::get<0>(order_depth[i]) < depth){ //If previous value has lower depth, our new order value will always be 1
                        order = 1;
                        order_depth.push_back(std::make_tuple(depth, order));
                        insert_SE_OD(pivot, std::make_tuple(depth, order));
                        break;
                    }
                    else{
                        order = std::get<1>(order_depth[i]) + 1;
                        order_depth.push_back(std::make_tuple(depth, order));
                        insert_SE_OD(pivot, std::make_tuple(depth, order));
                        break;
                    }
                }
            }
        }else{
            order = 1;
            order_depth.push_back(std::make_tuple(depth, order));
            insert_SE_OD(pivot, std::make_tuple(depth, order));
        }
        fill_function_to_inside(pivot);
    }
}

void derivative::fill_function_ranges() {
    std::vector<int> indices_buffer;
    if(!_repeated_values){
        typedef std::map<std::string, std::vector<int>>::const_iterator iterator;
        for(iterator it = _function_to_rangem.begin(); it != _function_to_rangem.end(); it++){
            indices_buffer.push_back(it->second[1]);
        }
        std::sort(indices_buffer.begin(), indices_buffer.end());
        for(int sindex : indices_buffer){
            for(iterator it = _function_to_rangem.begin(); it != _function_to_rangem.end(); it++){
                if (it->second[1] == sindex){
                    _function_ranges.push_back(std::make_tuple(it->second[1], it->second[0]));
                    break;
                } else
                    continue;
            }
        }
    }
    else {
        typedef std::multimap<std::string, std::vector<int>>::const_iterator iteratorr; //refer to https://stackoverflow.com/questions/29535125/access-key-from-values-and-value-from-key
        for (iteratorr it = _function_to_rangemr.begin(); it != _function_to_rangemr.end(); it++) {
            indices_buffer.push_back(it->second[1]);
        }
        std::sort(indices_buffer.begin(), indices_buffer.end());
        for(int sindex : indices_buffer) {
            for (iteratorr it = _function_to_rangemr.begin(); it != _function_to_rangemr.end(); it++) {
                if (it->second[1] == sindex) {
                    _function_ranges.push_back(std::make_tuple(it->second[1], it->second[0]));
                    break;
                } else
                    continue;
            }
        }
    }
}

void derivative::function_parser(){
    std::vector<std::tuple<int, int>> order_depth;
    std::tuple<int, int> pivot;
    fill_function_ranges();
    find_functions_inside();
    test1();
    test2();
    test3();
}

std::string derivative::differentiate_argument(int &SB, int &EB, bool is_Polynomial){
    std::string argument;
    std::string derivative;
    for(int i = SB; i < EB; i++){
        argument += _expressionarr[i];
    }
    return argument;
    //We create the argument object here,
    //argument argument = argument(argument, is_Polynomial);
    //derivative = argument.differentiate();
    //return derivative;
}

//Welcome to hell...
std::string derivative::differentiate() {
    typedef std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>>::const_iterator Iterator;
    std::vector<std::tuple<int, int>> functions_buffer;
    std::pair<std::tuple<int, int>, std::tuple<std::string, std::string>> scope_to_function_to_derivativep;
    std::map<std::tuple<int, int>, std::tuple<std::string, std::string>> scope_to_function_to_derivative;
    int SB;
    int EB;
    std::string dfx;
    std::string dgx;
    std::string function;
    std::string derivative;
    for(Iterator iter = _function_to_inside_functionm.begin(); iter != _function_to_inside_functionm.end(); iter++){
        if(iter->first == iter->second.back()){ //If the function only has polynomials inside
            functions_buffer.push_back(iter->first);
        }
        else
            continue;
    }
    for(auto &func : functions_buffer){
        SB = std::get<0>(func);
        EB = std::get<1>(func);
        function = give_function(SB);
        dfx = differentiate_function(function);
        dgx = differentiate_argument(SB, EB, true); //There is only one level where there are polynomials, so we don't need to write a routine that finds that out
        derivative = dfx + '*' + dgx;
        scope_to_function_to_derivativep.first = func;
        scope_to_function_to_derivativep.second = std::make_tuple(function, derivative);
        scope_to_function_to_derivative.insert(scope_to_function_to_derivativep);
    }
    return derivative;
}


derivative::derivative(std::string expression){
    _expression = expression;
}

derivative::~derivative(){
    
}

std::string derivative::differentiate_function(std::string function) {
    return std::string();
}

class Argument {
    
    
private:
    
    bool _repeated_values;
    bool _use_multimap;
    std::string _argument;
    std::vector<std::string> _argumentarr;
    std::vector<int> _numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'x', 'y', 'z', '^'};
    std::vector<int> _symbols = {'+', '-', '*', '/', '(', ')'};
    std::vector<std::string> _func = {"log", "ln", "sin", "cos", "tan", "sec", "cosec", "cotan", "e", "arcsin", "arcos", "arctan"};
    std::map<std::size_t, bool> _index_to_bracketsm;
    std::map<char,std::vector<int>> _symbols_classificationm;
    std::pair<int, std::string> _index_to_functionp; //Stores each function as a key with its corresponding index in the expression
    std::map<int, std::string> _index_to_functionm;
    std::multimap<int, std::string> _index_to_functionmr; //Same for repeated values
    std::pair<int, std::string> _index_to_resultp;
    std::map<int, std::string> _index_to_result;
    struct argument{ //TODO check gx inside differential and find out how to handle the chain rule for polynomials and functions altogether inside functions
        bool product_rule;
        bool quotient_rule;
        bool product_and_quotients_exist = false;
        bool product_exists = false;
        bool quotient_exists = false;
        bool polynommial_on_polynomial = false;
        bool polynomial_on_function = false;
        bool function_on_polynomial = false;
        bool function_on_function = false;
        bool addition_or_subtraction = false;
        bool equivalent_exponentiation;
        bool both_integers;
        bool non_exponentiated_monomials;
        bool double_subtraction = false;
        std::vector<std::tuple<int, int>> integers;
        std::deque<std::tuple<int, int>> indices_tuples;
        std::deque<std::tuple<std::string, std::string>> simple_arithmetic_operations;
        std::deque<std::tuple<std::string, std::string>> quotients_and_products;
        std::string expression_a;
        std::string expression_b;
        std::vector<int> add_sub_indices;
        std::vector<std::string> argument;
        std::string sign;
        std::vector<int> symbols; //Symbol indices
        std::pair<int, std::string> index_to_expressionp;
        std::map<int, std::string> index_to_expression;
        std::vector<int> parsed_argument;
    };
    argument argument;
    
public:
    
    void detect_functions();
    bool isNumber(char &number);
    bool isSymbol(std::string symbol);
    std::string differentiate();
    std::string differentiate_monomial(std::string &monomial);
    bool hasExponent(const std::string & str);
    std::string operate(char &sign);
    bool isFunction(std::string &pfunction);
    void parse_argument();
    void insert_index_to_function();
    std::string get_exponent(std::string &monomial, bool isExponent);
    void collect_exponents(std::deque<std::string> &exponents);
    void insert_index_to_expression(int &index, std::string &expression);
    bool product_exists();
    bool expression_is_function(char &expression);
    bool quotient_exists();
    char set_sign(int &count);
    void fill_simple_arithmetic();
    void fill_quotients_and_products();
    std::string differentiation();
    std::string perform_differentiation_helper(bool simple_arithmetic);
    std::string perform_differentiation();
    void reset_operation_kind();
    void assign_ab(std::deque<std::tuple<std::string, std::string>> &operations);
    std::vector<std::tuple<int, int>> gather_up_common_exponents(std::string &exponent_pivot);
    std::string set_subtractions_additions(std::vector<std::tuple<int, int>> &common_exponents);
    void set_operation(bool add_or_sub, std::string a_sign, std::string b_sign);
    std::string operate_integers();
    Argument(std::string &argument);
};

bool Argument::isNumber(char &number) {
    auto pos = std::find(_numbers.begin(), _numbers.end(), number);
    return pos != _numbers.end();
}

bool Argument::isSymbol(std::string symbol) {
    if(symbol.size() > 1){
        return false;
    }
    else if(isNumber(symbol[0])){
        return false;
    }
    else {
        return true;
    }
}

void Argument::insert_index_to_function(){
    for(int i = 0; i < _argumentarr.size(); i++){
        auto isfunction = std::find(_func.begin(), _func.end(), _argumentarr[i]);
        if(isfunction != _func.end()){
            _index_to_functionp.first = i;
            _index_to_functionp.second = _argumentarr[i];
            if(!_repeated_values)
                _index_to_functionm.insert(_index_to_functionp);
            else
                _index_to_functionmr.insert(_index_to_functionp);
        }
        else
            continue;
    }
}

void Argument::parse_argument() {
    std::string function;
    std::string monomial;
    bool catch_argument = false;
    std::string symbol;
    int index = 0;
    Parser parse = Parser(_argument);
    _argumentarr = parse.detect_functions();
    _use_multimap = true;
    insert_index_to_function();
    for(int i = 0; i < _argumentarr.size(); i++){
        //We prepare a map, mapping indices to expressions
        if(_argumentarr[i] == "(" or _argumentarr[i] == ")"){
            if(_argumentarr[i] == "(")
                catch_argument = true;
            else
                catch_argument = false;
        }
        else if(isFunction(_argumentarr[i])) {
            function = _argumentarr[i] + "(" + ")";
            std::cout<<"Function: "<<function<<std::endl;
        }
        else if(!isSymbol(_argumentarr[i]) and !isFunction(_argumentarr[i]) and catch_argument){
            std::string monomial_inside = _argumentarr[i];
            std::cout<<"Monomial inside: "<<monomial_inside<<std::endl;
            auto starting_brackets = function.find(")");
            function.insert(starting_brackets, monomial_inside); //We insert the argument
            insert_index_to_expression(index, function);
            index += 1;
        }
        else if(!isSymbol(_argumentarr[i]) and !isFunction(_argumentarr[i]) and !catch_argument) {
            monomial = _argumentarr[i];
            std::cout<<"Monomial: "<<monomial<<std::endl;
            insert_index_to_expression(index, monomial);
            index += 1;
        }
        else if(isSymbol(_argumentarr[i])){
            symbol = _argumentarr[i];
            std::cout<<"Symbol: "<<symbol<<std::endl;
            insert_index_to_expression(index, symbol);
            index += 1;
        }
    }
    typedef std::map<int, std::string>::const_iterator it;
    it iterator;
    for(iterator = argument.index_to_expression.begin(); iterator != argument.index_to_expression.end(); iterator++){
        argument.parsed_argument.push_back(iterator->first);
        argument.argument.push_back(iterator->second);
    }
}

void Argument::insert_index_to_expression(int &index, std::string &expression){
    argument.index_to_expressionp.first = index;
    argument.index_to_expressionp.second = expression;
    argument.index_to_expression.insert(argument.index_to_expressionp);
    argument.parsed_argument.push_back(index);
}

bool Argument::expression_is_function(char &expression){
    if(expression == 's' or expression == 't' or expression == 'a' or expression == 'c' or expression == 'l' or expression == 'e'){
        return true;
    } else return false;
}
//3x + 2x - [(4sin(3x) * 7x) * (5 * 8x^2) * (3x * 2sin(5x)) * (4x * 5x) * (4x * 5) * (7x * 9)
// 6x - [(4cos(3x) * 7 + 7x * 4sin(3x)) * (40x^2) * (6sin(5x) + 3x * 2cos(5x)) * (20x^2) * (20x) * (63x)] //Also having, n product rules, the resulting expression is going to be splitted into n subexpressions with additions
//If both are functions //Given a number of possible operations n, there will always be a certain number of possible product rules, and therefore possible expansions
//3x + 2x - [(4sin(3x) * 7x) * (3x * 2sin(5x)) * (4x * 5x) * (4x * 5) * (7x * 9)]


std::string Argument::operate(char &sign) {
    bool double_subtraction = false;
    algebraic_manipulation algebraic_operation = algebraic_manipulation(double_subtraction);
    if(sign == '*') {
        if(argument.function_on_function) {
            ;
        } else if(argument.function_on_polynomial) {
            ;
        } else if(argument.polynomial_on_function) {
            ;
        } else if(argument.polynommial_on_polynomial) {
            algebraic_operation.mulMonomials(argument.expression_a, argument.expression_b);
        }
    } else if(sign == '/') {
        if(argument.function_on_function) {
            ;
        } else if(argument.function_on_polynomial) {
            ;
        } else if(argument.polynomial_on_function) {
            ;
        } else if(argument.polynommial_on_polynomial) {
            ;//TODO: Write a method that computes the quotient of two monomials
        }
    }
}

void Argument::reset_operation_kind(){
    argument.polynommial_on_polynomial = argument.polynommial_on_polynomial ? !argument.polynommial_on_polynomial : argument.polynommial_on_polynomial;
    argument.polynomial_on_function = argument.polynomial_on_function ? !argument.polynomial_on_function : argument.polynomial_on_function;
    argument.function_on_polynomial = argument.function_on_polynomial ? !argument.function_on_polynomial : argument.polynomial_on_function;
    argument.function_on_function = argument.function_on_function ? !argument.function_on_function : argument.function_on_function;
}

std::string Argument::get_exponent(std::string &monomial, bool isInteger){
    typedef std::tuple<int, int> a_b_int;
    int count = 0;
    auto found_exponent = monomial.find('^');
    std::string exponent;
    if(found_exponent != std::string::npos){
        exponent = monomial.substr(found_exponent - 1, monomial.size());
    } else if(monomial.back() == 'x'){
        exponent = "x";
    } else if(monomial.back() == '+' or monomial.back() == '-' or monomial.back() == '*' or monomial.back() == '/'){
        exponent = "null";
    }else{
        exponent = "integer";
        isInteger = true;
    }
    return exponent;
}

void Argument::collect_exponents(std::deque<std::string> &exponents){
    std::string exponent;
    bool isInteger = false;
    for(int i = 0; i <= argument.simple_arithmetic_operations.size(); i++){
        exponent = get_exponent(std::get<0>(argument.simple_arithmetic_operations[i]), isInteger);
        exponents.push_back(exponent);
    }
    std::sort(exponents.begin(), exponents.end());
    exponents.erase(std::unique(exponents.begin(), exponents.end()), exponents.end());
    for(std::string exponent: exponents){
        std::cout<<exponent<<std::endl;
    }
}

void Argument::assign_ab(std::deque<std::tuple<std::string, std::string>> &operations){
    argument.expression_a = std::get<0>(operations.front());
    argument.expression_b = std::get<1>(operations.front());
    operations.pop_front();
}
//TODO: Get your shit done here
std::vector<std::tuple<int, int>> Argument::gather_up_common_exponents(std::string &exponent_pivot){
    bool isInteger = false;
    int count = 0;
    int integer_count = 0;
    int a_index;
    int b_index;
    int int_a_index;
    int int_b_index;
    std::string exponent;
    std::vector<std::tuple<int, int>> common_exponents;
    typedef std::tuple<std::string, std::string> a_and_b;
    typedef std::tuple<int, int> indices_pair;
    auto last = std::unique(argument.parsed_argument.begin(), argument.parsed_argument.end());
    argument.parsed_argument.erase(last, argument.parsed_argument.end());
    for(int index : argument.parsed_argument){
        isInteger = false;
        if(get_exponent(argument.index_to_expression[index], isInteger) == exponent_pivot){
            if(isInteger){
                if(integer_count == 0){
                    int_a_index = index;
                    integer_count += 1;
                }else if(integer_count == 1){
                    int_b_index = index;
                    argument.integers.push_back(std::make_tuple(int_a_index, int_b_index));
                    integer_count += 1;
                } else{
                    integer_count = 0;
                }
            }
            if(count == 0){
                a_index = index;
                count += 1;
            } else if(count == 1){
                b_index = index;
                common_exponents.push_back(std::make_tuple(a_index, b_index));
                count += 1;
            }
            else{
                count = 0;
            }
        }
        else
            continue;
    }
    return common_exponents;
}

void Argument::set_operation(bool add_or_sub, std::string a_sign, std::string b_sign){
    std::string temp;
    if(a_sign == "-" and b_sign == "+"){
        add_or_sub = false;
        temp = argument.expression_a;
        argument.expression_b = argument.expression_a;
        argument.expression_b = temp;
    } else if(a_sign == "-" and b_sign == "-"){
        add_or_sub = true;
        argument.double_subtraction = true;
    } else if(a_sign == "+" and b_sign == "+"){
        add_or_sub = true;
    } else if(a_sign == "+" and b_sign == "-"){
        add_or_sub = false;
    }
}

std::string Argument::set_subtractions_additions(std::vector<std::tuple<int, int>> &common_exponents){
    typedef std::tuple<int, int> ab;
    int count = 0;
    std::vector<int> already_processed_operations; //Stores indices of already processed monomials
    bool add_or_sub;
    std::string result;
    std::string a_sign;
    std::string b_sign;
    bool double_negative;
    for(ab a_b : common_exponents){
        count += 1;
        if(result.empty()){
            a_sign = argument.index_to_expression[std::get<0>(a_b) - 1];
            if(a_sign.empty())
                a_sign = "+";
            b_sign = argument.index_to_expression[std::get<1>(a_b) - 1];
            argument.expression_a = argument.index_to_expression[std::get<0>(a_b)];
            argument.expression_b = argument.index_to_expression[std::get<1>(a_b)];
            //std::cout<<"joder"<<std::endl;
            std::cout<<a_sign<<argument.expression_a<<std::endl;
            std::cout<<b_sign<<argument.expression_b<<std::endl;
            //We call the add or sub monomials function here
        } else {
            a_sign = argument.index_to_expression[std::get<0>(a_b) - 1];
            b_sign = argument.index_to_expression[std::get<1>(a_b) - 1];
            argument.expression_a = result;
            argument.expression_b = argument.index_to_expression[std::get<1>(a_b)];
            //We call the add or sub monomials function here
        }
        //if(count == common_exponents.size()){ TODO: Test this mess
        //;
    }
    common_exponents.clear();
    return result;
}

std::string Argument::operate_integers(){
    typedef std::tuple<int, int> ab;
    bool add_or_sub;
    std::string result;
    std::string a_sign;
    std::string b_sign;
    for(ab a_b : argument.integers){
        if(result.empty()){
            a_sign = argument.index_to_expression[std::get<0>(a_b) - 1];
            if(a_sign.empty()){
                a_sign = "+";
            }
            b_sign = argument.index_to_expression[std::get<1>(a_b) - 1];
            argument.expression_a = argument.index_to_expression[std::get<0>(a_b)];
            argument.expression_b = argument.index_to_expression[std::get<1>(a_b)];
        }
        else {
            a_sign = argument.index_to_expression[std::get<0>(a_b) - 1];
            b_sign = argument.index_to_expression[std::get<1>(a_b) - 1];
            argument.expression_a = result;
            argument.expression_b = argument.index_to_expression[std::get<1>(a_b)];
        }
    }
}

std::string Argument::perform_differentiation_helper(bool simple_arithmetic){ //TODO: Write a function that keeps track of indices so that the derivatives of all expressions will be returned in order
    std::string result;
    std::deque<std::string> exponents;
    std::vector<std::string> exponents_already_processed;
    std::vector<std::tuple<int, int>> common_exponents;
    std::string exponent_pivot;
    std::string simplification_result;
    std::string derivative;
    if(simple_arithmetic){
        collect_exponents(exponents);
        if(exponents.empty()){
            operate_integers();
        } else {
            for(std::string exponent : exponents){
                exponent_pivot = exponent;
                common_exponents = gather_up_common_exponents(exponent_pivot);
                set_subtractions_additions(common_exponents);               common_exponents.clear();
            }
            if(!exponents.empty()){
                operate_integers();
            }
        }
    } else{ //We perform quotient or product rule
        while(!argument.quotients_and_products.empty()) {
            reset_operation_kind();
            assign_ab(argument.quotients_and_products);
            if (!isNumber(argument.expression_a.back()) and !isNumber(argument.expression_b.back())) {
                std::cout << "Functions times function" << std::endl;
                std::cout << argument.expression_a << std::endl;
                std::cout << argument.expression_b << std::endl;
            } else if (isNumber(argument.expression_a.back()) and isNumber(argument.expression_b.back())) { //3x * 2x or 3 * 2
                //else we have an operation between two polynomials which always results in a single expression, note: it could also be two numbers !
                std::cout << "Operation between polynomials"
                << std::endl; //Add monomials, subtract monomials and mulMonomials here
                std::cout << argument.expression_a << std::endl;
                std::cout << argument.expression_b << std::endl;
            } else if (expression_is_function(argument.expression_a.back()) and expression_is_function(argument.expression_b.back())) {//sin(x) * cos(x)
                std::cout << "Functions operation" << std::endl;
                std::cout << argument.expression_a << std::endl;
                std::cout << argument.expression_b << std::endl;
            } else if (!isNumber(argument.expression_a.back()) and (isNumber(argument.expression_b.back()) and !expression_is_function(argument.expression_b.back()))) { //Function times polynomial
                std::cout << "Function times polynomial" << std::endl;
                std::cout << argument.expression_a << std::endl;
                std::cout << argument.expression_b << std::endl;
            } else if ((isNumber(argument.expression_a.back()) and !expression_is_function(argument.expression_a.back())) and !isNumber(argument.expression_b.back())) { //Polynomial times function
                std::cout << "Polynomial times function" << std::endl;
                std::cout << argument.expression_a << std::endl;
                std::cout << argument.expression_b << std::endl;
            }
        }
    }
    return derivative;
}

std::string Argument::differentiation() {
    std::string derivative;
    if (product_exists() and quotient_exists()) {
        argument.product_and_quotients_exist = true;
    }
    derivative = perform_differentiation();
    return derivative;
}

void Argument::fill_simple_arithmetic(){
    typedef std::tuple<int, int> index_a_b;
    int count = 0;
    for (int i = 0; i <= _argumentarr.size(); i++) {
        if (_argumentarr[i][0] == '+' or _argumentarr[i][0] == '-') {
            argument.indices_tuples.push_back(std::make_tuple(i - 1, i + 1));
            argument.simple_arithmetic_operations.push_back(std::make_tuple(_argumentarr[i - 1], _argumentarr[i + 1]));
        }
        continue;
    }
    for(index_a_b iab : argument.indices_tuples){
        count += 1;
        if(count == argument.indices_tuples.size()) {
            argument.add_sub_indices.push_back(std::get<0>(iab));
            argument.add_sub_indices.push_back(std::get<1>(iab));
        }
        else{
            argument.add_sub_indices.push_back(std::get<0>(iab));
        }
    }
}

void Argument::fill_quotients_and_products(){
    for (int i = 0; i < _argumentarr.size(); i++) {
        if (_argumentarr[i][0] == '*' or _argumentarr[i][0] == '/') {
            argument.quotients_and_products.push_back(std::make_tuple(_argumentarr[i - 1], _argumentarr[i + 1]));
        }
        continue;
    }
}

std::string Argument::perform_differentiation(){
    std::string derivative;
    std::string simple_arithmetic_derivative;
    if (argument.product_and_quotients_exist){
        fill_quotients_and_products();
        simple_arithmetic_derivative = perform_differentiation_helper(false);
        fill_simple_arithmetic();
        derivative = simple_arithmetic_derivative + perform_differentiation_helper(true);
    } else if (argument.product_exists) {
        fill_quotients_and_products();
        simple_arithmetic_derivative = perform_differentiation_helper(false);
        fill_simple_arithmetic();
        derivative = simple_arithmetic_derivative + perform_differentiation_helper(true);
    } else if (argument.quotient_exists){
        fill_quotients_and_products();
        simple_arithmetic_derivative = perform_differentiation_helper(false);
        fill_simple_arithmetic();
        derivative = simple_arithmetic_derivative + perform_differentiation_helper(true);
    } else{
        fill_simple_arithmetic();
        derivative = perform_differentiation_helper(true);
    }
    return derivative;
}

bool Argument::product_exists(){
    auto find_product = std::find(argument.argument.begin(), argument.argument.end(), "*");
    if (find_product != argument.argument.end()) {
        argument.product_exists = true;
        return true;
    } else {
        return false;
    }
}

bool Argument::quotient_exists(){
    auto find_quotient = std::find(argument.argument.begin(), argument.argument.end(), "/");
    if (find_quotient != argument.argument.end()) {
        argument.quotient_exists = true;
        return true;
    } else {
        return false;
    }
}

std::string Argument::differentiate() {
    std::string derivative;
    parse_argument(); //It stores an index with its corresponding expression in a map
    derivative = differentiation();
    return derivative;
}

std::string Argument::differentiate_monomial(std::string &monomial) {
    std::string exponent;
    std::string derivative;
    std::string coefficient;
    if (monomial == "x"){ //We handle the case where we have x
        derivative = "1";
        return derivative;
    }
    else{
        auto partition = monomial.find('^');
        if(partition != std::string::npos){
            exponent = monomial.substr(partition, monomial.size());
            monomial = monomial.substr(0, partition - 2);
        } else if(monomial.back() == 'x'){
            derivative = monomial.substr(0, monomial.size() - 1);
        }
    }
    if(exponent.front() == '^') //Handles the case where we have Nx^N
        if(monomial != "x"){
            if(exponent.front() == '2' and exponent.size() == 2) { //Nx^2
                derivative = std::to_string(std::stoi(exponent.substr(1, exponent.size())) * std::stoi(
                                                                                                       monomial.substr(0, monomial.size() - 1))); //We subtract one to get rid of x
                derivative += 'x';
            }
            else{ //Nx^N
                derivative = std::to_string(std::stoi(exponent.substr(1, exponent.size())) * std::stoi(monomial.substr(0, monomial.size() - 1))); //We subtract one to get rid of x
                exponent.erase(exponent.begin());
                derivative += "x^" + std::to_string(std::stoi(exponent) - 1);
            }
        }
        else{ //Else we have x^N or x^2
            if(exponent.front() == '2' and exponent.size() == 2){
                derivative = "2x";
            }
            else{
                coefficient = exponent.substr(1, exponent.size());
                exponent = std::to_string(std::stoi(exponent.substr(1, exponent.size())) - 1);
                derivative = derivative + '^' + exponent;
            }
        }
        else{ //We just have a coefficient N
            derivative = "0";
        }
    return derivative;
}


bool Argument::hasExponent(const std::string & str)
{
    const char c = '^';
    return str.find(c) != std::string::npos;
}


bool Argument::isFunction(std::string &pfunction) {
    auto isFunction = std::find(_func.begin(), _func.end(), pfunction);
    return isFunction != _func.end();
}

Argument::Argument(std::string &argument){
    _argument = argument;
}

void test_argument(){
    std::string expression = "3x + 5x^53 - 3x - 5x^53";
    Argument argument = Argument(expression);
    std::cout<<argument.differentiate()<<std::endl;
}

void autocalculus(){
    char input[100];
    while(true){
        std::cout <<"                                               <---Auto Calculus--->\n\n\n";
        std::cout <<"                                         Enter function to be differentiated\n                                 " << std::endl;
        std::cin.getline(input, sizeof(input));
        derivative function = derivative(input);
        function.find_scopes();
        function.function_parser();
        function.differentiate();
        break;
    }
}

int main(){
    
    test_argument();
    getchar();
    return 0;
}
