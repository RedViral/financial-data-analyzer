#ifndef TRAIT_2_H_
#define TRAIT_2_H_

#include <vector>
#include <iostream>

#include "Typedefs.h"
#include "Trait_1.h"
#include "Configuration.h"

/*	ave(X-ave(X)) padne do nejakeho intervalu */
class Trait_2 : public Trait
{
public:
	Trait_2(int window_size) : Trait_2(window_size, CONFIG.trait_B_belt_width, CONFIG.trait_B_number_of_intervals){}
	/*
		width = sjednoceni vsech intervalu (0...max)
		num_intervals  > 1
	*/
	Trait_2(int window_size, double width, int num_intervals);
	/*	vezme okenko a pouzije ho do statistiky */
	virtual void proc_window(const time_window & w, bool increase) override;
	virtual void print_out() override;
	/*	vezme okenko a spocital ave(X-ave(X)) */
	static double deviation(const time_window & w);
private:
	std::vector<int> intervals; //	pocet okened co s prumery, kt. padnou do danych intervalu ... (-inf, ) () () ( ,inf)
	std::vector<int> pos; //	pocet okenek s pozitivnimi views
	double interval_w; //	sirka jednoho intervalu na realne ose
	char const trait_id = 'B'; /* HARD-DEF */
	int win_size;

	std::string get_interval(int i, int length);
};

#endif