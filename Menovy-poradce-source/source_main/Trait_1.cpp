#include "Trait_1.h"

using namespace std;

Trait_1::Trait_1(int window_size, double width, int num_intervals) : intervals(num_intervals, 0), pos(num_intervals, 0), win_size{ window_size }
{
	if (num_intervals < 2 || width < 0.0 || num_intervals % 2 == 1)
	{
		cerr << "Error: Trait_1::constructor, wrong initialization values" << endl;
		exit(1);
	}
	/*
		eg.
		num_intervals = 6
		width = 4.0
		(-inf, -2) (-2, -1) (-1, 0) (0, 1) (1, 2) (2, inf)
		interval_w = 1.0
	*/
	interval_w = width / (num_intervals - 2); /* bocni intervaly na obou stranach pokryvaji zbytky do +-inf */
	middle_i = num_intervals / 2;
}

void Trait_1::proc_window(const time_window & w, bool increase)
{
	int max_i = middle_i; //3 (pro 6)
	double ave = average(w);
	/// eg. values
	///   0.000013
	///  -0.000110
	bool ave_neg = ave < 0.0 ? true : false;
	ave = abs(ave);

	int i = 0;
	while (ave >= 0.0 && i < max_i)
	{
		ave -= interval_w;
		i++;
	}
	///i = 1...max_i

	if (i == 0)
	{
		cerr << "Error: Trait_1::proc_window : i == 0" << endl;
		exit(1);
	}

	int index;
	if (ave_neg)
	{
		index = middle_i - i;
	}
	else
	{
		index = middle_i - 1 + i;
	}
	intervals[index]++;
	if (increase)
	{
		pos[index]++;
	}

	count(increase);
}

double Trait_1::average(const time_window & w)
{
	if (w.size() == 0)
	{
		cerr << "Error: Statistics::average, time_window of 0 size" << endl;
		exit(1);
	}
	double sum = 0.0;
	for (int i = 0; i < w.size(); ++i)
	{
		sum += w[i];
	}
	double ave = sum / w.size();
	return ave;
}

void Trait_1::print_out()
{
	/*
		eg.
		(-inf, -2) (-2, -1) (-1, 0) (0, 1) (1, 2) (2, inf)
	*/
	if (total == 0 || intervals.size() == 0)
	{
		cerr << "Error: printing Trait 1, but parameters are bad." << endl;
		exit(1);
	}

	/* increase prob. | occurence probability | window size | trait | parameter | additional info. */

	int N = intervals.size();
	for (int i = 0; i < N; ++i)
	{
		string par = get_interval(i, N);
		int occ = intervals[i];
		int inc = pos[i];
		cout << fixed << (occ == 0 ? 0 : (double)inc / occ) << " " << ((double)occ / total) << " " << win_size << " " << trait_id << " ";
		cout << par << " IW: " << defaultfloat << interval_w << " #I: " << N << endl;
	}
}

string Trait_1::get_interval(int i, int length)
{
	/*
		eg.
		N = 6
		(-inf, -2) (-2, -1) (-1, 0) (0, 1) (1, 2) (2, inf)
	*/
	double left = (i - length / 2) * interval_w;
	double right = left + interval_w;
	string R = i == length - 1 ? "inf" : to_string(right);
	string L = i == 0 ? "-inf" : to_string(left);
	return "( " + L + ", " + R + " )";
}
