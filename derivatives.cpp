#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <array>
#include <vector>
#include <set>
#include <algorithm>
#include <boost/algorithm/string/find.hpp>
#include <tuple>
#include <cctype>
#include <math.h>
#include<cmath>
#include <numeric>
#include<deque>
#include <stack>
#include <sstream>

class derivative{


    private:

        std::string _expression;
        std::deque<std::tuple<int, int>> _function_ranges;
        std::vector<std::string> _expressionarr;
        std::vector<char> _symbols_stored;
        bool _use_multimap = false;
        std::vector<std::string> _functions;
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
        bool _negative_operation = false;
        struct Monomials{ //It stores a pair of monomials to perform the product on quotient rule on them and their subsequent arithmetic operations
            bool _subtract_monomials = false;
            bool negative_operation = false;
            std::string a;
            std::string b;
            std::string da;
            std::string db;
            std::string monomial_a;
            std::string monomial_b;
            std::string mderivative;
            char a_sign;
            char b_sign;
        };
        struct Polynomial{ //TODO check gx inside differential and find out how to handle the chain rule for polynomials and functions altogether inside functions
            bool product_rule = false;
            bool quotient_rule = false;
            bool concatenated = false;
            std::vector<int> symbols; //Symbol indices
            std::pair<int, std::tuple<std::string, std::string>> index_to_expressionp;
            std::map<int, std::tuple<std::string, std::string>> index_to_expression;
            std::string polynomial;
            std::vector<int> parsed_polynomial;
            std::string derivative_buffer;
            Monomials monomials;
        };
        Polynomial polynomial;

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
        void set_monomials(int &index, bool concatenated);
        int perform_arithmetic(int a, int b, bool addition);
        std::string give_function(int &index);
        std::vector<int> brackets_bag();
        std::string product_rule(int &index, bool concatenated);
        std::string quotient_rule(int &index, bool concatenated);
        void find_scopes();
        void find_functions_inside();
        void fill_function_to_inside(std::tuple<int, int> &pivot);
        char parse_signs(char a_sign, char b_sign);
        std::string differentiate_monomial(std::string &monomial, char &sign);
        std::string differentiate_polynomial(int &sindex, int &eindex);
        std::string differentiate();
        std::string differentiate_function(std::string function);
        std::string arithmetic(std::string &a, std::string &b, char &sign);
        std::string mulMonomials(std::string &a, std::string &b);
        bool hasExponent(const std::string & str);
        bool isFunction(std::string &pfunction);
        void function_parser();
        void test1();
        void test2();
        void test3();
        bool isProduct(int &index);
        bool isQuotient(int &index);
        void isRule(int &index);
        bool isConcatenated();
        void reset_polynomial(); //It resets the values of a polynomial object to false
        derivative(std::string expression); //Copy constructor
        derivative(derivative&&) = default; //Move constructor
        derivative& operator = (const derivative&) & = default; //Move constructor
        derivative& operator = (derivative&&) & = default; //Copy assignment operator
        ~derivative(); //Destructor
};

void derivative::detect_functions(){ //Refactoring needed
    int log = 0, ln = 0, sin = 0, cos = 0, tan = 0, sec = 0, cosec = 0, cotan = 0, arcsin = 0, arcos = 0, arctan = 0, e = 0;
    std::string exponent;
    bool catch_exponent = false;
    bool polynomial_raised = false;
    bool integer_raised = false;
    bool done = false;
    bool previous_number;
    std::string number;
    std::string function;
    while(!done){
        for(int i = 0; i < _expression.size(); i++){

            switch(_expression[i]){

                case 'l' :  if(_expression[i + 1] == 'n'){
                        function = "ln";
                        _expressionarr.push_back(function);
                        ln++;
                    }
                    else if(_expression[i + 1] == 'o'){
                        function = "log";
                        _expressionarr.push_back(function);
                        log++;
                    }
                    break;

                case 's' : if(_expression[i + 1] == 'i'){
                        if(_expression[i - 3] == 'a')
                            ;
                        else {
                            function = "sin";
                            _expressionarr.push_back(function);
                            sin++;
                        }
                    }
                    else if(_expression[i - 2] == 'c'){
                        if(_expression[i + 1] == 'e'){
                            ;
                        }
                        else if(_expression[i - 1] == 'c'){
                            function = "arcsin";
                            _expressionarr.push_back(function);
                            arcsin++;
                        }
                        else{
                            function = "cos";
                            _expressionarr.push_back(function);
                            cos++;
                        }
                    }
                    else if(_expression[i + 1] == 'e'){
                        function = "sec";
                        _expressionarr.push_back(function);
                        sec++;
                    }
                    break;

                case 'c' :  if(_expression[i + 1] == 'o'){
                        if(_expression[i - 2] == 'a')
                            ;
                        else if(_expression[i + 3] == 'e'){
                            function = "cosec";
                            _expressionarr.push_back(function);
                            cosec++;
                        }
                        else if(_expression[i + 2] == 's'){
                            function = "cos";
                            _expressionarr.push_back(function);
                            cos++;
                        }
                    }
                    else if(_expression[i + 1] == 's') //If it is arcsin we will cause a redundance
                        ;
                    else if(_expression[i - 1] == 'e')
                        ;
                    else
                        ;
                    break;

                case 'a': switch(_expression[i+3]){

                        case 'o':  function = "arcos";  _expressionarr.push_back(function); arcos++;
                            break;

                        case 's':   function = "arcsin"; _expressionarr.push_back(function); arcsin++;
                            break;


                        case 't':   function = "arctan";  _expressionarr.push_back(function); arctan++;
                            break;
                    }
                    break;

                case 't':   if(_expression[i - 3] == 'a')
                                ;
                            else if(_expression[i - 2] == 'c'){
                                function = "cotan";
                                _expressionarr.push_back(function);
                                cotan++;
                                break;
                            }
                            else{
                                function = "tan";
                                _expressionarr.push_back(function);
                                tan++;
                                break;
                            }
                            break;

                case 'e':   if(_expression[i - 1] == 's')
                                ;
                            else if(_expression[i - 3] == 'c')
                                ;
                            else{
                                function = "e";
                                _expressionarr.push_back(function);
                                e++;
                                break;
                            }
                            break;
                default:
                    if(isspace(_expression[i]))
                        ;
                    else if(isNumber(_expression[i]) and !previous_number) {
                        if(!catch_exponent){
                            number += _expression[i];
                        }
                        else{
                            exponent += _expression[i];
                        }
                        previous_number = true;
                    }
                    else if(isNumber(_expression[i]) and previous_number){
                        if(!catch_exponent)
                            number += _expression[i];
                        else
                            exponent += _expression[i];
                    }
                    else if(!isNumber(_expression[i]) and previous_number){
                        if(_expression[i] == '^') {
                            if (_expression[i - 1] == 'x' or
                                _expression[i - 1] == 'y') {//If it is a polynomial raised to some power
                                number += _expression[i]; //Number will be added its exponent symbol
                                catch_exponent = true;
                                polynomial_raised = true;
                            }
                            else{
                                catch_exponent = true;
                                integer_raised = true;
                            }
                        }
                        else if(catch_exponent and integer_raised) { //TODO Get the program to catch exponents of integers and monomials correctly testing phase
                            number = std::to_string(std::pow(std::stoi(number), std::stoi(exponent)));
                            _expressionarr.push_back(number);
                            exponent.clear();
                            integer_raised = false;
                        }
                        else if(catch_exponent and polynomial_raised){
                            number += exponent; //Number will be added its exponent so it will later be processed by differentiate_monomial
                            _expressionarr.push_back(number);
                            exponent.clear();
                            polynomial_raised = false;
                        }
                        else{
                            previous_number = false;
                            _expressionarr.push_back(number);
                            number.clear();
                        }
                    }
                    if(isSymbol(_expression[i])){ //TODO Get the program to process exponents to expressions between brackets as functions
                        if(_expression[i] == '^' and _expression[i - 1] == ')') //If we have an exponent to an expression between brackets, we can interpret it as a function
                            ; //with its respective derivative
                        else
                            _expressionarr.push_back(std::string(1, _expression[i])); //We convert char into a string of length one
                    }
            }
        }
        if(ln >= 2 or log >= 2 or sin >= 2 or cos >= 2 or tan >= 2 or sec >= 2 or cosec >= 2 or cotan >= 2 or arcsin >= 2 or arcos >= 2 or arctan >= 2 or e >= 2){
            _repeated_values = true;
        }
        if(!_repeated_values)
            done = true;
        else if(_use_multimap)
            done = true;
        else{
            _use_multimap = true; //We make do another set of iterations
            _expressionarr.clear();
            _index_to_bracketsm.clear();
            _symbols_classificationm.clear();
        }
    }
    insert_index_to_function();
    insert_index_to_symbols();
}



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
    detect_functions();
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

void derivative::set_monomials(int &index, bool concatenated) {
    Monomials monomials;
    if(concatenated){
        polynomial.monomials.a = polynomial.derivative_buffer;
        polynomial.monomials.a_sign = polynomial.monomials.a.back();
        polynomial.monomials.a.erase(polynomial.monomials.a.back());
        polynomial.monomials.da = differentiate_monomial(polynomial.monomials.a, polynomial.monomials.a_sign);
    } else{
        polynomial.monomials.a = std::get<0>(polynomial.index_to_expression[index - 1]);
        polynomial.monomials.da = std::get<1>(polynomial.index_to_expression[index - 1]);
        polynomial.monomials.a_sign = polynomial.monomials.da.back();
        polynomial.monomials.da.erase(polynomial.monomials.a.back());
    }
    polynomial.monomials.b = std::get<0>(polynomial.index_to_expression[index + 1]);
    polynomial.monomials.db = std::get<1>(polynomial.index_to_expression[index + 1]);
    polynomial.monomials.b_sign = polynomial.monomials.db.back();
    polynomial.monomials.db.erase(polynomial.monomials.db.back());
}

void derivative::insert_SE_OD(std::tuple<int, int> &SE, std::tuple<int, int> OD){
    _SE_to_ODp.first = SE;
    _SE_to_ODp.second = OD;
    _SE_to_OD.insert(_SE_to_ODp);
}

std::string derivative::differentiate_monomial(std::string &monomial, char &sign){ //TODO Test phase Get the function to process monomials raised to some power
    std::string exponent;
    std::string derivative;
    std::string coefficient;
    int partition;
    if(monomial == "x") //We handle the case where we have x
        derivative = sign + 'x';
        return derivative;
    for(int i = 0; i < monomial.size(); i++) {
        if (monomial[i] == '^') {
            partition = i;
            exponent = monomial.substr(partition, monomial.size());
            monomial = monomial.substr(0, partition - 1);
            break;
        }
        continue;
    }
    if(exponent.back() == '^') //Handles the case where we have Nx^N
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
    else //If exponent.back is not equal to '^' we simply have no exponent and therefore will have this case Nx
        derivative = monomial.substr(0, monomial.size() - 1);
    return sign + derivative;
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


bool derivative::isProduct(int &index) {
    auto isProduct = std::find(_symbols_classificationm['*'].begin(), _symbols_classificationm['*'].end(), index);
    return isProduct != _symbols_classificationm['*'].end();
}

bool derivative::isQuotient(int &index) {
    auto isQuotient = std::find(_symbols_classificationm['/'].begin(), _symbols_classificationm['/'].end(), index);
    return isQuotient != _symbols_classificationm['/'].end();
}

void derivative::isRule(int &index) {
    auto pos = std::find(polynomial.symbols.begin(), polynomial.symbols.end(), index);
    auto it = polynomial.symbols.begin() + *pos;
    auto next = std::next(it, 1);
    if (isProduct(polynomial.symbols[*pos]) and (_expressionarr[*it] == _expressionarr[*next])) { //If there are two adjacent products
        polynomial.concatenated = true;
        polynomial.product_rule = true;
    } else if (isQuotient(polynomial.symbols[*pos]) and (_expressionarr[*it] == _expressionarr[*next])) {
        polynomial.concatenated = true;
        polynomial.quotient_rule = true;
    } else if(isProduct(index))
        polynomial.product_rule = true;
    else if(isQuotient(index))
        polynomial.quotient_rule = true;
}

bool derivative::isConcatenated() {
    if(polynomial.product_rule and polynomial.concatenated){
        return true;
    }
    else return polynomial.product_rule and polynomial.concatenated;
}

char derivative::parse_signs(char a_sign, char b_sign){
    if(a_sign != b_sign) //If - + or + -
        return '-';
    else if(a_sign == '-') //or b_sign for that matter
        return '+';
    else
        return '+';
}

std::string derivative::product_rule(int &index, bool concatenated) { //TODO get it to return the resulting sign as well!!!
    char atoken; //We will use these tokens
    char btoken; //for the program to know whether not the variables are numbers or monomials
    std::string result1;
    std::string result2; //We will need to perform two operations if both expressions are monomials
    std::string derivative;
    set_monomials(index, concatenated);
    atoken = polynomial.monomials.a.front();
    btoken = polynomial.monomials.b.front();
    char sign;
    if(isNumber(atoken) and isNumber(btoken) and (!hasExponent(polynomial.monomials.a) and !hasExponent(polynomial.monomials.b))) //If both variables are numbers the derivative will be zero
        derivative = "null";
    else{
        result1 = mulMonomials(polynomial.monomials.da, polynomial.monomials.b);
        result2 = mulMonomials(polynomial.monomials.db, polynomial.monomials.a);
        sign = parse_signs(polynomial.monomials.a_sign, polynomial.monomials.b_sign);
        derivative = arithmetic(result1, result2, sign);
    }
    return derivative;
}


std::string derivative::quotient_rule(int &index, bool concatenated){ //TODO get the program to process h(x) raised to some power (hint: use the pow function)
    char atoken; //We will use these tokens TODO Check division by zero
    char btoken; //for the program to know whether not the varables are numbers or monomials
    char variable; //It stores x or y for the resulting monomial to be returned after operating a monomial and a number together
    char sign;
    char a_sign;
    char b_sign;
    std::string a;
    std::string da;
    std::string b;
    std::string db;
    int result1;
    int result2; //We will need to perform two operations if both expressions are monomials
    atoken = a.front();
    btoken = b.front();
    std::string derivative;
    if(!concatenated){ //Note in case fractions of fractions have to be processed there will be conflict due to them not consisting of a single value as is the case with products
        a = std::get<0>(polynomial.index_to_expression[index - 1]);
        da = std::get<1>(polynomial.index_to_expression[index - 1]);
    } else{
        ; //TODO Throw an error here
    }
    b = std::get<0>(polynomial.index_to_expression[index + 1]);
    db = std::get<1>(polynomial.index_to_expression[index + 1]);
    a_sign = a.front();
    b_sign = b.front();
    if(hasExponent(a) and hasExponent(b)) { //If both variables are raised to some power
        sign = parse_signs(a_sign, b_sign);
        if(sign == '-'){
            ;
        }
    }
    else{
        ;
    }
    return derivative;
}

void derivative::reset_polynomial() {
    polynomial.product_rule = false;
    polynomial.quotient_rule = false;
    polynomial.concatenated = false;
}

bool derivative::hasExponent(const std::string & str)
{
    const char c = '^';
    return str.find(c) != std::string::npos;
}

int derivative::perform_arithmetic(int a_integer, int b_integer, bool addition){
    int result_integer;
    if(addition){
        result_integer = _negative_operation ? a_integer - b_integer : a_integer + b_integer;
    }
    else{ //If it is a subtraction and the resulting sign after the product is negative, a will be negative and b positive
        result_integer = _negative_operation ? (-a_integer + b_integer) : a_integer - b_integer;
    }
    return result_integer;
}

std::string derivative::arithmetic(std::string &a, std::string &b, char &sign) { //TODO when a and b cannot be summed, a sutable routine checking for the length of polynomial buffer must be created in case there are concatenated product rules
    std::string a_exponent;
    int a_integer;
    std::string b_exponent;
    int b_integer;
    int result_integer;
    bool negative_operation = false; //i.e -3x - 4
    std::string result;
    if(sign == '-')
        _negative_operation = true;
    if(hasExponent(a) and hasExponent(b)){ //If both monomials are raised to some power
        auto pos = a.find('^');
        auto posb = b.find('^');
        a_exponent.assign(a.begin() + pos + 1, a.begin() + a.size());
        a_exponent.assign(b.begin() + posb + 1, b.begin() + b.size());
        if(a_exponent == b_exponent){ //They can only be added if they share their exponent
            a_integer = std::stoi(a.substr(0, pos - 2)); //-2 to make sure we get rid of the x
            b_integer = std::stoi(b.substr(0, posb - 2));
            result_integer = perform_arithmetic(a_integer, b_integer, true);
            result = std::to_string(result_integer) + 'x' + '^' + a_exponent;
        }
        else
            result = polynomial.monomials._subtract_monomials ? a + '-' + b : a + '+' + b;
    }
    else if((hasExponent(a) and !hasExponent(b)) or (!hasExponent(a) and hasExponent(b))){ //If there is a discrepancy in degrees between monomials
        return a + '+' + b; //They cannot be added and so we return the expression
    }
    else if(a.front() == 'x' and a.front() == b.front()){ //If we have something like 3x + 4x
        a_integer = std::stoi(a.substr(0, a.size() - 1));
        b_integer = std::stoi(b.substr(0, b.size() - 1));
        result_integer = perform_arithmetic(a_integer, b_integer, true);
        result = std::to_string(result_integer) + 'x';
    }
    else if(a.front() != 'x' and b.front() != 'x'){ //If they both are integers
        a_integer = std::stoi(a);
        b_integer = std::stoi(b);
        result_integer = perform_arithmetic(a_integer, b_integer, true);
        result = std::to_string(result_integer);
    }
    else
        result = polynomial.monomials._subtract_monomials ? a + '-' + b : a + '+' + b;
    return result;
}

std::string derivative::mulMonomials(std::string &a, std::string &b) { //TODO If there's a negative number we add it as a character and check in the next process if str.back() is equal to -
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
    if(hasExponent(a) and hasExponent(b)){ //3x^2 * 4x^2 will be equal to 12x^4
        pos = (int) a.find(exponent);
        posb = (int) b.find(exponent);
        a_integer = std::stoi(a.substr(0, (unsigned long) pos - 1));
        b_integer = std::stoi(b.substr(0, (unsigned long) posb - 1));
        a_exponentint = std::stoi(a.substr((unsigned long) (pos + 1), a.size()));
        b_exponentint = std::stoi(b.substr((unsigned long) (posb + 1), b.size()));
        result_exponent = a_exponentint + b_exponentint;
        result_integer = a_integer * b_integer;
        result = std::to_string(result_integer) + 'x' + '^' + std::to_string(result_exponent);
    }
    else if(hasExponent(a) and !hasExponent(b)){ //3x^2 and 4x or 4
        pos = (int) a.find(exponent);
        a_integer = std::stoi(a.substr(0, (unsigned long) pos - 1)); //We store 3 here
        a_exponent.assign(a.begin() + pos - 1, a.begin() + pos); //pos - 1 to ensure we get the variable as well it will store x^ here
        a_exponentint = std::stoi(a.substr(pos + 1, a.size())); //We store the 2 for the exponent here
        if(isNumber(b.front())) { // 3x^2 * 4
            b_integer = std::stoi(b);
            result = std::to_string(a_integer * b_integer) + a_exponent +
                     std::to_string(a_exponentint); //is 12x^2
        }
        else{ // 3x^2 * 4x
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
        if(isNumber(a.front())){ //4 * 3x^2
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
    else if(isNumber(a.front())) { //4 * 3x
            a_integer = std::stoi(a);
            b_integer = std::stoi(b.substr(0, b.size() - 1));
            result = std::to_string(a_integer * b_integer) + b.front();
    }
    else if(!isNumber(b.front())){ //3x * 4x
        a_integer = std::stoi(a.substr(0, a.size() - 1));
        b_integer = std::stoi(b.substr(0, b.size() - 1));
        result = std::to_string(a_integer * b_integer) + a.front(); //a.front() is 'x'
    }
    else { //3x * 4
        a_integer = std::stoi(a.substr(0, a.size() - 1));
        b_integer = std::stoi(b);
        result = std::to_string(a_integer * b_integer) + a.front();
    }
    return result;
}


std::string derivative::differentiate_polynomial(int &sindex, int &eindex) { //TODO get the program to correctly assign signs i.e - - is + etc
    bool previous_null = false;
    std::vector<std::tuple<std::string, std::string>> parsed_polynomial;
    std::string coefficient;
    std::string monomial;
    std::string mderivative; //Derivative of a monomial
    std::string derivative;
    char sign;
    char token;
    int previous_concatenated = 0;
    for (int i = sindex; i < eindex; i++) { //In the end we will have the indices to every symbol, a hash table mapping indices to an expresson plus its derivative
        if(isSymbol(_expressionarr[i][0])) //And a vector containing indices to all strings contained within this specific sub-expression
            sign = _expressionarr[i][0];
        if(_expressionarr[i][0] == '(')
            ;
        else
            token = _expressionarr[i].front();
        if (token == 'x' or token == 'y') { //Refactoring needed
            monomial = _expressionarr[i];
            mderivative = differentiate_monomial(monomial, sign);
            polynomial.index_to_expressionp.first = i;
            polynomial.index_to_expressionp.second = std::make_tuple(monomial, mderivative);
            polynomial.index_to_expression.insert(polynomial.index_to_expressionp);
        } else if (isNumber(token)) {
            if(hasExponent(_expressionarr[i])){ //We check
                monomial = _expressionarr[i];
                mderivative = differentiate_monomial(monomial, sign);
                polynomial.index_to_expressionp.first = i;
                polynomial.index_to_expressionp.second = std::make_tuple(monomial, mderivative);
                polynomial.index_to_expression.insert(polynomial.index_to_expressionp);
            }
            else {
                coefficient = _expressionarr[i];
                polynomial.index_to_expressionp.first = i;
                polynomial.index_to_expressionp.second = std::make_tuple(coefficient, "null");
                polynomial.index_to_expression.insert(polynomial.index_to_expressionp);
            }
        } else{
            polynomial.symbols.push_back(i);
        }
        polynomial.parsed_polynomial.push_back(i);
        polynomial.polynomial += _expressionarr[i];
    }
    for(int index : polynomial.parsed_polynomial){ //Test phase
        if(isSymbol(_expressionarr[index][0])){
            reset_polynomial();
            isRule(index);
            if(polynomial.product_rule){
                if(isConcatenated()){ //Next operation is also product rule
                    if(!previous_concatenated){ //x + 2x - 3x * 4x * 5x
                        if(product_rule(index, false) != "null"){
                            polynomial.derivative_buffer = product_rule(index, false); //Derivative buffer is empty so its value won't need to be taken into account, we set the condition to false then
                            previous_concatenated = true; //The program will know there was a previous product rule throughout the next iteration
                        } else previous_null = true;
                    }
                    else if(!previous_null)//[3x * 4x] * 5x we already found expression between brackets
                        polynomial.derivative_buffer = product_rule(index, true);
                    else ;
                }
                else if(previous_concatenated and !previous_null){ //3x * 4x * [5x * 6x] + 2x
                    if(product_rule(index, true) != "null"){
                        polynomial.derivative_buffer = product_rule(index, true);
                        derivative += polynomial.derivative_buffer;
                        polynomial.derivative_buffer.clear();
                        previous_concatenated = false;
                        previous_null = false;
                    } else ;
                }
                else{ //x - [2x * 3x] + 4x
                    if(product_rule(index, false) != "null")
                        derivative += product_rule(index, false);
                    else ;
                }
            }
            else if(polynomial.quotient_rule){ //Possible to-do Concatenated quotients won't be processed for now (Way too much of a headache)
                if(isConcatenated()){
                    polynomial.derivative_buffer = quotient_rule(index, true); //It will raise an exception
                    polynomial.derivative_buffer.clear();
                }
                else{
                    if(quotient_rule(index, false) != "null")
                        derivative += quotient_rule(index, false);
                    else ;
                }
            }
            else { //3x * 6x [+ 5x] here lies the cursor at this point
                if(std::get<1>(polynomial.index_to_expression[index + 1]) != "null") {
                    derivative += _expressionarr[index] + std::get<1>(polynomial.index_to_expression[index + 1]);
                }
                else //Derivative of a quotient is zero
                    ;
                previous_concatenated = false;
                polynomial.derivative_buffer.clear();
            }
        }
        else
            continue;
    }
    return derivative;
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

void derivative::function_parser(){
    std::vector<std::tuple<int, int>> order_depth;
    std::tuple<int, int> pivot;
    fill_function_ranges();
    find_functions_inside();
    test1();
    test2();
    test3();
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
        dgx = differentiate_polynomial(SB, EB);
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


void autocalculus(){
    char input[100];
    while(true){
        std::cout <<"                                               <---Auto Calculus--->\n\n\n";
        std::cout <<"                                         Enter function to be differentiated\n                                 " << std::endl;
        std::cin.getline(input, sizeof(input));
        derivative function = derivative(input);
        function.find_scopes();
        function.function_parser();
        break;
    }
}

int main(){

    autocalculus();
    getchar();
    return 0;
}
