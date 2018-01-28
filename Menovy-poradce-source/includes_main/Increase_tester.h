#ifndef INCREASE_TESTER_H_
#define INCREASE_TESTER_H_

#include <iostream>

#include "Typedefs.h"


/* 
	rozhoduje, zda view ohsabuje INCREASE nebo NE.
	to co se povazuje za increase muze byt cokoli.
	napr. ze prvni zmena ve view je kladna
	napr. ze soucet zmen ve view preroste pres nejakou Epsilon-hranici
*/
class Inc_tester
{
public:
	/* TODO: implementovat dalsi increase-testery */

	/*
		vraci true, pokud prvni zmena view je increase, jinak false.
		tuto funkci pouziva mnoho jinych.
	*/
	static bool test_increase(time_window const & view)
	{
		double x = view[0];
		return x > 0.0;
	}
	static bool test_increase_2(time_window const & view)
	{
		// 10 pipu == 0.01
		// 0.001 da nevalne vysledky
		double thres = 0.0001;

		double x = view[0];
		double y = view[7];
		return y - x > thres;
	}

	/* Zakoduje bool do dvou doublu pro trenink site.
	*/
	static std::string to_string(bool increase)
	{
		if (increase)
			return "1 -1"; /* HARD-DEF */
		else
			return "-1 1";
	}
	/* Rozkoduje doubly ze site na bool.
	   true = increase, false = else.
	*/
	static bool get_increase(double const * output)
	{
		if (output == nullptr || output + 1 == nullptr)
		{
			std::cerr << "get_slope get nullptr as argument" << std::endl;
			std::exit(1);
		}
		if ((int)*output == 1 && (int)*(output + 1) == -1) /* HARD-DEF */
			return true;
		else
			return false;
	}
};



#endif
