#ifndef TRAIT_1_H_
#define TRAIT_1_H_

#include <vector>
#include <iostream>
#include <string>

#include "Typedefs.h"
#include "Trait.h"
#include "Configuration.h"

/*	Prumer zmen v okenku padne do nejakeho intervalu */
class Trait_1 : public Trait
{
public:
	Trait_1(int window_size) : Trait_1(window_size, CONFIG.trait_A_belt_width, CONFIG.trait_A_number_of_intervals){}
	/*	width = sjednoceni vsech intervalu (max-min), num_intervals .... sude */
	Trait_1(int window_size, double width, int num_intervals);
	/*	vezme okenko a pouzije ho do statistiky */
	virtual void proc_window(const time_window & w, bool increase) override;
	virtual void print_out() override;
	/*	vrati prumer z okenka */
	static double average(const time_window & w);


private:
	std::vector<int> intervals; /*	pocet okenek co s prumery, kt. padnou do danych intervalu ... (-inf, ) () () ( ,inf) */
	std::vector<int> pos; /*	pocet okenek s pozitivnimi(increasujicimi) views */
	double interval_w; /*	sirka jednoho intervalu na realne ose */
	int middle_i; /* prvni pozitivni interval, tedy (0, neco) */
	int win_size;	/*	sirka okenka */
	char const trait_id = 'A'; /* HARD-DEF */

	/* pomaha pri printovani */
	std::string get_interval(int i, int length);
};





#endif