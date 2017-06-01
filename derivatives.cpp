#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <array>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <boost/algorithm/string/find.hpp>
#include <tuple>
#include <cctype>
#include <math.h>
#include <numeric>

class derivative{
    
    
private:
    
    std::string _expression;
    bool _use_multimap = false;
    int _chain_rule; //Amount of times the chain rule must be computed
    std::vector<char> _arguments;
    std::vector<std::string> _functions;
    std::vector<int> _functions_indices;
    std::vector<int> _product_indices;
    std::vector<int> _quotient_indices;
    std::vector<int> _exponential_indices;
    std::pair<std::string, std::vector<char>> _a; //Function given as a string, argument given as a vector containing characters
    std::map<std::string, std::vector<char>> _b;
    std::pair<std::string, std::vector<int>> _c; //Stores function given as a string and a vector containing the chain rule iterations and the indices for the argument
    std::map<std::string, std::vector<int>> _d;
    std::multimap<std::string, std::vector<int>> _dr; //If there are repeated values
    std::pair<std::size_t, bool> _e; //Stores position of brackets and a boolean (false for '(' true for ')')
    std::map<std::size_t, bool> _f;
    std::pair<int, std::string> _g; //Stores each function as a key with its corresponding index in the expression
    std::map<int, std::string> _h;
    std::multimap<int, std::string> _hr; //Same for repeated values
    std::pair<int, char> _i; //It assigns a symbol to an index
    std::map<int, char> _j;
    std::vector<std::string> _numbers = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    std::vector<std::string> _function_numbers; //If function is raised to some power we want that power
    std::vector<std::string> _polynomial_numbers; //It will be used to simplify algebraic expressions at the end of the program
    
    bool _repeated_values = false;
    
public:
    
    void give_functions(){
        bool done = false;
        while(!done){
            for(int i = 0; i < _expression.size(); i++){
                
                switch(_expression[i]){
                        
                    case 'l' :  if(_expression[i + 1] == 'n'){
                                    if(_expression[i + 2] == '^'){
                                        insert_function("ln^", i); //Inserts function and index in _g hash table
                                    }
                                    insert_function("ln", i);
                                }
                                else if(_expression[i + 1] == 'o'){
                                    if(_expression[i + 2] == '^'){
                                        insert_function("log^", i);
                                    }
                                    else
                                        insert_function("log", i);
                                }
                                    break;
                        
                    case 's' :  if(_expression[i + 1] == 'i'){
                                    if(_expression[i - 3] == 'a')
                                        ;
                                    else{
                                        if(_expression[i + 2] == '^'){
                                            insert_function("sin^", i); //Inserts function and index in _g hash table
                                        }
                                        else
                                            insert_function("sin", i);
                                    }
                                }
                                else if(_expression[i + 1] == 'e'){
                                    if(_expression[i + 2] == '^'){
                                        insert_function("sec^", i);
                                    }
                                    insert_function("sec", i);
                                }
                                    break;
                        
                    case 'c' : if(_expression[i + 1] == 'o'){
                                    if(_expression[i - 2] == 'a')
                                        ;
                                    else{
                                        insert_function("cos", i);
                                    }
                                }
                                else if(_expression[i + 1] == 's') //If it is arcsin we will cause a redundance
                                    ;
                                else if(_expression[i + 3] == 'e'){
                                    insert_function("cosec", i);
                                }
                                else
                                    ;
                                    break;
                        
                    case 'a':  switch(_expression[i+3]){
                            
                                    case 'o':   insert_function("arcos", i);
                                        break;
                                        
                                    case 's':   insert_function("arcsin", i);
                                        break;
                                        
                                        
                                    case 't':   insert_function("arctang", i);
                                        break;
                                }
                                break;
                        
                    case 't':   if(_expression[i - 3] == 'a')
                                    ;
                                else if(_expression[i - 2] == 'c'){
                                    insert_function("cotan", i);
                                    break;
                                }
                                else{
                                    insert_function("tan", i);
                                    break;
                                }
                                    break;
                        
                    case 'e':   if(_expression[i - 1] == 's')
                                    ;
                                else{
                                    insert_function("e", i);
                                    break;
                                }
                                    break;
                }
            }
            for(std::string func : _functions){
                for(int i = 0; i < _functions.size(); i++){
                    if(_functions[i] == func)
                        _repeated_values = true;
                }
            }
            if(!_repeated_values)
                done = true;
            else{
                _h.clear();
                _use_multimap = true;
            }
        }
    }
    
    void insert_function(std::string function, int index){
        //It inserts a function along with its index in a hash table
        _functions.push_back(function);
        _functions_indices.push_back(index);
        _g.first = index - 2;
        _g.second = function;
        if(_use_multimap)
            _hr.insert(_g);
        else
            _h.insert(_g);
    }
    
    std::string give_function(int index){
        //It will return a function given the index of its starting brackets
        //It is given by calculating the distances between the brackets and all other functions.
        //The minimum distance is used to find the index to the function that matches the brackets
        std::vector<int> distances;
        int distance;
        std::size_t at;
        if(!_repeated_values){ //If there are no repeated values we iterate through a map
            typedef  std::map<int, std::string>::const_iterator indices; //It extracts the indices to a function from the _h map (see above)
            for( indices iter = _h.begin(); iter != _h.end(); iter++){
                distance = index - iter->first;
                if(distance > 0)
                    distances.push_back(distance);
                else
                    continue;
            }
        }
        else{ //Else we iterate through a multimap
            typedef std::multimap<int, std::string>::const_iterator indicesr;
            for(indicesr iter = _hr.begin(); iter != _hr.end(); iter++){
                distance = index - iter->first;
                if(distance > 0)
                    distances.push_back(distance);
                else
                    continue;
            }
        }
        auto min = std::min_element(std::begin(distances), std::end(distances));
        at = std::distance(distances.begin(), min);
        return _h[index - distances[static_cast<int>(at)]];
    }
    
    std::vector<std::vector<std::size_t>> brackets_bag(){
        //It collects the positions of starting and ending brackets and stores them in two separate vectors, contained in a vector
        std::vector<std::size_t> sb;
        std::vector<std::size_t> eb;
        std::vector<std::vector<std::size_t>> positions;
        boost::iterator_range<std::string::iterator> pos;
        std::pair<std::size_t, bool> p;
        std::map<std::size_t, bool> m;
        int occursb = 0; //Starting brackets occurences
        int occureb = 0; //Ending brackets occurences
        for(char& c : _expression){
            if(c  == '('){ //It stores every index of a starting bracket in sb
                pos = boost::find_nth(_expression, "(", occursb); //Find the nth occurence
                sb.push_back(distance(_expression.begin(), pos.begin()));
                occursb++;
            }
            else if(c == ')'){ //It stores every index of an ending bracket in eb
                pos = boost::find_nth(_expression, ")", occureb); //Find the nth occurence
                eb.push_back(distance(_expression.begin(), pos.begin()));
                occureb++;
            }
        }
        positions.push_back(sb);
        positions.push_back(eb);
        return positions;
    }
    
    std::vector<std::size_t> mergesubvectors(std::vector<std::vector<std::size_t>> vector){
        auto merged = vector[0];
        merged.insert(std::end(merged), std::begin(vector[1]), std::end(vector[1]));
        return merged;
    }
    
    std::vector<std::size_t> arrange_brackets(){
        //Creates a hash table with the position for each bracket and a boolean specifying its type (false for '(' and true for ')')
        std::vector<std::size_t> positions;
        if(brackets_bag()[0].size() > 0){ //If there are any functions
            _chain_rule = 1;
            for(int i = 0; i < brackets_bag()[0].size(); i++){
                _e.first = brackets_bag()[0][i];
                _e.second = false;
                _f.insert(_e);
            }
            for(int i = 0; i < brackets_bag()[1].size(); i++){
                _e.first = brackets_bag()[1][i];
                _e.second = true;
                _f.insert(_e);
            }
            positions = mergesubvectors(brackets_bag());
            std::sort(positions.begin(), positions.end());
            return positions;
        }
        else if(brackets_bag()[0].size() == 1){ //If there are only two brackets
            _chain_rule = 1;
            return positions;
        }
        else //If there are no brackets we just have a polynomial
            return positions; //Positions will be empty in this case
    }
    
    void functions_with_polynomials(std::vector<std::size_t> brackets_positions, std::vector<int> arguments_positions){
        int starting_pos;
        int ending_pos;
        for(int i = 0; i < brackets_positions.size(); i ++){
            if(!_f[brackets_positions[i]] and _f[brackets_positions[i + 1]]){ //If there are two brackets facing each other
                _c.first = give_function(static_cast<int>(brackets_positions[i]));
                starting_pos = static_cast<int>(brackets_positions[i]);
                ending_pos = static_cast<int>(brackets_positions[i + 1]);
                arguments_positions.push_back(starting_pos);
                arguments_positions.push_back(ending_pos);
                arguments_positions.push_back(_chain_rule);
                _c.second = arguments_positions;
                if(_repeated_values)
                    _dr.insert(_c);
                else
                    _d.insert(_c);
                arguments_positions.clear(); //Arguments is cleared for it to be used in the next iteration
            }
        }
    }
    
    void functions_inside_functions(std::vector<std::string> functions, std::vector<std::size_t> brackets_positions, std::vector<int> arguments_positions){
        bool done = false;
        bool inside_function;
        int count = 1; //Keeps track of how many functions within functions there are
        int init;
        int starting_pos;
        int ending_pos;
        int iterations = 0;
        while(!done){
            //Next we search for functions having other functions as arguments
            //The program keeps track of consecutive non-opposite brackets deleting each pair of complementary opposite brackets as the ending brackets are found
            //It will iterate until the amount of iterations is equal to the amount of arguments there are
            //Note that this is more iterations than it would be normal for it to make but since double brackets are processed one by one and we cannot count them without previously processing them this is the only way to end the loop
            //At the end the program will have kept track of each function and the range within which its arguments lie.
            if(iterations == functions.size())
                done = true;
            inside_function = false;
            init = count;
            for(int i = 0; i < brackets_positions.size(); i++){
                if(!_f[brackets_positions[i]] and !_f[brackets_positions[i + 1]]){ //If there are two consecutive starting brackets
                    if(inside_function){ //In order for the program not to get confused, we'll ignore double brackets found inside double brackets
                        count++; //In other words, if there is a function containing another function inside, count will be increased
                    }
                    else{
                        _chain_rule++;
                        starting_pos = static_cast<int>(brackets_positions[i]);
                        arguments_positions.push_back(starting_pos);
                        _c.first = give_function(static_cast<int>(brackets_positions[i]));
                        brackets_positions.erase(brackets_positions.begin() + i); //We remove the first bracket before the next iteration
                        inside_function = true;
                    }
                }
                else if(_f[brackets_positions[i]] and _f[brackets_positions[i + 1]]){ //If there are two consecutive ending brackets
                    if(inside_function){ //In order for the program not to get confused, we'll ignore double brackets found inside double
                        if(count != init)
                            count--;    //Once the ending brackets of a function containing another function are found, count is decreased
                        else{
                            ending_pos = static_cast<int>(brackets_positions[i + 1]);
                            arguments_positions.push_back(ending_pos);
                            arguments_positions.push_back(_chain_rule);
                            brackets_positions.erase(brackets_positions.begin() + i); //We remove the outer bracket before the next iteration
                            _c.second = arguments_positions;
                            if(_repeated_values)
                                _dr.insert(_c); //If there are repated values we will use a multimap
                            else
                                _d.insert(_c); //If not we store them in a map
                            arguments_positions.clear();
                            _chain_rule--;
                        }
                    }
                }
            }
            iterations++;
        }
    }
    
    void arrange_arguments(){
        //Inserts values in a map, key: function(ln, sin...) values: a vector containing the starting and ending positions for the arguments of the function and the chain rule iterations
        give_functions(); //We detect and store the different functions
        std::vector<std::size_t> brackets_positions = arrange_brackets(); //We get the sorted positions of all brackets
        std::vector<int> arguments_positions; //This vector is stored in the map and cleared it contains the argument range and chain rule iterations
        if(brackets_positions.size() > 2){ //If there are functions within functions or multiple functions one after the other
            //First we check for functions with nothing but polynomials within them
            functions_with_polynomials(brackets_positions, arguments_positions);
            if(_d.size() == _functions.size()) //If there are no functions inside this function we can skip that processing
                for(std::string function : _functions){
                    iterate_argument(_d[function], _d[function][0],_d[function][1]);
                }
            else
                functions_inside_functions(_functions, brackets_positions, arguments_positions);
            for(std::string function : _functions){
                iterate_argument(_d[function], _d[function][0], _d[function][1]);
            }
        }
        else if(brackets_positions.size() == 2){ //If there is a single function
            functions_with_polynomials(brackets_positions, arguments_positions);
            for(std::string function : _functions){
                iterate_argument(_d[function], _d[function][0], _d[function][1]);
            }
            
        }
        else{ //If the expression is just a polynomial
            std::vector<int> symbols_bag;
            iterate_polynomial(symbols_bag);
            
        }
    }
    
    
    void iterate_argument(std::vector<int> between_brackets, int init_pos, int end_pos){
        for(int i = init_pos; i < end_pos; i++){
            symbols_bag(between_brackets, _expression[i], i);
        }
        std::sort(between_brackets.begin(), between_brackets.end());
    }
    
    void classify_symbols(int index, char symbol){
        _i.first = index; _i.second = symbol; _j.insert(_i);
    }
    
    void symbols_bag(std::vector<int> symbols_bag, char c, int index){
        //_functions will be useless if the expression is non polynomical since the symbols´ positions will be stored in the function
        switch(c){
                
            case '+' : symbols_bag.push_back(index); classify_symbols(index, '+'); break;
                
            case '-' : symbols_bag.push_back(index); classify_symbols(index, '-'); break;
                
            case '*' :  symbols_bag.push_back(index);  _product_indices.push_back(index); classify_symbols(index, '*'); break;
                
            case '/' :  symbols_bag.push_back(index); _quotient_indices.push_back(index); classify_symbols(index, '/'); break;
                
            case '^' : symbols_bag.push_back(index);  _exponential_indices.push_back(index); classify_symbols(index, '^'); break;
                
        }
    }
    
    void iterate_polynomial(std::vector<int> bag){
        for(int i = 0; i < _expression.length(); i++){
            symbols_bag(bag, _expression[i], i); //We store the positions for every symbol in our expression
        }
    }
    
    std::string differentiate_trigonometric(std::string function){
        
        if(function == "sin")
            return "cos]";
        else if(function == "cos")
            return "-sin]";
        else if(function =="tan")
            return "sec^2]"; //] will be replaced by the argument
        else if(function == "sec")
            return "sec]tan]";
        else if(function == "cot")
            return "-cosec^2]";
        else if(function == "cosec")
            return "-sin]";
        else if(function == "arcsin")
            return "1/sqrt(1 + ])";
        else if(function == "arcos")
            return "-1/sqrt( 1 + ])";
        else
            return "1/(1 + ])";
        
    }
    
    std::string differentiate_logarithmic(std::string function){
        return "1/]";
    }
    
    std::string differentiate_e(std::string function){
        return function;
    }
    
    std::string differentiate_monomial(std::string monomial){
        std::string derivative;
        derivative = monomial.assign(derivative.begin(), derivative.end() - 1); //We get rid of the x
        return derivative;
    }
    
    bool is_polynomic(int index){
        //Given a symbol index it will tell if its previous expression is polynomic
        if(_expression[index - 2] == 'x')
            return true;
        else
            return false;
    }
    
    int get_coefficient(int index){
        std::string coefficient;
        std::string::size_type sz; //Alias for size_t
        for(int i = index; i > 0; i--){
            if(isspace(_expression[i]))
                if(is_polynomic(index))
                    return std::stoi(coefficient.assign(_expression[i], _expression[index - 1]), &sz);
                else
                    return std::stoi(coefficient.assign(_expression[i], _expression[index]), &sz);
                else
                    continue;
        }
        return -1; //The compiler will jump if control reaches the end of a non-void function
    }
    
    std::string algebraic_manipulations(std::vector<int> symbols_bag){
        //It will split monomials and constants into two different groups to perform arithmetic operations on them
        std::string monomial;
        std::vector<int> coefficients; //We set the coefficients apart
        std::vector<int> coefficients_operators;
        std::vector<int> constants; //We set constants apart
        std::vector<int> constants_operators;
        int result;
        for(int symbol : symbols_bag){
            if(is_polynomic(symbol)){
                coefficients.push_back(get_coefficient(symbol));
                coefficients.push_back(get_coefficient(symbol));
                coefficients_operators.push_back(symbol);
            }
            else{
                constants.push_back(get_coefficient(symbol));
                constants.push_back(get_coefficient(symbol));
                constants_operators.push_back(symbol);
            }
        }
        result = manipulate_expression(coefficients, coefficients_operators); //We start by manipulating coefficients
        result = manipulate_expression(constants, constants_operators); //Next we operate with constants
    }
    
    void fill_operands(std::vector<int> values, std::vector<int> operations, std::vector<int> operands, int i, char sign){
        //Helper function for manipulate expression
        for(int j = 0; j < values.size(); j++){
            if(values[j] == operations[i]){
                operands.push_back(operation(values[j], values[j + 1], sign));
            }
            else
                continue;
        }
    }
    
    int manipulate_expression(std::vector<int> values, std::vector<int> operations){
        std::vector<int> mul_results; //Product results
        std::vector<int> div_results; //Division results
        std::vector<int> sum_sub_results; //Sum and subtraction results
        int result;
        std::vector<char> signs;
        char sign;
        for(int i = 0; i < operations.size(); i++){
            sign = _j[operations[i]];
            if(sign == '*'){
                fill_operands(values, operations, mul_results, i, sign);
            }
            else if(sign == '/'){
                ;
            }
            else if(sign == '+' or sign == '-'){
                fill_operands(values, operations, sum_sub_results, i, sign);
            }
        }
        auto merged = mul_results; //Now we add the results of every multiplication and addition/subtraction
        merged.insert(std::end(merged), std::begin(sum_sub_results), std::end(sum_sub_results));
        result = std::accumulate(merged.begin(), merged.end(), 0);
    }
    
    int operation(int first_number, int second_number, char operation){
        
        switch(operation){
                
            case '+' : return first_number + second_number; break;
                
            case '-' : return first_number + second_number; break;
                
            case '*' : return first_number - second_number; break;
                
            case '/' : return int(floor(first_number / second_number)); break; //Division operation must be an isolated case
                
            case '^' : return pow(first_number, second_number); break; //Exponential should really be considered as a function
                
            default : return -1; break;
        }
    }
    std::string differentiate(std::string function){
        std::string derivative;
        if(function == "Ln" or function == "Log"){
            derivative = differentiate_logarithmic(function);
        }
        else if(function == "sin" or function == "cos" or function == "tan" or function == "sec" or function == "cosec" or function == "cotan"){
            derivative = differentiate_trigonometric(function);
        }
        else if(function == "e"){
            derivative = differentiate_e(function);
        }
        else
            derivative = differentiate_monomial(function);
        return derivative;
    }
    
    
    std::string product_rule(bool polynomic, int symbol_index){
        std::string derivative_a;
        std::string derivative_b;
        std::string function_a;
        std::string function_b;
        if(polynomic){
            function_a = give_previous_function(symbol_index); //give_function will give the function lying on the right side (it searches for the closes function)
            function_b = give_function(symbol_index);
            derivative_a = differentiate(function_a);
            derivative_b = differentiate(function_b);
            return derivative_a + "*" + function_b + "+" + function_a + "+" + derivative_b;
        }
        else{
            if(!_repeated_values){
                function_b = give_function(symbol_index);
                for(int i = 0; i < _functions.size(); i++){
                    if(_functions[i] == function_b){
                        function_a = _functions[i - 1]; //It works since functions are stored in order of appearance
                    }
                }
                derivative_a = differentiate(function_a);
                derivative_b = differentiate(function_b);
                return derivative_a + "*" + function_b + "+" + function_a + "+" + derivative_b;
            }
            else{
                ;
            }
        }
    }
    std::string give_previous_function(int index){
        std::string monomial;
        for(int i = index - 1; i > 0; i--){
            if(isspace(_expression[i])){
                return monomial.assign(_expression, i + 1, index - 1); //It will return the monomial lying behind the symbol
            }
            else
                continue;
        }
    }
    //WRITE A FUNCTION THAT ITERATES THROUGH THE NUMBERS BETWEEN THE EXPONENTIAL SIGN AND THE BRACKETS
    std::string quotient_rule(std::string function){
        ;
    }
    
    derivative(std::string expression){
        _expression = expression;
    }
};

void autocalculus(){
    char input[100];
    while(true){
        std::cout <<"                                               <---Auto Calculus--->\n\n";
        std::cout <<"How to's:\n\n";
        std::cout<<"           1.When you have a function containing other functions inside your function\n";
        std::cout<<"             apart from the other functions or polynomials inside\n";
        std::cout<<"             you must make sure that the function or functions containing other functions inside your function will NEVER\n";
        std::cout<<"             be the first argument if there are no other polynomials inside or the first after a polynomial\n";
        std::cout<<"             Example: sin(tan(5x) + 3x + log(3x) - sec(5x + ln(2x)))";
        std::cout<<"             NOT sin(tan(5x) + 3x + sec(5x + ln(2x)) + log(3x))\n";
        std::cout<<"             As long as it is not the first function to appear in the argument any other combination can be valid\n";
        std::cout<<"             Example: sin(tan(5x) + 3x + log(3x) sec(5x + ln(2x)) + cos(x))\n\n";
        std::cout<<"           2. Only the char x will be taken as a variable other non-numeric characters won´t be processed.\n\n";
        std::cout<<"           3. A space must always be included between expressions and symbols.\n\n\n";
        std::cout <<"                                   Please, enter the function to be differentiated\n                                 " << std::endl;
        std::cin.getline(input, sizeof(input));
        derivative function = derivative(input);
        function.arrange_arguments();
        break;
    }
}

//typedef std::map<std::string, std::vector<int>>::const_iterator MapIterator;
//for(MapIterator iter = _d.begin(); iter != _d.end(); iter++){
//std::cout << "Function: " << iter->first << "\nArguments:" << std::endl;
//typedef std::vector<int>::const_iterator VectorIterator;
//for(VectorIterator vect_iter = iter ->second.begin(); vect_iter != iter -> second.end(); vect_iter++){
//std::cout << " " << *vect_iter << std::endl;
//}
//}

int main(){
    
    autocalculus();
    getchar();
    return 0;
}
