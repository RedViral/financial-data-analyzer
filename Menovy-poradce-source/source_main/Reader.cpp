#include "Reader.h"

using namespace std;

bool Reader::get_number(double & out)
{
	if (!readable())
		return false;
	string s;
	getline(input_stream, s);
	if (s.size() < 1)
		line_error(s);
	if (s[0] == '#') //comment section indicator /* HARD-DEF */
	{
		comment_section = true;
		return false;
	}
	try
	{
		out = stod(s);
	}
	catch (...)
	{
		line_error(s);
	}
	return true;
}


bool Reader::get_change(double & out)
{
	double x;
	if (get_number(x))
	{
		out = x - last_value;
		last_value = x;
		return true;
	}
	else
		return false;
}

void Reader::open(const string & path)
{
	input_stream.open(path);
	comment_section = false;
	if (!readable())
	{
		cerr << "Error: couldn't open file: " << path << endl;
		exit(1);
	}
	if (!get_number(last_value))
	{
		cerr << "Error: can't read fist data value" << endl;
		exit(1);
	}
}