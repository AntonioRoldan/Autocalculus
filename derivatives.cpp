#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <array>
#include <vector>
#include <set>
#include <algorithm>
#include <boost/algorithm/string/find.hpp>

class derivative{
    
private:
    
    std::string _expression;
    int _chain_rule; //Amount of times the chain rule must be computed
    std::vector<char> _arguments;
    std::pair<std::string, std::vector<char>> _a; //Function given as a string, argument given as a vector containing characters
    std::map<std::string, std::vector<char>> _b;
    std::pair<std::string, std::vector<int>> _c; //Stores function given as a string and a vector containing the chain rule iterations and the indices for the argument
    std::map<std::string, std::vector<int>> _d;
    std::pair<std::size_t, bool> _e; //Stores position of brackets and a boolean (false for '(' true for ')')
    std::map<std::size_t, bool> _f;
    std::pair<int, std::string> _g; //Stores each function as a key with its corresponding index in the expression
    std::map<int, std::string> _h;
    
public:
    
    
    std::vector<std::string> give_functions(){
        std::vector<std::string> functions_found;
        for(int i = 0; i < _expression.size();i++){
            
            switch(_expression[i]){
                    
                case 'l' :  if(_expression[i + 1] == 'n'){
                    functions_found.push_back("ln");
                    _g.first = i;
                    _g.second = "ln";
                    _h.insert(_g);
                }
                else if(_expression[i + 1] == 'o'){
                    functions_found.push_back("log");
                    _g.first = i;
                    _g.second = "log";
                    _h.insert(_g);
                }
                    break;
                    
                case 's' : if(_expression[i + 1] == 'i'){
                    if(_expression[i - 3] == 'a')
                        ;
                    else{
                        functions_found.push_back("sin");
                        _g.first = i;
                        _g.second = "sin";
                        _h.insert(_g);
                    }
                }
                else if(_expression[i + 1] == 'e'){
                    functions_found.push_back("sec");
                    _g.first = i;
                    _g.second = "sec";
                    _h.insert(_g);
                }
                    break;
                    
                case 'c' : if(_expression[i + 1] == 'o'){
                    if(_expression[i - 2] == 'a')
                        ;
                    else{
                        functions_found.push_back("cos");
                        _g.first = i;
                        _g.second = "cos";
                        _h.insert(_g);
                    }
                }
                else if(_expression[i + 1] == 's') //If it is arcsin we will cause a redundance
                    ;
                else if(_expression[i + 3] == 'e'){
                    functions_found.push_back("cosec");
                    _g.first = i;
                    _g.second = "cosec";
                    _h.insert(_g);
                }
                else
                    ;
                    break;
                    
                case 'a': switch(_expression[i+3]){
                        
                    case 'o': functions_found.push_back("arcos");
                        _g.first = i;
                        _g.second = "arcos";
                        _h.insert(_g);
                        break;
                        
                    case 's': functions_found.push_back("arcsin");
                        _g.first = i;
                        _g.second = "arcsin";
                        _h.insert(_g);
                        break;
                        
                        
                    case 't': functions_found.push_back("arctan");
                        _g.first = i;
                        _g.second = "arctan";
                        _h.insert(_g);
                        break;
                        
                }
                    break;
                    
                case 't': if(_expression[i - 3] == 'a')
                    ;
                else if(_expression[i - 2] == 'c'){
                    functions_found.push_back("cotan");
                    _g.first = i - 2;
                    _g.second = "cotan";
                    _h.insert(_g);
                    break;
                    
                }
                else{
                    functions_found.push_back("tan");
                    _g.first = i;
                    _g.second = "tan";
                    _h.insert(_g);
                    break;
                    
                }
                    break;
                    
                case 'e': if(_expression[i - 1] == 's')
                    ;
                else{
                    functions_found.push_back("e");
                    _g.first = i;
                    _g.second = "e";
                    _h.insert(_g);
                    break;
                    
                }
                    break;
            }
            
        }
        return functions_found;
    }
    
    std::string give_function(int index){
        std::vector<int> distances;
        int distance;
        std::size_t at;
        typedef  std::map<int, std::string>::const_iterator indices;
        for( indices iter = _h.begin(); iter != _h.end(); iter++){
            distance = index - iter->first;
            if(distance > 0)
                distances.push_back(distance);
            else
                ;
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
                pos = boost::find_nth(_expression, "(", occursb);
                sb.push_back(distance(_expression.begin(), pos.begin()));
                occursb++;
            }
            else if(c == ')'){ //It stores every index of an ending bracket in eb
                pos = boost::find_nth(_expression, ")", occureb);
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
        if(brackets_bag()[0].size() > 2){ //If there are more than two brackets
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
            //typedef std::map<std::string, std::vector<int>>::const_iterator ValuesIterator;
            //for(ValuesIterator iter = m.begin(); iter != m.end(); iter++){
            //  std::cout << iter->first <<"\t" << iter->second << std::endl;
            //}
            positions = mergesubvectors(brackets_bag());
            std::sort(positions.begin(), positions.end());
        }
        else if(brackets_bag()[0].size() == 2){ //If there are only two brackets
            _chain_rule = 1;
        } //If there are no brackets we just have a polynomial
        else
            ;
        return positions;
    }
    void chain_rule(){
        //Inserts values in a map, key: function(ln, sin...) values: a vector containing the starting and ending positions for the arguments of the function and the chain rule iterations
        bool done = false;
        bool inside_function;
        int count = 1; //Keeps track of how many functions within functions there are
        int init;
        int starting_pos;
        int ending_pos;
        int iterations = 0;
        std::vector<std::string> functions = give_functions(); //We detect and store the different functions
        std::vector<std::size_t> brackets_positions = arrange_brackets();
        std::vector<int> arguments_positions;
        for(int i = 0; i <brackets_positions.size(); i ++){
            if(!_f[brackets_positions[i]] and _f[brackets_positions[i + 1]]){ //If there are two brackets facing each other
                _c.first = give_function(static_cast<int>(brackets_positions[i]));
                starting_pos = static_cast<int>(brackets_positions[i]);
                ending_pos = static_cast<int>(brackets_positions[i +1]);
                arguments_positions.push_back(starting_pos);
                arguments_positions.push_back(ending_pos);
                arguments_positions.push_back(_chain_rule);
                _c.second = arguments_positions;
                _d.insert(_c);
                arguments_positions.clear();
            }
        }
        while(!done){
            if(iterations == functions.size())
                done = true;
            inside_function = false;
            init = count;
            for(int i = 0; i < brackets_positions.size(); i++){
                if(!_f[brackets_positions[i]] and !_f[brackets_positions[i + 1]]){ //If there are two consecutive starting brackets
                    if(inside_function){
                        count++;
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
                    if(inside_function){
                        if(count != init)
                            count--;
                        else{
                            ending_pos = static_cast<int>(brackets_positions[i + 1]);
                            arguments_positions.push_back(ending_pos);
                            arguments_positions.push_back(_chain_rule);
                            brackets_positions.erase(brackets_positions.begin() + i); //We remove the outer bracket before the next iteration
                            _c.second = arguments_positions;
                            _d.insert(_c);
                            arguments_positions.clear();
                            _chain_rule--;
                        }
                    }
                }
            }
            iterations++;
        }
        typedef std::map<std::string, std::vector<int>>::const_iterator MapIterator;
        for(MapIterator iter = _d.begin(); iter != _d.end(); iter++){
            std::cout << "Function: " << iter->first << "\nArguments:" << std::endl;
            typedef std::vector<int>::const_iterator VectorIterator;
            for(VectorIterator vect_iter = iter ->second.begin(); vect_iter != iter -> second.end(); vect_iter++){
                std::cout << " " << *vect_iter << std::endl;
            }
        }
    }
    void build_hash_table(std::vector<std::string> functions, std::vector<std::size_t> positions){
        ;
    }
    
    derivative(std::string expression){
        _expression = expression;
    }
    
    
};

//typedef std::map<std::string, std::vector<char>>::const_iterator MapIterator;
//for(MapIterator iter = _f.begin(); iter != _f.end(); iter++){
//std::cout << "Function: " << iter->first << "\nArguments:" << std::endl;
//typedef std::vector<char>::const_iterator VectorIterator;
//for(VectorIterator vect_iter = iter ->second.begin(); vect_iter != iter -> second.end(); vect_iter++)
//std::cout << " " << *vect_iter << std::endl;

//_e.first = functions.front(); //As we empty the vector, functions are stored in the right order, prior to the computation of their derivatives
//_arguments.erase(_arguments.begin() + _arguments.size() - 1);
//functions.erase(functions.begin());
//_e.second = _arguments; //Arguments to those functions are also being stored
//_f.insert(_e); //We insert the pair into our hash table

int main(){
    
    derivative function = derivative("arcsin(3x + sec(3x) + tan(4x) + log(8x) + ln(5x))");
    function.chain_rule();
    return 0;
}
