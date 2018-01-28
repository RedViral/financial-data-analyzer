#ifndef TEMPS_H_
#define TEMPS_H_

#include <cstdio>
#include <vector>
#include <string>
#include <iostream>

/* Ma prehled o tom, kt. vsechny temp. soubory se timto behem vytvorili a umi je smazat */
class File_manager /* HARD-DEF */
{
public:
	File_manager()
	{
		/*	!! pokud soubor, kde se nachazi analytik.exe bude obrovskou shodou okolnosti
			obsahovat nize uvedene soubory, tak budou prepsany a pravdepodobne i smazany */
		files = {
			"", //0
			"",
			"",
			"TEMP_train.txt",		//3
			"TEMP_data_train.txt",	//4
			"TEMP_data_test.txt",	//5
			"TEMP_saved_net.txt",	//6 /* tento se nemaze */
			"",
			"",
			"",
			"TEMP_trait_A.txt",		//10
			"TEMP_trait_B.txt",		//11
			"TEMP_trait_C.txt",		//12

			/*	Prazdne stringy jsou zalozni - lze do nich pridat novy TEMP a pouzivat ho.
				Tempy 10+ jsou rezervovane pro Traity.
			*/
		};
		for (int i = 0; i < files.size(); ++i)
			used.push_back(false);
	}
	/* vrati nazev souboru a nastavi ho na used. */
	std::string get_name(int i)
	{
		if (i < 0 || i > files.size() - 1)
		{
			std::cerr << "File_manager::get_name bad index" << std::endl;
			std::exit(1);
		}
		used[i] = true;
		return files[i];
	}
	/* smaze vsechny temp soubory kt. byly pouzite v tomto behu */
	void delete_temps()
	{
		used[6] = false; /* TEMP_saved_net.txt se nemaze */
		for (int i = 0; i < files.size(); ++i)
		{
			if (used[i])
				if (remove(files[i].c_str()) != 0) /* prevede string na const char* */
					std::cout << "Error: File_manager::delete_temps couldnt delete file " + files[i] << std::endl;
		}
	}
private:
	std::vector<std::string> files;
	std::vector<bool> used;
};

extern File_manager FM;



#endif