#ifndef PREDICTOR_H_
#define PREDICTOR_H_

#include "doublefann.h" /* header pro .dllko */
#include "fann_cpp.h" /* C++ wrapper */

#include <string>
#include <iostream>

/****** DEPRECATED .. TATO TRIDA BUDE IMPLEMENTOVANA JAKO .DLLKO JINDE ***/
#include "Increase_tester.h"

using namespace std;

/*	Predictor je teleso, kt. predpovida rust dat.
	Tento predictor je ciste pouze jedna neuronova sit.
	TODO: Implementovat silnejsi predictor (mozna zavest dedicnost)
*/
class Predictor
{
public:
	Predictor(const string & saved_net) : win_size{ 0 }
	{
		reset_predictor(saved_net);
	}
	/*	input = pointer na prvni prvek pole doublu spravne delky (delka = get_win_size()).
		vraci true pokud predpovida increase, jinak false.
		^^ fann_type je alias pro double
	*/
	bool predict_slope(fann_type * input)
	{
		fann_type * output = ann.run(input);
		if (output == nullptr)
		{
			cerr << "Error: (Predictor::predict_slope) ouput == nullptr .. network is probably NULL" << endl;
			exit(1);
		}
		bool predicted_inc = Inc_tester::get_increase(output);
		return predicted_inc;
	}
	/* nacte predictor ze souboru, ten souboru musi byt existovat a obsahovat serializovanej predictor !! */
	void reset_predictor(const string & saved_net)
	{
		ann.destroy();
		ann.create_from_file(saved_net);
		win_size = ann.get_num_input();
	}
	/* vrati velikost okenka, kt. sit prijima */
	int get_win_size()
	{
		return win_size;
	}
private:
	int win_size;
	FANN::neural_net ann;
};

#endif
