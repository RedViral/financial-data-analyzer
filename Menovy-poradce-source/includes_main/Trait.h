#ifndef TRAIT_H_
#define TRAIT_H_

#include <iostream>

#include "Typedefs.h"

/* abstraktni tirda pro jevy, kt. lze hledat v datech */
class Trait
{
public:
	Trait() : total{ 0 }, total_pos{ 0 }{}
	/* 	pouzije okenko do statistiky. */
	virtual void proc_window(const time_window & w, bool increase) = 0;
	/* standardizovane vypise statistcke poznatky */
	virtual void print_out()
	{
		/* "increase prob. | occurence probability | window size | trait | parameter | additional info." */
	}
	/* zapocita zpracovane okenko */
	void count(bool increase)
	{
		total++;
		if (increase)
			total_pos++;
	}
protected:
	int total; /* pocet zpracovanych okenek */
	int total_pos; /* pocet vsech rostoucich (positivnich) views */
};

#endif