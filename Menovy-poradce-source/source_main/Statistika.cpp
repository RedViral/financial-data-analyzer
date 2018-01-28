#include "Statistika.h"

using namespace std;

void Statistic::TOP(int win_size_min, int win_size_max, int gap_size,
	int view_size, const string & path, int how_many, double least_occurence_prob, vector<char> const & traits)
{
	vector<string> temp_files;
	for (char const c : traits)
		temp_files.push_back(trait_to_temp(win_size_min, win_size_max, gap_size, view_size, path, c));

	vector<vector<zaznam>> vektory_zaznamu;
	for (string const & temp : temp_files)
		vektory_zaznamu.push_back(top_in_file(how_many, least_occurence_prob, temp));

	vector<zaznam> smyseny_top = merge(vektory_zaznamu);

	cout << "increase prob. | occurence probability | window size | trait | parameter | additional info." << endl;

	for (int i = 0; i < smyseny_top.size(); ++i)
		cout << get<0>(smyseny_top[i]) << " " << get<1>(smyseny_top[i]) << " " << get<2>(smyseny_top[i]) << endl;
}

unique_ptr<Trait> Statistic::get_trait(char trait_id, int win_size) /* HARD-DEF */
{
	if (trait_id == 'A')
	{
		/* je mozne take nastavit pocet intervalu a jejich pokryti */
		unique_ptr<Trait> p{ new Trait_1(win_size) };
		return p;
	}
	else if (trait_id == 'B')
	{
		/* je mozne take nastavit pocet intervalu a jejich pokryti */
		unique_ptr<Trait> p{ new Trait_2(win_size) };
		return p;
	}
	else if (trait_id == 'C')
	{
		unique_ptr<Trait> p{ new Trait_3(win_size) };
		return p;
	}
	else
	{
		cerr << "Error: (get_trait) unknown trait_id" << endl;
		exit(1);
	}
}


void Statistic::TAC(int win_size_min, int win_size_max, int gap_size,
	int view_size, char trait_id, const string & path)
{
	/* dela jeden trait, ale vsechny velikosti okenek zaroven pri jednom pruchodu souborem */
	vector<unique_ptr<Trait>> trait_ptrs;
	for (int win_size = win_size_min; win_size <= win_size_max; ++win_size)
		trait_ptrs.push_back(get_trait(trait_id, win_size));

	int support_size = win_size_max + gap_size + view_size;
	Support s(support_size, path);

	vector<tuple<time_window, int, time_window>> ramce;
	for (int win_size = win_size_min; win_size <= win_size_max; ++win_size)
	{
		time_window win(win_size, 5.5);
		time_window view(view_size, 5.5);
		ramce.push_back(make_tuple(win, gap_size, view));
	}

	while (s.shift())
	{
		int i = 0;
		/* napln ramce a spocitej korespondujici statistiky */
		for (int win_size = win_size_min; win_size <= win_size_max; ++win_size)
		{
			s.fill(get<0>(ramce[i]), get<1>(ramce[i]), get<2>(ramce[i]));
			bool increase = Inc_tester::test_increase(get<2>(ramce[i])); /* get<2>(ramce[i]) == i-te view */
			trait_ptrs[i]->proc_window(get<0>(ramce[i]), increase); /* trait s odpovidajici velikosti okenka */
			i++;
		}
	}

	for (int i = 0; i < trait_ptrs.size(); ++i)
		trait_ptrs[i]->print_out();
}

string Statistic::trait_to_temp(int win_size_min, int win_size_max, int gap_size,
	int view_size, const string & path, char trait_id)
{
	streambuf * const cout_pointer = cout.rdbuf(); /* aby se dal vystup zase vratit na cout */

	int k = trait_id - 'A' + 10; /*!! mohlo by teoreticky nekdy (v nejakych kompilatorech) delat problem */ /* HARD-DEF */
	string nazev = FM.get_name(k); /* HARD-DEF */

	ofstream out{ nazev };
	cout.rdbuf(out.rdbuf()); /* presmerovat vystup do temp souboru */

	TAC(win_size_min, win_size_max, gap_size, view_size, trait_id, path);

	cout.rdbuf(cout_pointer); /* vrati vystup na cout */
	return nazev;
}

void Statistic::try_add(vector<zaznam> & top, zaznam z)
{
	/* najde nejhorsi zaznam */
	double min = 1.0; /* nejhorsi pravdepodobnost rustu */
	int j = -1; /* index nejhorsiho zaznamu */

	for (int i = 0; i < top.size(); ++i)
	{
		double perc = get<0>(top[i]);
		if (perc < min)
		{
			j = i;
			min = perc;
		}
	}
	if (j == -1) /* vsechny zaznamy uz maji 100% pravdepoobnost */
		return;
	if (get<0>(z) > min)
		top[j] = z;
}

vector<zaznam> Statistic::top_in_file(int how_many, double least_occurence_prob, string const & path)
{
	ifstream in{ path };
	vector<zaznam> top(how_many, make_tuple(0.0, 0.0, "--")); //dummy hodnoty - jen aby tam neco bylo

	while (true)
	{
		double perc, occ;
		in >> perc >> occ;
		string rest;
		getline(in, rest);
		if (in.fail())
			break;
		if (occ >= least_occurence_prob)
		{
			zaznam z = make_tuple(perc, occ, rest);
			try_add(top, z);
		}
	}
	/* top obsahuje nejlepsich HOW_MANY zaznamu pro jeden trait */
	return top;
}

vector<zaznam> Statistic::merge(vector<zaznam> & a, vector<zaznam> & b)
{
	int N = a.size(); /* == b.size() */
	auto lambda = [](zaznam & za, zaznam & zb){ return get<0>(za) > get<0>(zb); };
	sort(begin(a), end(a), lambda);
	sort(begin(b), end(b), lambda);
	vector<zaznam> c(2 * N, make_tuple(5.5, 5.5, "")); //dummy hodnoty - jen aby tam neco bylo
	std::merge(begin(a), end(a), begin(b), end(b), begin(c), lambda);
	vector<zaznam> d(N, make_tuple(5.5, 5.5, "")); //dummy hodnoty - jen aby tam neco bylo
	copy_n(begin(c), N, begin(d));
	return d;
}

vector<zaznam> Statistic::merge(vector<vector<zaznam>> & vektory_zaznamu)
{
	if (vektory_zaznamu.size() < 2)
	{
		cerr << "Error: (merge) vektoru zaznamu je moc malo" << endl;
		exit(1);
	}
	vector<zaznam> a = vektory_zaznamu[0];
	for (int i = 1; i < vektory_zaznamu.size(); ++i)
		a = merge(a, vektory_zaznamu[i]);
	return a;
}
