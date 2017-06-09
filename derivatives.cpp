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
    std::vector<std::string> _expressionarr;
    bool _use_multimap = false;
    std::vector<std::string> _functions;
    std::vector<int> _numbers = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'x', 'y', 'z'};
    std::vector<int> _symbols = {'+', '-', '*', '/', '(', ')'};
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
    void insert_function(std::string &function, int &index);
    bool isNumber(char &number);
    bool isSymbol(char &symbol);
    std::string give_function(int &index);
    std::vector<int> brackets_bag();
    void find_scopes();
    derivative(std::string expression); //Copy constructor
    derivative(derivative&&) = default; //Move constructor
    derivative& operator = (const derivative&) & = default; //Move constructor
    derivative& operator = (derivative&&) & = default; //Copy assignment operator
    ~derivative(); //Destructor
};

void derivative::detect_functions(){
    int log = 0, ln = 0, sin = 0, cos = 0, tan = 0, sec = 0, cosec = 0, cotan = 0, arcsin = 0, arcos = 0, arctan = 0, e = 0;
    int index;
    bool done = false;
    bool previous_number;
    std::string number;
    std::string function;
    int coefficient;
    while(!done){
        for(int i = 0; i < _expression.size(); i++){

            switch(_expression[i]){

                case 'l' :  if(_expression[i + 1] == 'n'){
                                function = "ln";
                                insert_function(function, i);
                                _expressionarr.push_back(function);
                                ln++;
                            }
                            else if(_expression[i + 1] == 'o'){
                                function = "log";
                                insert_function(function, i);
                                 _expressionarr.push_back(function);
                                log++;
                            }
                            break;

                case 's' : if(_expression[i + 1] == 'i'){
                                if(_expression[i - 3] == 'a')
                                    ;
                                else {
                                    function = "sin";
                                    insert_function(function, i);
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
                                    insert_function(function, i);
                                    _expressionarr.push_back(function);
                                    arcsin++;
                                }
                                else{
                                    function = "cos";
                                    insert_function(function, i);
                                    _expressionarr.push_back(function);
                                    cos++;
                                }
                            }
                            else if(_expression[i + 1] == 'e'){
                                function = "sec";
                                insert_function(function, i);
                                _expressionarr.push_back(function);
                                sec++;
                            }
                            break;

                case 'c' :  if(_expression[i + 1] == 'o'){
                                if(_expression[i - 2] == 'a')
                                    ;
                                else if(_expression[i + 3] == 'e'){
                                    function = "cosec";
                                    insert_function(function, i);
                                    _expressionarr.push_back(function);
                                    cosec++;
                                }
                                else if(_expression[i + 2] == 's'){
                                    function = "cos";
                                    insert_function(function, i);
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

                            case 'o':  function = "arcos"; insert_function(function, i); _expressionarr.push_back(function); arcos++;
                                break;

                            case 's':   function = "arcsin"; insert_function(function, i); _expressionarr.push_back(function); arcsin++;
                                break;


                            case 't':   function = "arctan"; insert_function(function, i); _expressionarr.push_back(function); arctan++;
                                break;
                        }
                        break;

                case 't':   if(_expression[i - 3] == 'a')
                                ;
                            else if(_expression[i - 2] == 'c'){
                                function = "cotan";
                                index = i - 2;
                                insert_function(function, index);
                                _expressionarr.push_back(function);
                                cotan++;
                                break;
                            }
                            else{
                                function = "tan";
                                insert_function(function, i);
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
                                insert_function(function, i);
                                _expressionarr.push_back(function);
                                e++;
                                break;
                            }
                            break;
                default: if(isspace(_expression[i]))
                            ;
                         else if(isNumber(_expression[i]) and !previous_number) {
                            number += _expression[i];
                            previous_number = true;
                         }
                         else if(isNumber(_expression[i]) and previous_number){
                            number += _expression[i];
                         }
                         else if(!isNumber(_expression[i]) and previous_number){
                            previous_number = false;
                            _expressionarr.push_back(number);
                            std::cout<<number<<std::endl;
                            number.clear();
                         }
                         else if(!isSymbol(_expression[i])){
                            ;
                         }
                         else {
                        std::cout << _expression[i] << std::endl;
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
        }
    }
}

bool derivative::isNumber(char &number) {
    auto pos = std::find(_numbers.begin(), _numbers.end(), number);
    if(pos != _numbers.end()){
        return true;
    }
    else
        return false;
}

bool derivative::isSymbol(char &symbol) {
    auto pos = std::find(_symbols.begin(), _symbols.end(), symbol);
    if(pos != _symbols.end()){
        return true;
    }
    else
        return false;
}

void derivative::insert_function(std::string &function, int &index){
    //It inserts a function along with its index in a hash table
    _functions.push_back(function); //We keep track of functions order
    _index_to_functionp.first = index - 2;
    _index_to_functionp.second = function;
    if(_use_multimap)
        _index_to_functionmr.insert(_index_to_functionp);
    else
        _index_to_functionm.insert(_index_to_functionp);
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
    int occurrencesb = 0; //Starting brackets occurences
    int occurrenceb = 0; //Ending brackets occurences
    for(char& c : _expression){
        if(c  == '('){
            pos = boost::find_nth(_expression, "(", occurrencesb);
            bracket_index = distance(_expression.begin(), pos.begin());
            brackets_positions.push_back(static_cast<int>(distance(_expression.begin(), pos.begin())) - 2); //We need to cast a size_t value to int
            _position_to_bracketsp.first = bracket_index - 2;
            _position_to_bracketsp.second = true; //true stands for '('
            _position_to_bracketsm.insert(_position_to_bracketsp);
            occurrencesb++;
        }
        else if(c == ')'){
            pos = boost::find_nth(_expression, ")", occurrenceb);
            bracket_index = distance(_expression.begin(), pos.begin());
            brackets_positions.push_back(static_cast<int>(bracket_index) - 2);
            _position_to_bracketsp.first = bracket_index - 2;
            _position_to_bracketsp.second = false; //false stands for ')'
            _position_to_bracketsm.insert(_position_to_bracketsp);
            occurrenceb++;
        }
    }
    return brackets_positions;
}



void derivative::find_scopes(){
    //It stores each function along with its argument range, defined by the brackets position in a map
    detect_functions();
    std::vector<int> brackets_positions = brackets_bag();
    std::stack<int> pending_brackets; //Brackets whose potential partners were already taken:(
    int single_bracket; //A bracket looking for a partner
    std::vector<int> arguments_range; //It will serve as a temporary container to store values in a hash table
    for( unsigned i = (int) brackets_positions.size(); i-- > 0;){
        if(!_position_to_bracketsm[brackets_positions[i]]){ //If brackets are ending brackets ')'
            if(!_position_to_bracketsm[brackets_positions[i - 1]]){ //If its previous brackets are also ending brackets
                pending_brackets.push(brackets_positions[i]); //We push it into a stack
            } //The logic might seem counter-intuitive at first glance, keep in mind we are iterating backwards
            else{
                arguments_range.push_back(brackets_positions[i] + 2); //We push the position for ending brackets
                arguments_range.push_back(brackets_positions[i - 1] + 2); //We push the position for starting brackets
                _function_to_rangep.first = give_function(brackets_positions[i]); //Values for ending and starting brackets are swapped when being stored
                _function_to_rangep.second = arguments_range; //That way the computer stores them in a start-to-end order
                if(!_repeated_values) //We store in a map
                    _function_to_rangem.insert(_function_to_rangep);
                else //We store in a multimap
                    _function_to_rangemr.insert(_function_to_rangep);
                arguments_range.clear(); //We clear for the next value to be stored
            }
        }
        else if(!_position_to_bracketsm[brackets_positions[i + 1]]){ //If the previous bracket to our current bracket is an ending bracket ')' i.e if ')))...)'
                ; //We ignore it, this way we make sure that when the next condition is met there will be a '(' to ')' relationship
            }
        else { //If our current bracket is a starting bracket '(' i.e if we have '()'
            single_bracket = pending_brackets.top(); //We get our previously stored value from the stack
            arguments_range.push_back(single_bracket + 2); //We push the position for ending brackets
            arguments_range.push_back(brackets_positions[i] + 2); //We push the position for starting brackets
            pending_brackets.pop(); //And free our friend from the stack
            _function_to_rangep.first = give_function(brackets_positions[i]); //Values for ending and starting brackets are swapped when being stored
            _function_to_rangep.second = arguments_range; //That way the computer stores them in a start-to-end order
            if (!_repeated_values) //We store in a map
                _function_to_rangem.insert(_function_to_rangep);
            else //We store in a multimap
                _function_to_rangemr.insert(_function_to_rangep);
            arguments_range.clear();
        }
    }
    typedef std::multimap<std::string, std::vector<int>>::const_iterator MapIterator;
    for(MapIterator iter = _function_to_rangemr.begin(); iter != _function_to_rangemr.end(); iter++){
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
        std::cout <<"                                               <---Auto Calculus--->\n\n\n";
        std::cout <<"                                   Please, enter the function to be differentiated\n                                 " << std::endl;
        std::cin.getline(input, sizeof(input));
        derivative function = derivative(input);
        function.find_scopes();
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
