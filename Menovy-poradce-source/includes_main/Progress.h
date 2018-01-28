#ifndef PROGRESS_H_
#define PROGRESS_H_

#include <iostream>

using namespace std;

/* DEPRECTAED */

/*
	Pomocna trida.
	Zobrazuje progress pri provadeni nejakeho dlouheho cylku.
	Je potreba zajistit aby objekt zaniknul hned po ukonceni cyklu. (v destruktoru se mazou zbytky progress baru)
*/
class Progress
{
public:
	/*	goal = celkovy pocet cyklu. */
	Progress(int goal) : bar{ goal / 10 }, x{ 0 }, percent{ 0 }{}
	/*
		Pri zaniku objektu smaze znamky progressbaru z konzole.
		!! Ze zahadnych duvodu nefunguje u -n.
	*/
	~Progress(){ cout << '\r'; }
	/*
		Inkrementuje progress o 1 cyklus.
		Pouziva se na konci uvnitr cyklu.
	*/
	void inc()
	{
		x++;
		if (x == bar)
		{
			x = 0;
			percent += 10;
			printf("\r[%3d%%]", percent);
		}
	}

private:
	int x;
	int percent;
	int bar;
};

#endif