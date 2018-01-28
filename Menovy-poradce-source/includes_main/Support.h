#ifndef SUPPORT_H_
#define SUPPORT_H_

#include "Reader.h"
#include "Typedefs.h"

/*	Nosne okenko, ze ktereho se plni okenka Window a View */
class Support
{
public:
	/* 
		size > 0
		path .. cesta k souboru s kruzovymi daty
	*/
	Support(int size, std::string const & path) : support(size, 0.0)
	{
		r.open(path);
		init();
	}
	/* smazat copy a move construcotry.. jelikoz Reader je take nema */
	Support(Support const & s) = delete;
	Support & operator=(Support const & s) = delete;
	Support(Support && s) = delete;
	Support & operator=(Support && s) = delete;
	/* 
		Posune support o 1 cislo z datoveho souboru dopredu.
		Vrati false pokud dosel na konec souboru.
	*/
	bool shift();
	/*
		Pred prvnim fill() muselo byl volano shift().
		Naplni okenka Window a View pricemz mezi nimi muze byt mezera.
		Soucet window + gap + view muze byt mensi nez support, ale ne vetsi.
	*/
	void fill(time_window & window, int gap, time_window & view);
private:
	time_window support;
	Reader r; /* pri zaniku supportu se zavola destruktor readeru a zavre se soubor (stream) */
	/*
		Inicializuje support.
		Musi byt pouzito pred prvnim shiftem.
	*/
	void init();
};

#endif