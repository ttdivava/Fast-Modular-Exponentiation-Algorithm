// Tite_Ani_S_DES.cpp : This file contains the 'main' function. Program execution begins and ends there.
/****************************************************************************************************************
*** NAME : 			Tite Divava	& Anirudh Dhoundiyal													  	  ***
*** CLASS : 		CSc 487 																			  	  ***
*** ASSIGNMENT : 	Assignement 2.A																		  	  ***
*** DUE DATE : 		10/08/2021													   						  	  ***
*** INSTRUCTOR :    Robert Fourney																		  	  ***
*****************************************************************************************************************
*** DESCRIPTION :  	This file contains the 'main' function. Program execution begins and ends there.		  ***
****************************************************************************************************************/

//libraries 
#include <iostream>

//including class header
#include "FastModExpAlgo.h".


//main
int main()
{
    string fileName = "Plaintext.txt";
    FastModExpAlgo modExp;
    modExp.testFastExpo();
	
	return 0;
}
