#ifndef READER_H_
#define READER_H_

#include <string>
#include <iostream>
#include <fstream>

/*	Prochazi souboru (s tiky) sekvencne. Umi otevrit a zavrit soubor a vratit zmenu v datech.
*/
class Reader
{
	/* HARD-DEF */
	/*
		input file format: (1:5 fiexd-point or less)

		INFO ABOUT DATA
		5.2342
		4.23423
		### now comments follow
		anything here
	*/
public:
	Reader() : comment_section(false), last_value(0.0) {}
	//	zakazat copy a move konstruktory
	Reader(const Reader & r) = delete;
	Reader & operator=(const Reader & r) = delete;
	Reader(Reader && r) = delete;
	Reader & operator=(const Reader && r) = delete;
	~Reader()
	{
		close();
	}
	/* vrati naslednujici zmenu v datech a nebo false, nelze-li nacist dalsi cislo*/
	bool get_change(double & out);
	/* otevre soubor pro cteni. a overi ze je citelny */
	void open(const std::string & path);
private:
	bool comment_section;
	std::ifstream input_stream;
	double last_value;

	/* precte cislo ze streamu. vraci false, pokud se nezdarilo */
	bool get_number(double & out);
	void line_error(const std::string & s)
	{
		std::cerr << "Error: bad input line: " << s << std::endl;
		std::exit(1);
	}
	/* v sekci komentaru, nebo pri spatnem streamu vrati false */
	bool readable()
	{
		return !comment_section && input_stream.good();
	}
	/* zavre soubor. dusledkem readable() bude vracet false */
	void close()
	{
		input_stream.close();
	}
};

#endif