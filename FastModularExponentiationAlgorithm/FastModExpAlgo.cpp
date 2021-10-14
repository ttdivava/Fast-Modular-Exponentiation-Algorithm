#include "FastModExpAlgo.h"
//preprocessor directive.
// Tite_Ani_S_DES.cpp : This file contains the 'main' function. Program execution begins and ends there.
/****************************************************************************************************************
*** NAME : 			Tite Divava	& Anirudh Dhoundiyal													  	  ***
*** CLASS : 		CSc 487 																			  	  ***
*** ASSIGNMENT : 	Assignement 2.A																		  	  ***
*** DUE DATE : 		10/08/2021													   						  	  ***
*** INSTRUCTOR :    Robert Fourney																		  	  ***
*****************************************************************************************************************
*** DESCRIPTION :  	This file contains t                                         he class definition.												  ***
****************************************************************************************************************/


FastModExpAlgo::FastModExpAlgo()
{
}

FastModExpAlgo::~FastModExpAlgo()
{
}

void printFastModTable(int i, char bt, int c, int f) {
    cout << i << "\t\t" << bt << "\t\t" << c << "\t\t" << f << "\t\t" << endl;
}

/****************************************************************************************
*** FUNCTION < fastModExpAlg >       											  	  ***
*****************************************************************************************
*** DESCRIPTION : This function performs the Fast Modular Exponentiation calculation  ***
***               of the modulo n of an integer a raised to the power of an integer b ***
***                                     a ^ b mod n                                   *** 
*** INPUT ARGS :  < String, int, int >             								  	  ***
*** OUTPUT ARGS : < None > 															  ***
*** IN/OUT ARGS : < None >          	    										  ***
*** RETURN : 	  < int > 															  ***
****************************************************************************************/
int FastModExpAlgo::fastModExpAlg(string binary, int a, int n) {
    int c = 0,
        f = 1;
    // Print
    cout << "i\t\t" << "b\t\t" << "c\t\t" << "f\t\t" << endl;
    for (int i = binary.size() - 1; i >= 0; i--) {
        // 
        c = 2 * c;
        f = (f * f) % n;
        // Check that the binary digit at position i is 1 to perform ...
        if (binary[i] == '1') {
            c = c + 1;
            f = (f * a) % n;
        }
        printFastModTable(i, binary[i], c, f);
    }
    return f;
}

/****************************************************************************************
*** FUNCTION < decTobin >              											  	  ***
*****************************************************************************************
*** DESCRIPTION : This function converts a decimal value to binary and return result  ***
***               as a string                                                         ***
*** INPUT ARGS :  < int >                        								  	  ***
*** OUTPUT ARGS : < None > 															  ***
*** IN/OUT ARGS : < None >          	    										  ***
*** RETURN : 	  < string >    													  ***
****************************************************************************************/
string FastModExpAlgo::decTobin(int n) {
    // hold the value of the binary string after convertion to be returned 
    string binary = "";

    // do this while n is positive, until the remainder is 0
    while (n > 0) {
        // get the remainder of n divided by 2
        binary += to_string(n % 2);
        // get the new result of n
        n = n / 2;
    }
    return binary;
}

/****************************************************************************************
*** FUNCTION < testFastExpo >              											  ***
*****************************************************************************************
*** DESCRIPTION : This function test the fast modular exponents algorithm. Prompt the ***
***               the user to enter values to apply the algo on. It prints the results **
***               of the Fast Modular Exponentiation Algorithm for a ^ b mod n        ***
*** INPUT ARGS :  < int >                        								  	  ***
*** OUTPUT ARGS : < None > 															  ***
*** IN/OUT ARGS : < None >          	    										  ***
*** RETURN : 	  < string >    													  ***
****************************************************************************************/
void FastModExpAlgo::testFastExpo() {
    // a ^ b mod n 
    int a, b, n, result;
    string binary;
    cout << "Enter a --> ";
    cin >> a;
    cout << endl << "Enter b --> ";
    cin >> b;
    cout << endl << "Enter n --> ";
    cin >> n;
    // convert b to binary then assign to binary string  
    binary = decTobin(b);
    // send binary string, a and n to calculate the fast modular of a to the power of b modular n
    // by using the binary string,  the integer a and the modular number
    // return the result 
    result = fastModExpAlg(binary, a, n);
    cout << endl << a << " ^ " << b << " mod " << n << " = " << result << endl;
}