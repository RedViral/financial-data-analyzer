#include "Trait_3.h"

using namespace std;

void Trait_3::proc_window(const time_window & w, bool increase)
{
	if (w.size() != size) /* window musi odpovidat size zadane v konstruktoru */
	{
		cerr << "Error: Trait_3::proc_window ... wrong size" << endl;
		exit(1);
	}
	int sum = 0;
	for (int i = 0; i < w.size(); ++i)
		if (w[i] > 0.0)
			sum++;
	/* sum = 0..size */
	positive_changes[sum]++;
	if (increase)
		pos[sum]++;

	count(increase);
}

void Trait_3::print_out()
{
	if (total == 0)
	{
		cerr << "Error: Trait 3 printing out without processing any windows" << endl;
		return;
	}

	/* increase prob. | occurence probability | window size | trait | parameter | additional info. */
	for (int i = 0; i < positive_changes.size(); ++i)
	{
		int p = positive_changes[i];
		int s = pos[i];
		/* printf natvrdo pise do stdout, i kdyz je redirektovany :( */
		cout << fixed << ( p == 0 ? 0 : (double)s / p ) << " " << (double)p / total << " " << size << " " << trait_id << " " << i << endl;
		/* zadne additional info */
	}
}
