#include "Prodavac.h"
#include "Increase_tester.h"

/* includy FANN */
#include "doublefann.h" /* header pro .dllko */
#include "fann_cpp.h" /* C++ wrapper */

using namespace std;

/* skryta globalni promenna - neuronova sit */
FANN::neural_net ann;


bool CURRENCY::Predictor::predict_slope(double * input)
{
	double * output = ann.run(input);
	return Inc_tester::get_increase(output);
}

void CURRENCY::Predictor::reset_predictor(const string & saved_net)
{
	ann.destroy();
	ann.create_from_file(saved_net);
}

int CURRENCY::Predictor::get_win_size()
{
	return ann.get_num_input();
}

