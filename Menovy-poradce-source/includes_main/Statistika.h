#ifndef STATISTIKA_H_
#define STATISTIKA_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <tuple>

#include "Typedefs.h"
#include "Trait.h"
#include "Trait_1.h"
#include "Trait_2.h"
#include "Trait_3.h"
#include "Support.h"
#include "Increase_tester.h"
#include "Temps.h"

class Statistic
{
public:
	/* !! Pozor pri zmene: traits musi byt spravne nastaveny */
	Statistic(int view_size_, int gap_size_) 
		: view_size{ view_size_ }, gap_size{ gap_size_ }
	{
		traits.push_back('A'); /* HARD-DEF */
		traits.push_back('B');
		traits.push_back('C');
	}

	/*
		vypise nekolik (how_many) traitu (detailne), ktere s nejvyssi pravdepodobnosti predvidaji rust,
		pricemz jejich pravdepodobnosti vyskytu byla alespon neco (least_occurence_prob).
		Radky doplni hodnotami (0.0, 0.0, --) na pocet "how_many".
	*/
	void TOP(int win_size_min, int win_size_max, std::string const & path, int how_many, double least_occurrence_probability)
	{
		TOP(win_size_min, win_size_max, gap_size, view_size, path, how_many, least_occurrence_probability, traits);
	}

private:
	int view_size;
	int gap_size;
	std::vector<char> traits;

	/*	vypise nekolik (how_many) traitu (detailne), ktere s nejvyssi pravdepodobnosti predvidaji rust,
		pricemz jejich pravdepodobnosti vyskytu byla alespon neco (least_occurence_prob)
	*/
	void TOP(int win_size_min, int win_size_max, int gap_size,
		int view_size, std::string const & path, int how_many, double least_occurence_prob, std::vector<char> const & traits);
	/*	vrati pointer na trait odpovidajici pismenku (id) a velikosti okenka.
		!! musi se zmenit, pokud se budou menit traits.
	*/
	std::unique_ptr<Trait> get_trait(char trait_id, int win_size);
	/*	Traits Across Capacity.
		Sleduje 1 trait pres skalu velikosti okenka.
	*/
	void TAC(int win_size_min, int win_size_max, int gap_size,
		int view_size, char trait_id, std::string const & path);
	/* prozkouma trait a vypise vysledky do temporary souboru */
	std::string trait_to_temp(int win_size_min, int win_size_max, int gap_size,
		int view_size, std::string const & path, char trait_id);
	/*	prida Z do TOP na misto nejhorsiho zaznamu - pokud je Z lepsi nez nejhorsi.
		jinak nic.
	*/
	void try_add(std::vector<zaznam> & top, zaznam z);
	/* vrati nekolik (how_many) zaznamu, kt. nejlepe predpovidaji rust, a navic maji dostatecnou pravdepodobnost vyskytu. */
	std::vector<zaznam> top_in_file(int how_many, double least_occurence_prob, std::string const & path);
	/* spoji dva vektory podle nejvyssi pravdepodobnosti nasledujiciho rustu. vrati spojeni. */
	std::vector<zaznam> merge(std::vector<zaznam> & a, std::vector<zaznam> & b);
	/*	vybere jen nejlepsi zaznamy pres vsechny vektory.
		modifikuje VEKTORY_ZAZNAMU.
	*/
	std::vector<zaznam> merge(std::vector<std::vector<zaznam>> & vektory_zaznamu);
};

#endif