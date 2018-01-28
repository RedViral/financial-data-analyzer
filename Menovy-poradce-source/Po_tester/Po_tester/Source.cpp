#include <vector>
#include <string>
#include <algorithm>

#include "Prodavac.h"
#include "Support.h"
#include "Increase_tester.h"

using namespace std;
using namespace CURRENCY;

/* testuje Po_prodavac.dll (Po_prodavac.dll pouziva fanndoubled.dll) */
void ozkousej_predictor(string const & saved_net, string const & test_file, int stopper, int skip)
{
	Predictor::reset_predictor(saved_net);
	int win_size = Predictor::get_win_size();
	int gap_size = 0;
	int view_size = 1;
	int needed = win_size + gap_size + view_size;

	Support s(needed, test_file);

	time_window win(win_size, 0.0);
	time_window view(view_size, 0.0);

	cout << "PRED. ACTU." << endl;
	int sk = 0;
	while (s.shift())
	{
		sk++;
		if (sk == skip)
			break;
	}

	int i = 0;
	int good = 0;
	vector<double> input(win_size, 0.0);
	while (s.shift())
	{
		s.fill(win, gap_size, view);
		copy(begin(win), end(win), begin(input));
		bool predicted = Predictor::predict_slope(&input[0]);
		bool actual = Inc_tester::test_increase(view);
		cout << predicted << " " << actual << endl;
		i++;
		if (predicted == actual)
			good++;

		if (i == stopper)
			break;
	}
	cout << "CORR:: " << (double)good * 100 / i << "%"<< endl;


	/* TODO: odstranit (debugovaci kod)*/
	cout << "PRESS ANY KEY TO CONTINUE." << endl;
	cin.get();
}




int main()
{
	/*	testuje sit natrenovanou na n-ticich ze zacatku souboru mensi.txt oproti tomu samemu souboru
		od jeho zacatku - vcetne kusu, na kt. byla sit trenovana
	*/
	string const net = "TEMP_saved_net.txt";
	string const test = "../../text_files/mensi.txt";

	ozkousej_predictor(net, test, 200, 300);

	return 0;
}