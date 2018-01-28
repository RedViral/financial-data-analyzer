#include "Trait_2.h"

using namespace std;

Trait_2::Trait_2(int window_size, double width, int num_intervals) : intervals(num_intervals, 0), pos(num_intervals, 0), win_size{ window_size }
{
	if (num_intervals < 2 || width < 0.0)
	{
		cerr << "Error: Trait_2::constructor, wrong initialization values" << endl;
		exit(1);
	}
	/*
		eg.
		(0, 1) (1, 2) (2, inf)
	*/
	interval_w = width / (num_intervals - 1); /* posledni interval pokryje zbytek az do inf */
}

void Trait_2::proc_window(const time_window & w, bool increase)
{
	if (w.size() == 0)
	{
		cerr << "Error: Trait_2::proc_windows .. windows size == 0" << endl;
		exit(1);
	}
	double dev = deviation(w);

	int i = 0;
	while (dev >= 0.0 && i < intervals.size())
	{
		dev -= interval_w;
		i++;
	}
	///i = 1...intervals.size()
	i--;

	if (i < 0)
	{
		cerr << "Error: Trait_2::proc_window : i == 0" << endl;
		exit(1);
	}

	intervals[i]++;
	if (increase)
	{
		pos[i]++;
	}

	count(increase);
}

double Trait_2::deviation(const time_window & w)
{
	double ave = Trait_1::average(w);
	double dev = 0.0;
	for (int i = 0; i < w.size(); ++i)
	{
		dev += abs(w[i] - ave);
	}
	dev /= w.size();
	return dev;
}

void Trait_2::print_out()
{
	if (total == 0 || intervals.size() == 0)
	{
		cerr << "Error: Trait 2 trying to print without processing any windows or set to have 0 intervals." << endl;
		exit(1);
	}

	/* increase prob. | occurence probability | window size | trait | parameter | additional info */

	int N = intervals.size();
	for (int i = 0; i < N; ++i)
	{
		string par = get_interval(i, N);
		int occ = intervals[i];
		int inc = pos[i];
		cout << fixed << (occ == 0 ? 0 : (double)inc / occ) << " " << ((double)occ / total) << " " << win_size << " " << trait_id;
		cout << par << " IW: " << defaultfloat << interval_w << " #I: " << N << endl;
	}
}

string Trait_2::get_interval(int i, int length)
{
	/*
		eg.
		N = 3
		(0, 1) (1, 2) (2, inf)
	*/
	double left = i * interval_w;
	double right = left + interval_w;
	string L = to_string(left);
	string R = i == length - 1 ? "inf" : to_string(right);
	return "( " + L + ", " + R + " )";
}