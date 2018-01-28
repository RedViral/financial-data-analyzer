#include "Configuration.h"

using namespace std;

Configuration::Configuration() : /* ^^ vetsina default hodnot je zvolena experimentalne - a nemusi byt optimalni */
	trait_A_belt_width{ 0.0007 },
	trait_A_number_of_intervals{ 20 },
	trait_B_belt_width{ 0.0007 },
	trait_B_number_of_intervals{ 20 },
	min_window_size{ 1 },
	max_window_size{ 100 },
	lines_of_output{ 15 },
	required_min_occurence{ 0.05 },

	layers{ 5 },
	allowed_learning_error{ 0.000001 },
	learning_rate{ 0.7 },
	max_unsuccessful_learning_cycles{ 5 },
	max_epochs{ 500000 },
	epochs_between_printed_reports{ 100000 },
	usage_mode{ "save" },
	window_size{ 8 },
	study_material_situations{ 50 },
	test_situations{ 50 },

	delete_temporary_files{ true },
	display_consumed_time{ true },
	view_size{ 1 },
	gap_size{ 0 }
{
	/* nacist & provest integritni kontorly */
	ifstream config;
	string const configuration_file = "config.txt"; /* HARD-DEF */
	config.open(configuration_file);
	string attribute;
	while (true)
	{
		config >> attribute;
		if (config.fail())
			break;
		if (attribute[0] != '#') //neni komentar
		{
			if (attribute == "trait_A_belt_width")
			{
				read_test_and_assign<double>(config, 0.0, trait_A_belt_width);
			}
			else if (attribute == "trait_A_number_of_intervals")
			{
				read_test_and_assign<int>(config, 2, trait_A_number_of_intervals);
			}
			else if (attribute == "trait_B_belt_width")
			{
				read_test_and_assign<double>(config, 0.0, trait_B_belt_width);
			}
			else if (attribute == "trait_B_number_of_intervals")
			{
				read_test_and_assign<int>(config, 2, trait_B_number_of_intervals);
			}
			else if (attribute == "min_window_size")
			{
				read_test_and_assign<int>(config, 0, min_window_size);
			}
			else if (attribute == "max_window_size")
			{
				read_test_and_assign<int>(config, 0, max_window_size);
			}
			else if (attribute == "lines_of_output")
			{
				read_test_and_assign<int>(config, 0, lines_of_output);
			}
			else if (attribute == "required_min_occurence")
			{
				read_test_and_assign<double>(config, 0.0, required_min_occurence);
			}
			else if (attribute == "layers")
			{
				read_test_and_assign<int>(config, 1, layers);
			}
			else if (attribute == "allowed_learning_error")
			{
				read_test_and_assign<double>(config, 0.0, allowed_learning_error);
			}
			else if (attribute == "learning_rate")
			{
				read_test_and_assign<double>(config, 0.0, learning_rate);
			}
			else if (attribute == "max_unsuccessful_learning_cycles")
			{
				read_test_and_assign<int>(config, 0, max_unsuccessful_learning_cycles);
			}
			else if (attribute == "max_epochs")
			{
				read_test_and_assign<int>(config, 0, max_epochs);
			}
			else if (attribute == "epochs_between_printed_reports")
			{
				read_test_and_assign<int>(config, 0, epochs_between_printed_reports);
			}
			else if (attribute == "usage_mode")
			{
				string s;
				config >> s;
				if (s == "save" || s == "test" || s == "check")
					usage_mode = s;
			}
			else if (attribute == "window_size")
			{
				read_test_and_assign<int>(config, 0, window_size);
			}
			else if (attribute == "study_material_situations")
			{
				read_test_and_assign<int>(config, 0, study_material_situations);
			}
			else if (attribute == "test_situations")
			{
				read_test_and_assign<int>(config, 0, test_situations);
			}
			else if (attribute == "delete_temporary_files")
			{
				bool result = true;
				if (get_yes_or_no(config, result))
					delete_temporary_files = result;
			}
			else if (attribute == "display_consumed_time")
			{
				bool result = true;
				if (get_yes_or_no(config, result))
					display_consumed_time = result;
			}
			else if (attribute == "view_size")
			{
				read_test_and_assign<int>(config, 0, view_size);
			}
			else if (attribute == "gap_size")
			{
				read_test_and_assign<int>(config, -1, gap_size);
			}
			else
			{
				cerr << "Error: Configuration::constructor - undefined attribute encountered." << endl;
				exit(1);
			}
		}
		getline(config, attribute); //zahodit zbytek radku
	}
	config.close();
}

bool Configuration::get_yes_or_no(ifstream & input, bool & result)
{
	string s;
	input >> s;
	if (s == "yes")
		result = true;
	else if (s == "no")
		result = false;
	else
		return false;
	return true;
}

Configuration CONFIG; /* definice externu */