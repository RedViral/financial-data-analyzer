/*
	Zapoctova prace do Programovani v C++ NPRG041
	Richard Finger
	9.5.2015

	Program pouziva externi knihovnu FANN pod GNU LGPL.
	(Program tedy muze mit lib. licenci a pouzivat knihovnu zdarma.)
	http://leenissen.dk/fann/wp/

	!! = nedostatek nebo predpoklad
	^^ = pozorovani
	TODO = co by se melo dodelat (vsechna TODOcka jsou nad ramec specifikace)
	HARD-DEF = zaspecifikovane veci, kt. neni radno menit

*/

#include <ctime>

#include "Statistika.h"
#include "Neural.h"
#include "Configuration.h"

using namespace std;

/*	Ukonci program s error kodem 1 a zobrazi korektni syntaxi pouziti.
*/
void usage_error()
{
	cerr << "Usage: [-s/n] (statistika/neuronove site) [path] (cesta k datovemu souboru)" << endl;
	exit(1);
}

int main(int argc, char * * argv)
{
	int start = clock();
	
	/* parsovani */
	vector<string> args(argv+1, argv+argc); /* zahodit nazev programu */
	if (args.size() < 2)
		usage_error();

	string flag = args[0];
	string path = args[1]; /* ^^ cesta k souboru nemusi byt platna */

	if (flag == "-s") /* bude se pocitat jednoducha statistika */
	{	
		Statistic sta(CONFIG.view_size, CONFIG.gap_size);
		sta.TOP(CONFIG.min_window_size, CONFIG.max_window_size, path, CONFIG.lines_of_output, CONFIG.required_min_occurence);
	}
	else if (flag == "-n") /* budou se pouzivat neuronove site */
	{
		FANN::neural_net n;
		Neural_controller nc;
		nc.use_network(n, path);
	}
	else
	{
		usage_error();
	}

	if (CONFIG.delete_temporary_files)
		FM.delete_temps();

	if (CONFIG.display_consumed_time)
	{
		cout << '\a';
		cout << endl << "Time: " << clock() - start << "ms" << endl;
		cin.get();
	}

	return 0;
}
