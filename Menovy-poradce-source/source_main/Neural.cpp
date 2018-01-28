#include "Neural.h"
#include "Configuration.h"

using namespace std;

void Neural_controller::use_network(FANN::neural_net & n, string const & path)
{
	int s = CONFIG.study_material_situations;
	int w = CONFIG.window_size;
	int g = CONFIG.gap_size;
	int v = CONFIG.view_size;
	string m = CONFIG.usage_mode;

	if (m == "check")
		train_and_check_network(n, path, s, w, g, v); //natrenuje a otestuje se na stejnych datech (debug)
	else if (m == "test")
		train_and_test_network(n, path, s, w, g, v, CONFIG.test_situations); //natrenuje a otestuje se na nasledujicich datech (na pozorovani)
	else if (m == "save")
		train_and_save_network(n, path, s, w, g, v); //natrenuje a ulozi do souboru (export)
	else
	{
		cout << "Error CONFING::usage_mode has abnormal value." << endl;
		exit(1);
	}
}


string Neural_controller::make_train_file(int situations, const string & path,
	int window_size, int gap_size, int view_size)
{
	/* HARD-DEF FANN - FORMAT TRAIN SOUBORU
	num_train_data num_input num_output
	inputdata seperated by space
	outputdata seperated by space

	.
	.
	.

	inputdata seperated by space
	outputdata seperated by space */

	int n = window_size + gap_size + view_size;
	Support s(n, path);
	time_window win(window_size, 0.0);
	time_window view(view_size, 0.0);

	try
	{
		ofstream training_file;
		string const file_name = FM.get_name(3); /* HARD-DEF */
		training_file.open(file_name);

		const int output_neurons = 2; /* HARD-DEF */
		training_file << situations << " " << window_size << " " << output_neurons << endl;

		{
			/* konfigurace output streamu pro output doublu */
			training_file << fixed;
			training_file.precision(5); /* ^^ kurzovni data maji 5 signifikantnich destinnych mist */

			int cycles = 0;

			while (s.shift())
			{
				s.fill(win, gap_size, view);

				bool increase = Inc_tester::test_increase(view);

				for (int i = 0; i < win.size(); ++i)
					training_file << win[i] << " "; /* !! vypise 1 prebytecnou mezeru */
				training_file << endl;
				training_file << Inc_tester::to_string(increase) << endl;

				cycles++;
				if (cycles == situations)
					break;
			}

			if (cycles < situations)
			{
				cerr << "Error: (make_train_file) input is insufficient to make train file of " << situations << " situations" << endl;
				exit(1);
			}

		}
		return file_name;
	}
	catch (...)
	{
		cerr << "Error: make_train_file:: Problem writing to a file" << endl;
		exit(1);
	}
}

pair<string, string> Neural_controller::split_data_file(string const & data_file, int num_data)
{
	ifstream in;
	in.open(data_file);
	if (!in.good())
	{
		cerr << "split_data_file:: invalid iput path." << endl;
		exit(1);
	}

	string out_train = FM.get_name(4); /* HARD-DEF */
	string out_test = FM.get_name(5); /* HARD-DEF */

	ofstream train;
	train.open(out_train);
	ofstream test;
	test.open(out_test);

	int count = 0;
	while (true)
	{
		string line;
		getline(in, line);
		if (in.fail())
		{
			cerr << "split_data_file:: data file contains less data then num_data";
			exit(1);
		}
		train << line;
		count++;
		if (count == num_data)
			break;
		train << endl;
	}
	bool first = true;
	while (true)
	{
		string line;
		getline(in, line);
		if (in.fail())
			break;
		if (first)
			first = false;
		else
			test << endl;
		test << line;
	}
	/* soubory se samy zavrou v destruktorech */
	return make_pair(out_train, out_test);
}

double Neural_controller::test_on_data(string const & file, FANN::neural_net & n, int win_size, int gap_size, int view_size, int stopper)
{
	int supp = win_size + gap_size + view_size;
	time_window window(win_size, 0.0);
	time_window view(view_size, 0.0);
	Support s(supp, file);
	int good = 0;
	int all = 0;
	vector<fann_type> input(win_size, 0.0);
	int i = 0;

	while (s.shift())
	{
		s.fill(window, gap_size, view);
		copy(begin(window), end(window), begin(input)); /* zkopiruje, aby prevky byly sekvencne v pameti za sebou */
		fann_type * output = n.run(&input[0]);

		if (output == nullptr)
		{
			cerr << "Error: (Neural_controller::test_on_data) ouput == nullptr .. network is probably NULL" << endl;
			exit(1);
		}

		bool predicted_inc = Inc_tester::get_increase(output);
		bool actual_inc = Inc_tester::test_increase(view);
		if (predicted_inc == actual_inc)
			good++;
		all++;

		i++;
		if (i == stopper)
			break;
	}
	return (double)good / all;
}

vector<int> Neural_controller::default_layers(int win_size, int layers) /* HARD-DEF */
{
	if (win_size < 1 || layers < 2)
	{
		cerr << "Error: Neural_controller::default_layer wrong win_size or layers" << endl;
		exit(1);
	}

	int half = layers / 2;
	int num = win_size;
	int incre = win_size;
	vector<int> net_layers;
	net_layers.push_back(win_size);
	for (int i = 1; i < half; ++i)
	{
		num += max(incre, 2);
		net_layers.push_back(num);
	}
	for (int i = half; i < layers - 1; ++i)
	{
		num = max(num - max(incre, 2), 2);
		net_layers.push_back(num);
	}
	net_layers.push_back(2);
	return net_layers;
}

bool Neural_controller::try_to_train_and_configure_net(FANN::neural_net & ann, string const & train_file, int win_size, int layers)
{
	//ann.create_standard(3, 4, 8, 2); /* je schopna naucit se 10 okenek */
	//ann.create_standard(5, 4, 16, 32, 16, 2); /* je schopna naucit se 100 okenek */

	if (layers < 2)
	{
		cerr << "Error: (Neural_controller::neural) less than two layers requested." << endl;
		exit(1);
	}

	make_net_from_vector(ann, win_size, default_layers(win_size, layers), CONFIG.learning_rate);

	/* aby sit prijimala a outputovala cisla z rozmezi [-1, 1] */
	ann.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC); /* HARD-DEF */
	ann.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);

	const double allowed_error = CONFIG.allowed_learning_error;

	for (int i = 0; i < CONFIG.max_unsuccessful_learning_cycles; ++i) /* sit se n-krat zkusi natrenovat, pak to vzda */
	{
		ann.randomize_weights(-0.1, 0.1); /* na factory-reset site by melo stacit FANN::randomize_weights(-0.1, 0.1)*/
		ann.train_on_file(train_file, CONFIG.max_epochs, CONFIG.epochs_between_printed_reports, allowed_error);
		if (ann.get_MSE() <= allowed_error)
			break;
	}
	if (ann.get_MSE() > allowed_error)
		return false;

	/* aby se vystup zaokrouhloval na -1 a 1 */
	ann.set_activation_function_output(FANN::THRESHOLD_SYMMETRIC); /* HARD-DEF */
	return true;
}

void Neural_controller::make_net_from_vector(FANN::neural_net & ann, int input_neurons, vector<int> neurons_in_layer,
	float learning_rate)
{
	/* ^^ FANN::create_standard variadicka funkce nema variantu, kt. by prijimala va_list,
	tudiz volitelny pocet vrstev se musis resist takto. */
	switch (neurons_in_layer.size())
	{
		/* TODO: vylepsit */
	case 3:
		ann.create_standard(3, neurons_in_layer[0], neurons_in_layer[1], neurons_in_layer[2]);
		break;
	case 4:
		ann.create_standard(4, neurons_in_layer[0], neurons_in_layer[1], neurons_in_layer[2], neurons_in_layer[3]);
		break;
	case 5:
		ann.create_standard(5, neurons_in_layer[0], neurons_in_layer[1], neurons_in_layer[2], neurons_in_layer[3], neurons_in_layer[4]);
		break;
	default:
		cerr << "Error:: Neural_controller::make_net_from_vector neurons_in_layer has invalid length." << endl;
		exit(1);
	}

	/* ^^ vyssi learning rate snizuje presnost site
	http://en.wikipedia.org/wiki/Backpropagation
	*/
	ann.set_learning_rate(learning_rate); /* default = 0.7 */
	/* TODO: activation_steepness, learning_momentum */
}

void Neural_controller::train_and_check_network(FANN::neural_net & n, string const & data_file, int situations, int win_size,
	int gap_size, int view_size)
{
	string train_file = make_train_file(situations, data_file, win_size, gap_size, view_size);
	bool result = try_to_train_and_configure_net(n, train_file, win_size, CONFIG.layers);
	if (!result)
	{
		cout << "Network failed to train" << endl;
		return;
	}
	double truth = test_on_data(data_file, n, win_size, gap_size, view_size, situations); /* zadny stopper */
	cout << "Correctness: " << truth * 100 << "%" << endl;
}

void Neural_controller::train_and_test_network(FANN::neural_net & n, string const & data, int situations, int win_size, int gap_size,
	int view_size, int stopper)
{
	pair<string, string> data_files = split_data_file(data, situations + win_size + gap_size + view_size);
	train_and_test_network(n, data_files.first, data_files.second, situations, win_size, gap_size, view_size, stopper);
}

void Neural_controller::train_and_test_network(FANN::neural_net & n, string const & data_train_file, string const & data_test_file, int situations,
	int win_size, int gap_size, int view_size, int stopper)
{
	string train_file = make_train_file(situations, data_train_file, win_size, gap_size, view_size);
	try_to_train_and_configure_net(n, train_file, win_size, CONFIG.layers);
	double truth = test_on_data(data_test_file, n, win_size, gap_size, view_size, stopper);
	cout << "Correctness: " << truth * 100 << "%" << endl;
}

bool Neural_controller::train_and_save_network(FANN::neural_net & n, string const & data_file, int situations, int win_size,
	int gap_size, int view_size)
{
	string train_file = make_train_file(situations, data_file, win_size, gap_size, view_size);
	bool result = try_to_train_and_configure_net(n, train_file, win_size, CONFIG.layers);
	if (result)
		n.save(FM.get_name(6)); /* HARD-DEF */
	return result;
}