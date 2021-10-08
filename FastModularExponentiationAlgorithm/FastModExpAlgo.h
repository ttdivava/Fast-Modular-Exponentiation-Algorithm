#pragma once
//preprocessor directive.
// Tite_Ani_S_DES.cpp : This file contains the 'main' function. Program execution begins and ends there.
/****************************************************************************************************************
*** NAME : 			Tite Divava	& Anirudh Dhoundiyal													  	  ***
*** CLASS : 		CSc 487 																			  	  ***
*** ASSIGNMENT : 	Assignement 2A																		  	  ***
*** DUE DATE : 		10/08/2021													   						  	  ***
*** INSTRUCTOR :    Robert Fourney																		  	  ***
*****************************************************************************************************************
*** DESCRIPTION :  	This file contains the class declaration.												  ***
****************************************************************************************************************/

#include<string>
#include <iostream>

using namespace std;

class FastModExpAlgo
{
	private:

	
	public:
		FastModExpAlgo();
		~FastModExpAlgo();
		int fastModExpAlg(string, int, int);
		string decTobin(int);
		void testFastExpo();
};

