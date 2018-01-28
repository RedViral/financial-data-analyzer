#ifndef TRAIT_3_H_
#define TRAIT_3_H_

#include <vector>

#include "Trait.h"

/*	trait 3: n zmen je pozitivnich.
	0 < n < velikost okenka
*/
class Trait_3 : public Trait
{
public:
	/* window_size = velikost okenek, se kt. se bude pracovat */
	Trait_3(int window_size) : size{ window_size }, positive_changes(size + 1, 0), pos(size + 1, 0)
	{}
	virtual void proc_window(const time_window & w, bool increase) override;
	virtual void print_out() override;
private:
	int size;
	std::vector<int> positive_changes; //	0...size
	std::vector<int> pos;
	char const trait_id = 'C'; /* HARD-DEF */
};

#endif
