#ifndef NEURAL_H_
#define NEURAL_H_

#include "doublefann.h" /* header pro .dllko */
#include "fann_cpp.h" /* C++ wrapper */

#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

#include "Typedefs.h"
#include "Support.h"
#include "Increase_tester.h"
#include "Temps.h"

/* Nabizi funkce na bazi neuronovych siti. */
class Neural_controller
{
public:
	/* dalsi parametry se berou z CONFIG */
	void use_network(FANN::neural_net & n, std::string const & path);
	/*	Natrenuje sit na datech a na bezprostredne nasledujicich datech ji otestuje.
		situations = pocet situaci, na kt. se trenuje (na zbytku se testuje)
		stopper = omezeni na pocet situaci, na kt. se testuje
		# dat v souboru data = (data na trenovani + data na testovani)
		dat na trenovani je potreba (situations + win_size + gap_size + view_size)
		dat na testovani by to chtelo take tolik
	*/
	void train_and_test_network(FANN::neural_net & n, std::string const & data, int situations, int win_size, int gap_size,
		int view_size, int stopper);
	/* Natrenuje sit na datech a na tech samych datech ji otestuje.
	   Pouziva se na bezpecne tweakovani parametru site.
	   # dat v data_filu musi byt >= situations + win_size + gap_size + view_size
	*/
	void train_and_check_network(FANN::neural_net & n, std::string const & data_file, int situations, int win_size,
		int gap_size, int view_size);
	/*	Natrenuje sit na datech a ulozi ji do souboru.
		# dat v data_filu musi byt >= situations + win_size + gap_size + view_size
	*/
	bool train_and_save_network(FANN::neural_net & n, std::string const & data_file, int situations, int win_size,
		int gap_size, int view_size);
private:
	/*	Vytvori treninkovy soubor pro neuronovnou sit. (TEMP_train.txt)
		situations = pocet okenek, kt. se bude neuronova sit ucit
		path = cesta k traninkovemu souboru s kurzovymi daty
		window_size = velikost input vrstvy site
		vrati relativni cestu k treninkovenu souboru.
	*/
	std::string make_train_file(int situations, std::string const & path,
		int window_size, int gap_size, int view_size);
	/* ann = prazdna neuronova sit
	*/
	void make_net_from_vector(FANN::neural_net & ann, int input_neurons, std::vector<int> neurons_in_layer,
		float learning_rate = 0.7);
	/* Vytvori defaultni pocty neuronu ve vrstvach pro neuronovou sit ("od oka")
	   male window_size by melo mit maly pocet layers.

	   !! zatim umi je pocty 3,4 a 5 layers.
	*/
	std::vector<int> default_layers(int win_size, int layers);
	/* 	Dostane prazdnou sit, natrenuje ji a pripravi ji na output.
		win_size = pocet vstupnich neuronu
		vraci true pokud se podarilo sit natrenovat, jinak false.

		TODO: tweakovat parametry, pouzit techniky AGGREGATION ... atd..
	*/
	bool try_to_train_and_configure_net(FANN::neural_net & ann, std::string const & train_file, int win_size, int layers);
	/*	splituje data file do dvou, pricemz prvni bude obsahovat num_data dat, a druhy zbytek.
		vrati jmena vytvorenych souboru.
	*/
	std::pair<std::string, std::string> split_data_file(std::string const & data_file, int num_data);
	/*	Otestuje neuronovou sit na datech bezprostredne nasledujicich treninkovym
		n = natrenovana sit, s aktivacni funkci pro output vrstvu nastavenou na THRESHOLD
		stopper = pocet okenek v testu */
	double test_on_data(std::string const & file, FANN::neural_net & n, int win_size, int gap_size, int view_size, int stopper);
	/*	Natrenuje sit na datech a na bezprostredne nasledujicich datech ji otestuje.
		data_test_file musi obsahovat data v case bezprostredne nasledujici po datech z data_train_filu.
		# dat v data_filu musi byt = situations + win_size.
	*/
	void train_and_test_network(FANN::neural_net & n, std::string const & data_train_file, std::string const & data_test_file, int situations,
		int win_size, int gap_size, int view_size, int stopper);


};

#endif
