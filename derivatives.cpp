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
#include<queue>
#include <stack>

class derivative{


private:

    std::string _expression;
    bool _use_multimap = false;
    std::vector<std::string> _functions;
    std::vector<int> _functions_indices;
    std::pair<std::string, std::vector<int>> _function_to_rangep; //Stores function given as a string and a vector containing the chain rule iterations and the indices for the argument
    std::map<std::string, std::vector<int>> _function_to_rangem;
    std::multimap<std::string, std::vector<int>> _function_to_rangemr; //If there are repeated values
    std::pair<std::size_t, bool> _position_to_bracketsp; //Stores position of brackets and a boolean (false for '(' true for ')')
    std::map<std::size_t, bool> _position_to_bracketsm;
    std::pair<int, std::string> _index_to_functionp; //Stores each function as a key with its corresponding index in the expression
    std::map<int, std::string> _index_to_functionm;
    std::multimap<int, std::string> _index_to_functionmr; //Same for repeated values
    bool _repeated_values = false;

public:
    void detect_functions();
    void insert_function(std::string function, int index);
    std::string give_function(int index);
    std::vector<int> brackets_bag();
    void find_scope();
    derivative(std::string expression);
    derivative& operator = (const derivative&) & = default;
    derivative& operator = (derivative&&) & = default;
    ~derivative();
};

void derivative::detect_functions(){
    int log = 0, ln = 0, sin = 0, cos = 0, tan = 0, sec = 0, cosec = 0, cotan = 0, arcsin = 0, arcos = 0, arctan = 0, e = 0;
    bool done = false;
    while(!done){
        for(int i = 0; i < _expression.size(); i++){

            switch(_expression[i]){

                case 'l' :  if(_expression[i + 1] == 'n'){
                        if(_expression[i + 2] == '^'){
                            insert_function("ln^", i);
                            ln++;
                        }
                        else{
                            insert_function("ln", i);
                            ln++;
                        }
                    }
                    else if(_expression[i + 1] == 'o'){
                        if(_expression[i + 2] == '^'){
                            insert_function("log^", i);

                        }
                        else
                            insert_function("log", i);
                        log++;
                    }
                    break;

                case 's' : if(_expression[i + 1] == 'i'){
                        if(_expression[i - 3] == 'a')
                            ;
                        else if(_expression[i + 2] == '^'){
                            insert_function("sin^", i);
                            sin++;
                        }
                        else
                            insert_function("sin", i);
                        sin++;
                    }
                    else if(_expression[i - 2] == 'c'){
                        if(_expression[i + 1] == 'e'){
                            if(_expression[i + 2] == '^'){
                                insert_function("cosec^", i);
                                cosec++;
                            }
                            else
                                ;
                        }
                        else if(_expression[i - 1] == 'c'){
                            if(_expression[i + 2] == '^'){
                                insert_function("arcsin^", i);
                                arcsin++;
                            }
                            else
                                insert_function("arcsin", i);
                            arcsin++;
                        }
                        else{
                            if(_expression[i + 2] == '^'){
                                insert_function("cos^", i);
                                cos++;
                            }
                            else
                                insert_function("cos", i);
                            cos++;
                        }
                    }
                    else if(_expression[i + 1] == 'e'){
                        if(_expression[i + 3] == '^'){
                            insert_function("sec^", i);
                            sec++;
                        }
                        else
                            insert_function("sec", i);
                        sec++;
                    }
                    break;

                case 'c' : if(_expression[i + 1] == 'o'){
                        if(_expression[i - 2] == 'a')
                            ;
                        else if(_expression[i + 3] == 'e'){
                            insert_function("cosec", i);
                            cosec++;
                        }
                        else if(_expression[i + 2] == 's'){
                            insert_function("cos", i);
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

                        case 'o':   insert_function("arcos", i); arcos++;
                            break;

                        case 's':   insert_function("arcsin", i); arcsin++;
                            break;


                        case 't':   insert_function("arctan", i); arctan++;
                            break;
                    }
                    break;

                case 't':   if(_expression[i - 3] == 'a')
                        ;
                    else if(_expression[i - 2] == 'c'){
                        insert_function("cotan", i - 2);
                        cotan++;
                        break;
                    }
                    else{
                        insert_function("tan", i);
                        tan++;
                        break;
                    }
                    break;

                case 'e':   if(_expression[i - 1] == 's')
                        ;
                    else if(_expression[i - 3] == 'c')
                        ;
                    else{
                        insert_function("e", i);
                        e++;
                        break;
                    }
                    break;
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
            _use_multimap = true;
        }
    }
}


void derivative::insert_function(std::string function, int index){
    //It inserts a function along with its index in a hash table
    _functions.push_back(function);
    _functions_indices.push_back(index);
    _index_to_functionp.first = index - 2;
    _index_to_functionp.second = function;
    if(_use_multimap)
        _index_to_functionmr.insert(_index_to_functionmr);
    else
        _index_to_functionm.insert(_index_to_functionm);
}

std::string derivative::give_function(int index){
    //It will return a function given the index of its starting brackets
    //It is given by calculating the distances between the brackets and all other functions.
    //The minimum distance is used to find the index to the function that matches the brackets
    std::vector<int> distances;
    int distance;
    std::size_t at;
    if(!_repeated_values){ //If there are no repeated values we iterate through a map
        typedef  std::map<int, std::string>::const_iterator indices; //It extracts the indices to a function from the _h map (see above)
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
            distance = index - iter->first;
            if(distance > 0)
                distances.push_back(distance);
            else
                continue;
        }
    }
    auto min = std::min_element(std::begin(distances), std::end(distances));
    at = std::distance(distances.begin(), min);
    return _index_to_functionm[index - distances[static_cast<int>(at)]];
}

 std::vector<int> derivative::brackets_bag(){
    //It collects the positions of starting brackets
    std::vector<int> brackets_positions;
    std::vector<std::vector<std::size_t>> positions;
    boost::iterator_range<std::string::iterator> pos;
    std::size_t bracket_index;
    int occurrence = 0; //Starting brackets occurences
    int occurrenceb = 0; //Ending brackets occurences
    for(char& c : _expression){
        if(c  == '('){ //It stores every index of a starting bracket in sb
            pos = boost::find_nth(_expression, "(", occurrence); //Find the nth occurence
            bracket_index = distance(_expression.begin(), pos.begin());
            brackets_positions.push_back(static_cast<int>(distance(_expression.begin(), pos.begin())) - 2);
            _position_to_bracketsp.first = bracket_index - 2;
            _position_to_bracketsp.second = true;
            _position_to_bracketsm.insert(_position_to_bracketsp);
            occurrence++;
        }
        else if(c == ')'){
            pos = boost::find_nth(_expression, ")", occurrenceb);
            bracket_index = distance(_expression.begin(), pos.begin());
            brackets_positions.push_back(static_cast<int>(bracket_index) - 2);
            _position_to_bracketsp.first = bracket_index - 2;
            _position_to_bracketsp.second = false;
            _position_to_bracketsm.insert(_position_to_bracketsp);
            occurrenceb++;
        }
    }
    return brackets_positions;
}



void derivative::find_scope(){
    detect_functions();
    std::vector<int> brackets_positions = brackets_bag();
    std::stack<int> pending_brackets; //Brackets whose potential partners were already taken:(
    int single_bracket; //A bracket looking for a partner
    std::vector<int> arguments_range;
    for( unsigned i = (int) brackets_positions.size(); i-- > 0;){
        if(_position_to_bracketsm[brackets_positions[i]] == false){
            if(_position_to_bracketsm[brackets_positions[i - 1]] == false){
                pending_brackets.push(brackets_positions[i]);
            }
            else{
                arguments_range.push_back(brackets_positions[i] + 2);
                arguments_range.push_back(brackets_positions[i - 1] + 2);
                _function_to_rangep.first = give_function(brackets_positions[i]);
                _function_to_rangep.second = arguments_range;
                if(!_repeated_values)
                    _function_to_rangem.insert(_function_to_rangep);
                else
                    _function_to_rangemr.insert(_function_to_rangep);
                arguments_range.clear();
            }
        }
        else{
            if(_position_to_bracketsm[brackets_positions[i + 1]] == false){
                ;
            }
            else{
                single_bracket = pending_brackets.top();
                arguments_range.push_back(single_bracket + 2);
                arguments_range.push_back(brackets_positions[i] + 2);
                pending_brackets.pop();
                _function_to_rangep.first = give_function(brackets_positions[i]);
                _function_to_rangep.second = arguments_range;
                if(!_repeated_values)
                    _function_to_rangem.insert(_function_to_rangep);
                else
                    _function_to_rangemr.insert(_function_to_rangep);
                arguments_range.clear();
            }
        }
    }
    typedef std::multimap<std::string, std::vector<int>>::const_iterator MapIterator;
    for(MapIterator iter = _function_to_rangem.begin(); iter != _function_to_rangem.end(); iter++){
        std::cout << "Function: " << iter->first << "\nArguments:" << std::endl;
        typedef std::vector<int>::const_iterator VectorIterator;
        for(VectorIterator vect_iter = iter ->second.begin(); vect_iter != iter -> second.end(); vect_iter++){
            std::cout << " " << *vect_iter << std::endl;
        }
    }
}

derivative::derivative(std::string expression){
    _expression = expression;
}

derivative::~derivative(){

}

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
        function.find_scope();
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
