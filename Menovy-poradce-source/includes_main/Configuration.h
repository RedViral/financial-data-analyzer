#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>
#include <fstream>
#include <iostream>

/* Nacte konfiguraci ze souboru config.txt */
class Configuration
{
public:
	/* Pokud se atribut v config.txt opakuje, bere se ten posledni */
	Configuration();
	
	/* TODO: predelat na private a pouzit gettery */
	//statistika
	double trait_A_belt_width;
	int trait_A_number_of_intervals;
	double trait_B_belt_width;
	int trait_B_number_of_intervals;
	int min_window_size; //>0
	int max_window_size;
	int lines_of_output;
	double required_min_occurence;
	//neuronove site
	int layers; //>1
	double allowed_learning_error; //MSE = mean square error(odchylka od toho na cem se sit uci)
	double learning_rate; //defaultni = 0.7
	int max_unsuccessful_learning_cycles; //tolikrat se sit pokousi naucit materialy - pak to vzda s errorem
	int max_epochs; //eg. 500000
	int epochs_between_printed_reports; //eg. 100000
	std::string usage_mode; //(save | check | test)
	int window_size;
	int study_material_situations;
	int test_situations;
	//obecne
	bool delete_temporary_files;
	bool display_consumed_time;
	int view_size; //>0
	int gap_size;

private:
	template <typename T>
	void read_test_and_assign(std::ifstream & input, T exclusive_min, T & variable)
	{
		T temp;
		input >> temp;
		if (temp > exclusive_min)
			variable = temp;
	}
	bool get_yes_or_no(std::ifstream & input, bool & result);
};

extern Configuration CONFIG;

#endif