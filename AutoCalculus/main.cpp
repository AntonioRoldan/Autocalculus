
/*std::string divMonomials(std::string &a, std::string &b) {
; //TODO divMonomials function
}*/

/*class derivative{
; //TODO derivative class
};*/

void test_simplification() {
	std::string expression = "3x + 5x^53 - 3x - 5x^53";
	simplifier simplified = simplifier(expression);
	simplified.simplify_expression();
}

//void //TODO: Not sure what this was for 

void autocalculus() {
	char input[100];
	while (true) {
		std::cout << "                                               <---Auto Calculus--->\n\n\n";
		std::cout << "                                         Enter function to be differentiated\n                                 " << std::endl;
		std::cin.getline(input, sizeof(input));
		expression function = expression(input);
		function.find_scopes();
		function.function_parser();
		//function.differentiate();
		break;
	}
}

int main() {
	autocalculus();
	getchar();
	return 0;
}
