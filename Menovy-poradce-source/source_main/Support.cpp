#include "Support.h"

using namespace std;

void Support::init()
{
	for (int i = 1; i < support.size(); ++i)
	{
		double d;
		if (r.get_change(d))
			support[i] = d;
		else
		{
			cerr << "Error: Window size is so large that not even a single window could have been created." << endl;
			exit(1);
		}
	}
}

bool Support::shift()
{
	support.pop_front();
	double d;
	if (r.get_change(d))
	{
		support.push_back(d);
		return true;
	}
	else
		return false;
}

void Support::fill(time_window & window, int gap, time_window & view)
{
	if (window.size() + view.size() + gap > support.size())
	{
		cerr << "Error: Support::fill ... support too small for fill request." << endl;
		exit(1);
	}
	int a = window.size();
	for (int i = 0; i < a; ++i)
		window[i] = support[i];
	int b = a + gap;
	int c = b + view.size();
	for (int i = b; i < c; ++i)
		view[i - b] = support[i];
}