#ifndef PRODAVAC_H_
#define PRODAVAC_H_

#ifdef F_PRODAVAC_EXPORTS
	#define PRODAVAC_API __declspec(dllexport)
#else
	#define PRODAVAC_API __declspec(dllimport)
#endif
/*	Tento header kompiluju, kdyz kompiluju toto DLL - tehdy potrebuju DLLEXPORT
	(Visual studio pri kompilaci samo nadefinuje makro <PROJECTNAME>_EXPORTS)
	Tento header se take kompiluje, kdyz ho inculudju v nejake aplikaci, kt. pouziva toto DLL - tehdy potrebuju DLLIMPORT
*/

#include <string>

namespace CURRENCY
{

	/*	Predictor je teleso, kt. predpovida rust dat.
		Tento predictor je ciste pouze jedna neuronova sit.
		TODO: Implementovat silnejsi predictor (mozna zavest dedicnost)
	*/
	class Predictor
	{
	public:
		/*	input = pointer na prvni prvek pole doublu spravne delky (delka = get_win_size()).
			vraci true pokud predpovida increase, jinak false.
			^^ fann_type je alias pro double
		*/
		static PRODAVAC_API bool predict_slope(double * input); /* !! pozor - co je mysleno doublem ??? */
		/* Nacte predictor ze souboru, ten souboru musi byt existovat a obsahovat serializovanej predictor !! */
		static PRODAVAC_API void reset_predictor(std::string const & saved_net);
		/*	Vrati velikost okenka, kt. predictor prijima.
			Nejaka sit musela predtim byt nactena.
		*/
		static PRODAVAC_API int get_win_size();
	};
}



#endif

