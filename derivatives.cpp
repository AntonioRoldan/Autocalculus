#include <iostream>
#include <map>
#include <array>
#include <vector>
#include <set>
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
    bool _repeated_values;
    std::vector<int> _numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'x', 'y', 'z', '^'};
    std::vector<int> _symbols = {'+', '-', '*', '/', '(', ')'};
    std::vector<std::string> _func = {"log", "ln", "sin", "cos", "tan", "sec", "cosec", "cotan", "e", "arcsin", "arcos", "arctan"};
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

void Parser::insert_index_to_function(){
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

void Parser::insert_index_to_expression(int &index, std::string &expression){
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
    for(int i = 0; i < _expressionarr.size(); i++){
        //We prepare a map, mapping indices to expressions
        if(_expressionarr[i] == "(" or _expressionarr[i] == ")"){
            catch_argument = _expressionarr[i] == "(";
        }
        else if(isFunction(_expressionarr[i])) {
            function = _expressionarr[i] + "(" + ")";
            std::cout<<"Function: "<<function<<std::endl;
        }
        else if(!isSymbol(_expressionarr[i][0]) and !isFunction(_expressionarr[i]) and catch_argument){
            std::string monomial_inside = _expressionarr[i];
            std::cout<<"Monomial inside: "<<monomial_inside<<std::endl;
            auto starting_brackets = function.find(")");
            function.insert(starting_brackets, monomial_inside); //We insert the argument
            insert_index_to_expression(index, function);
            index += 1;
        }
        else if(!isSymbol(_expressionarr[i][0]) and !isFunction(_expressionarr[i]) and !catch_argument) {
            monomial = _expressionarr[i];
            std::cout<<"Monomial: "<<monomial<<std::endl;
            insert_index_to_expression(index, monomial);
            index += 1;
        }
        else if(isSymbol(_expressionarr[i][0])){
            symbol = _expressionarr[i];
            std::cout<<"Symbol: "<<symbol<<std::endl;
            insert_index_to_expression(index, symbol);
            index += 1;
        }
    }
    typedef std::map<int, std::string>::const_iterator it;
    it iterator;
    for(iterator = _index_to_expression.begin(); iterator != _index_to_expression.end(); iterator++){
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
                        if (_expression[i] == '^' and _expression[i - 1] == ')') //If we have an exponent to an expression between brackets, we can interpret it as a function
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

class algebra {

private:

    std::vector<char> _numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
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
    bool isNumber(char &number);
    bool hasExponent(const std::string & str);
    void reset_binomial();
    algebra(bool &double_subtraction);
};

std::string algebra::subMonomials(std::string &a, std::string &b) {
    std::string result;
    binomial.addition_or_subtraction = false;
    result = arithmetic(a, b);
    if(std::stoi(result) < 0)
        result = "-" + result;
    else
        result = "+" + result;
    return result;
}

bool algebra::hasExponent(const std::string & str)
{
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

bool algebra::isNumber(char &number) {
    auto pos = std::find(_numbers.begin(), _numbers.end(), number);
    return pos != _numbers.end();
}

std::string algebra::addMonomials(std::string &a, std::string &b){
    std::string result;
    binomial.addition_or_subtraction = true;
    result = arithmetic(a, b);
    if(binomial.double_subtraction)
        result = "-" + result;
    return result;
}

int algebra::perform_arithmetic(int a_integer, int b_integer){
    int result_integer;
    result_integer = binomial.addition_or_subtraction ? a_integer + b_integer : a_integer - b_integer;
    return result_integer;
}

void algebra::reset_binomial(){
    binomial.equivalent_exponentiation = false;
    binomial.non_exponentiated_monomials = false;
    binomial.both_integers = false;
}

void algebra::check_arithmetic_equivalency(std::string &a, std::string &b){
    reset_binomial();
    std::string a_exponent;
    std::string b_exponent;
    if(hasExponent(a) and hasExponent(b)) { //If they both have exponents
        auto pos = a.find('^');
        auto posb = b.find('^');
        a_exponent.assign(a.begin() + pos + 1, a.begin() + a.size());
        b_exponent.assign(b.begin() + posb + 1, b.begin() + b.size());
        binomial.equivalent_exponentiation = a_exponent != b_exponent ? true : false;
        return;
    }
        //if equivalent exponentiation is set to false the operation will stop
    else if((hasExponent(a) and !hasExponent(b)) or (!hasExponent(a) and hasExponent(b))) {
        binomial.equivalent_exponentiation = false;
    }
    else if(a.front() == 'x' and a.front() == b.front()){
        binomial.non_exponentiated_monomials = true; //Non exponentiated but still with an x
    }
    else if(a.front() != 'x' and b.front() != 'x'){
        binomial.both_integers = true;
    }
}

std::string algebra::add_sub(std::string &a, std::string &b){
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

std::string algebra::arithmetic(std::string &a, std::string &b) {
    std::string result;
    check_arithmetic_equivalency(a, b);
    result = add_sub(a, b);
    return result;
}

algebra::algebra(bool &double_subtraction) {
    binomial.double_subtraction = double_subtraction;
}

class simplifier{


private:

    std::string expression;
    std::vector<std::string> _expressionarr;
    std::string _expression;
    std::vector<int> _numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    std::vector<std::string> _func = {"sin", "cos", "tan", "log", "ln", "sec", "cosec", "cotan", "arcos", "arcsin", "arctan"};
    std::vector<char> _symbols = {'+', '-', '/', '*', '(', ')'};
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

simplifier::simplifier(std::string &expression_argument) {
    _expression = expression_argument;
}

bool simplifier::isNumber(char &number) {
    auto pos = std::find(_numbers.begin(), _numbers.end(), number);
    return pos != _numbers.end();
}

bool simplifier::quotient_exists(){
    auto find_quotient = std::find(polynomial.polynomial.begin(), polynomial.polynomial.end(), "/");
    if (find_quotient != polynomial.polynomial.end()) {
        polynomial.quotient_exists = true;
        return true;
    } else {
        return false;
    }
}

bool simplifier::isFunction(std::string &pfunction) {
    auto isFunction = std::find(_func.begin(), _func.end(), pfunction);
    return isFunction != _func.end();
}

bool simplifier::product_exists(){
    auto find_product = std::find(polynomial.polynomial.begin(), polynomial.polynomial.end(), "*");
    if (find_product != polynomial.polynomial.end()) {
        polynomial.product_exists = true;
        return true;
    } else {
        return false;
    }
}

void simplifier::set_quotients_and_products(std::vector<std::string> &expressionarr){
    ;
}

void simplifier::set_simple_arithmetic(){
    ;
}

std::string simplifier::perform_simplification(){
    std::string simplification;
    std::string simple_arithmetic_simplification;
    std::string quotient_products_simplification;
    if (polynomial.product_and_quotients_exist){
        //If there are any product or quotient rules, they will be deleted from the map and added to a secondary map for their processing later
        ;//First we simplify products
        //Secondly we simplify quotients Note: we need the algebra class to work properly for this
    } else if (polynomial.product_exists) {
        ;
    } else if (polynomial.quotient_exists){
       ;
    } else{
       ;
    }
    return simplification;
}

bool simplifier::isSymbol(char &symbol) {
    auto pos = std::find(_symbols.begin(), _symbols.end(), symbol);
    return pos != _symbols.end();
}

std::string simplifier::get_exponent(std::string &monomial){
    typedef std::tuple<int, int> a_b_int;
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
    }
    return exponent;
}

void simplifier::collect_exponents(std::deque<std::string> &exponents){ //Breakpoint
    ;
}

std::vector<std::tuple<int, int>> simplifier::gather_up_common_exponents(std::string &exponent_pivot){
    bool isInteger;
    int count = 0;
    int a_index;
    int b_index;
    int int_a_index;
    int int_b_index;
    std::string exponent;
    std::vector<std::tuple<int, int>> common_exponents;
    typedef std::tuple<std::string, std::string> a_and_b;
    typedef std::tuple<int, int> indices_pair;
    auto last = std::unique(polynomial.parsed_argument.begin(), polynomial.parsed_argument.end());
    polynomial.parsed_argument.erase(last, polynomial.parsed_argument.end());
    for(int index : polynomial.parsed_argument) {
        if (get_exponent(index_to_expression[index]) == exponent_pivot) {
            if (count == 0) {
                a_index = index;
                count += 1;
            } else if (count == 1) {
                b_index = index;
                common_exponents.push_back(std::make_tuple(a_index, b_index));
                count += 1;
            } else {
                count = 0;
            }
        } else
            continue;
    }
    return common_exponents;
}

void simplifier::parse_argument() { //TODO Test the argument parsing
    std::tuple<std::vector<int>, std::map<int, std::string>, std::vector<std::string>, std::map<int, std::string>> tokens;
    Parser parser = Parser(_expression);
    _expressionarr = parser.detect_functions();
    tokens = parser.parse();
    polynomial.parsed_argument = std::get<0>(tokens);
    index_to_expression = std::get<1>(tokens);
    polynomial.poly = std::get<2>(tokens);
    _index_to_functionm = std::get<3>(tokens);
}

std::string simplifier::perform_polynomial_simplification_helper(bool simple_arithmetic){
    ;
}

std::string simplifier::perform_mixed_simplification_helper(bool simple_arithmetic){
    ;
}

std::string simplifier::simplification(){
    std::string simplification;
    if(quotient_exists() and product_exists())
        polynomial.quotient_and_product_exist = true;
    simplification = perform_simplification();
    return simplification;
}

std::string simplifier::simplify_expression(){
    parse_argument();
    return simplification();
}

class Argument { //It is from here that we perform simplification and differentiation operations


private:
    bool _isPolynomial;
    std::string _argument;
    struct argument{
        bool product_and_quotients_exist = false;
        bool product_exists = false;
        bool quotient_exists = false;
        std::vector<std::string> argument;
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

void Argument::parse_argument() {
    std::tuple<std::vector<int>, std::map<int, std::string>, std::vector<std::string>, std::map<int, std::string>> tokens;
    Parser parse = Parser(_argument);
    tokens = parse.parse();
    //TODO:
}

//3x + 2x - [(4sin(3x) * 7x) * (5 * 8x^2) * (3x * 2sin(5x)) * (4x * 5x) * (4x * 5) * (7x * 9)
// 6x - [(4cos(3x) * 7 + 7x * 4sin(3x)) * (40x^2) * (6sin(5x) + 3x * 2cos(5x)) * (20x^2) * (20x) * (63x)] //Also having, n product rules, the resulting expression is going to be splitted into n subexpressions with additions
//If both are functions //Given a number of possible operations n, there will always be a certain number of possible product rules, and therefore possible expansions
//3x + 2x - [(4sin(3x) * 7x) * (3x * 2sin(5x)) * (4x * 5x) * (4x * 5) * (7x * 9)]

std::string Argument::perform_polynomial_differentiation_helper(){
    ;
}

std::string Argument::differentiation() {
    std::string derivative;
    if (product_exists() and quotient_exists()) {
        argument.product_and_quotients_exist = true;
    }
    derivative = perform_differentiation();
    return derivative;
}

std::string Argument::perform_differentiation(){
    std::string derivative;
    std::string simple_arithmetic_derivative;
    std::string quotient_products_derivative;
    if (argument.product_and_quotients_exist){ //We start by simplifying the expressions
    } else if (argument.product_exists) {
    } else if (argument.quotient_exists){
    } else{
        ;
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

Argument::Argument(std::string &argument, std::deque<std::string> &derivatives_inside){
    _isPolynomial = derivatives_inside.empty() ? true : false;
    _argument = argument;
}

class derivative{
    ;
};

class expression{


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
    std::pair<std::tuple<int, int>, std::vector<std::tuple<int, int>> _function_to_inside_functionp; //Keys: function's range Values: ranges of the functions inside it
    std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>> _function_to_inside_functionm;
    std::pair<std::tuple<int, int>, std::tuple<int , int>> _SE_to_ODp;
    std::map<std::tuple<int, int>, std::tuple<int, int>> _SE_to_OD;
    std::vector<std::tuple<int, int>> _sorted_SB_EB;
    bool _repeated_values = false;

public:

    bool isNumber(char &number);
    bool isSymbol(char &symbol);
    void subvector_to_string(std::tuple<int, int> &indices, std::string &expression); //TODO: Test this mess
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

bool expression::isNumber(char &number) {
    auto pos = std::find(_numbers.begin(), _numbers.end(), number);
    return pos != _numbers.end();
}

bool expression::isSymbol(char &symbol) {
    auto pos = std::find(_symbols.begin(), _symbols.end(), symbol);
    return pos != _symbols.end();
}

void expression::insert_index_to_function(){
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

bool expression::initialise_classification(char symbol, int &index) {
    std::vector<int> symbols;
    auto is_stored = std::find(_symbols_stored.begin(), _symbols_stored.end(), symbol);
    if(is_stored != _symbols_stored.end())
        return false;
    else if(symbols.empty() or is_stored == _symbols_stored.end()){
        _symbols_stored.push_back(symbol);
        symbols.push_back(index);
        _symbols_classificationp.first = symbol; //Symbol
        _symbols_classificationp.second = symbols; //Symbols indices
        _symbols_classificationm.insert(_symbols_classificationp);
        return true;
    }
    return false;
}

void expression::set_index_to_symbols(char &symbol, int &index){
    _index_to_symbolsp.first = index;
    _index_to_symbolsp.second = symbol;
    _index_to_symbolsm.insert(_index_to_symbolsp);
}

void expression::insert_index_to_symbols() {
    for(int i = 0; i < _expressionarr.size(); i++){
        auto issymbol = std::find(_symbols.begin(), _symbols.end(), _expressionarr[i][0]);
        if(issymbol != _symbols.end()){
            insert_symbol(_expressionarr[i][0], i); //We index the string so as tu turn it into a character
        }
    }
}

void expression::insert_symbol(char &symbol, int &index) {
    set_index_to_symbols(symbol, index);
    switch(symbol){

        case '+' : if(initialise_classification('+', index)); else _symbols_classificationm['+'].push_back(index); break;

        case '-' : if(initialise_classification('-', index)); else _symbols_classificationm['-'].push_back(index); break;

        case '*' : if(initialise_classification('*', index)); else _symbols_classificationm['*'].push_back(index); break;

        case '/' : if(initialise_classification('/', index)); else _symbols_classificationm['/'].push_back(index); break;

        default : ;

    }
}

std::string expression::give_function(int &index){
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

std::vector<int> expression::brackets_bag(){
    //It collects the positions of starting brackets in order of appereance
    std::vector<int> brackets_positions;
    std::vector<std::vector<std::size_t>> positions;
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

void expression::test3(){
    typedef std::multimap<std::string, std::vector<int>>::const_iterator MapIterator;
    for(MapIterator iter = _function_to_rangemr.begin(); iter != _function_to_rangemr.end(); iter++){
        std::cout << "Function: " << iter->first << "\nArguments:" << std::endl;
        typedef std::vector<int>::const_iterator VectorIterator;
        for(VectorIterator vect_iter = iter ->second.begin(); vect_iter != iter -> second.end(); vect_iter++){
            std::cout << " " << *vect_iter << std::endl;
        }
    }
}

void expression::test2(){
    typedef std::multimap<std::tuple<int, int>, std::tuple<int, int>>::const_iterator Iterator;
    for(Iterator iter = _SE_to_OD.begin(); iter != _SE_to_OD.end(); iter++){
        std::cout<<"SE: "<< std::get<0>(iter->first) << "\t" << std::get<1>(iter->first) << "\nOD: " << std::get<0>(iter->second) << "\t" << std::get<1>(iter->second) << std::endl;
    }
}

void expression::test1(){
    typedef std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>>::const_iterator Iterator;
    for(Iterator iter = _function_to_inside_functionm.begin(); iter != _function_to_inside_functionm.end(); iter++){
        std::cout<<std::get<0>(iter->first) << "\t" << std::get<1>(iter->first)<<"\n" << "Inside: ";
        for(const auto &i : iter->second){
            std::cout<<std::get<0>(i) << "\t" << std::get<1>(i) << "\n" << std::endl;
        }
    }
}

void expression::find_scopes(){
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

void expression::insert_SE_OD(std::tuple<int, int> &SE, std::tuple<int, int> OD){
    _SE_to_ODp.first = SE;
    _SE_to_ODp.second = OD;
    _SE_to_OD.insert(_SE_to_ODp);
}

void expression::fill_function_to_inside(std::tuple<int, int> &pivot){
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

void expression::find_functions_inside() { //Refactoring needed
    bool inside;
    int depth;
    int order;
    std::vector<std::tuple<int, int>> order_depth;
    std::tuple<int, int> pivot;
    while(!_function_ranges.empty()){
        depth = 0;
        order = 0;
        pivot = _function_ranges.front();
        _sorted_SB_EB.push_back(pivot);
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

void expression::fill_function_ranges() {
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
        typedef std::multimap<std::string, std::vector<int>>::const_iterator iteratorr;
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

void expression::function_parser(){
    std::vector<std::tuple<int, int>> order_depth;
    std::tuple<int, int> pivot;
    fill_function_ranges();
    find_functions_inside();
    test1();
    test2();
    test3();
}

void expression::sort_tuples_vector(std::vector<std::tuple<int, int>> &tuples_vector){//////////////FIRST TEST
    std::sort(std::begin(tuples_vector), std::end(tuples_vector), [](std::tuple<int, int> const &t1, std::tuple<int, int> const &t2) {
        return std::get<0>(t1) < std::get<0>(t2);
    });
}

void expression::sort_tuples_queue(std::deque<std::tuple<std::tuple<int, int>, std::tuple<int, int>>> &tuples_queue){///////////////SECOND TEST
    std::sort(std::begin(tuples_queue), std::end(tuples_queue), [](std::tuple<std::tuple<int, int>, std::tuple<int, int>> const &t1) {
        return std::get<1>(std::get<0>(t1)) < std::get<1>(std::get<1>(t1));
    });
}

void expression::differentiate_level_by_level(std::map<std::tuple<int, int>, bool> &differentiated_functions, std::map<std::tuple<int, int>, std::tuple<std::string, std::string>> &scope_to_expression_to_derivative, std::deque<std::string> &polynomial_derivatives){
    /* As if we were opening a russian doll from the inside, that's how this function is processing each function argument, from the inner (which always contains a polynomial) to the outer-most degrees of depth*/
    typedef std::tuple<std::string, std::string> argument_to_derivative;
    typedef std::tuple<std::tuple<int, int>, std::tuple<int, int>> SE_OD; //Starting and ending brackets' indices
    typedef std::string sharp_standard;
    std::map<sharp_standard, argument_to_derivative> sharp_parsing; //
    std::deque<SE_OD> orders_of_depth; //How deep the functions are
    std::deque<SE_OD> current_level; //Functions which are currently to be processed
    std::deque<std::string> previous_derivatives; //We fill it with derivatives to functions
    int sharp_index;
    sharp_standard sharpStandard;
    std::string derivative_inside;
    std::string derivative_outside;
    std::string previous_derivative;
    std::string expression;
    SE_OD cur;
    std::tuple<int, int> to_be_deleted; //This container will store
    sort_tuples_vector(_sorted_SB_EB); ////////Test 1
    int deepest_level;
    while(!_sorted_SB_EB.empty()){ //While there is some function we haven't processed yet
        sharp_index = 1;
        for(std::tuple<int, int> SB_EB : _sorted_SB_EB){ //First we find out what arguments are lying at the deepest not-yet-processed layers
                orders_of_depth.push_back(std::make_tuple(SB_EB, _SE_to_OD[SB_EB]));
        }
        sort_tuples_queue(orders_of_depth);  //They are always being stored in a left to right(increasing) order //TEST 1 are elements sorted?
        deepest_level = std::get<0>(std::get<1>(orders_of_depth.front())); //The deepest level is always the last element in the queue, since we have sorted the queue in increasing order
        ///////Test 2 is it really the deepest level? it will be if the queue is properly sorted
        orders_of_depth.pop_front();
        while(std::get<0>(std::get<1>(orders_of_depth.front())) == deepest_level){ //While the functions being extracted from the deque are at the same level of depth (we check the first elemement of the second tuple for that)
            //We fill up current level, it will contain all functions that can be found at the same degree of depth
            cur = std::get<0>(orders_of_depth.front()); //We pick up the starting and ending brackets of a function at the current level
            current_level.push_back(cur); //We push them into the queue containing all functions found at the same level of depth
            orders_of_depth.pop_front();
            to_be_deleted = std::get<0>(cur); //Since we are going to process this function already before the loop reaches it, we can delete it
            _sorted_SB_EB.erase(std::remove(_sorted_SB_EB.begin(), _sorted_SB_EB.end(), to_be_deleted), _sorted_SB_EB.end()); //We get rid of the already differentiated function
        }////////////////////Test 3
        //We revert the current level
        for(SE_OD function : current_level){ //We are going from right to left in order to pick up the previous polynomials as we go
            if(std::get<0>(function) == 0){ //When we have the outer most function
                Argument argument = Argument(expression, previous_derivatives);
                derivative_inside = argument.differentiate();
                //Derivative_outside
                //return Derivative_inside * derivative outside
            } else{
                subvector_to_string(std::get<0>(function), expression); //We set up the expression to be differentiated
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
                if(derivative_inside != 1){////////Test 5 it should get rid of the one in this case
                    sharp_index += 1;
                    sharpStandard = "#" + std::to_string(sharp_index);
                    previous_derivative = derivative_inside + "*" + derivative_outside;
                    previous_derivatives.push_back(previous_derivative);
                } else{
                    sharp_index += 1;
                    sharpStandard = "#" + std::to_string(sharp_index);
                    previous_derivatives.push_back(derivative_outside);//TODO:Instead of a queue it should be a map (sharp to argument(the expression variable in this case) and its derivative
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
        //TODO:Write on paper the algorithm steps
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
    if(!_function_to_inside_functionm.empty()){ //If there are any trigonometric functions
       differentiate_level_by_level(differentiated_functions, scope_to_expression_to_derivative);
    } else { //We just differentiate a polynomial
        ;
    }
    return derivative;
}

void expression::subvector_to_string(std::tuple<int, int> &indices, std::string &expression){
    //Assigns string elements from a vector in a given indices range to a string variable
    for(int i = std::get<0>(indices); i <= std::get<1>(indices); i++){ //////////////////////////FOURTH TEST
        expression += _expressionarr[i];
    }
}

expression::expression(std::string expression){
    _expression = expression;
}

expression::~expression(){

}


void test_simplification(){
    std::string expression = "3x + 5x^53 - 3x - 5x^53";
    simplifier simplified = simplifier(expression);
    simplified.simplify_expression();
}

void 

void autocalculus(){
    char input[100];
    while(true){
        std::cout <<"                                               <---Auto Calculus--->\n\n\n";
        std::cout <<"                                         Enter function to be differentiated\n                                 " << std::endl;
        std::cin.getline(input, sizeof(input));
        expression function = expression(input);
        function.find_scopes();
        function.function_parser();
        //function.differentiate();
        break;
    }
}

int main(){
    autocalculus();
    getchar();
    return 0;
}
