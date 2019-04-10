#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <iostream>
#include <string>

class Commandline
{
private:

	bool command_ok;				// Tells whether the line of command is valid
	int cpu_time;					// Allocated CPU time (optional)
	int seed;						// Random seed
	std::string instance_name;		// Instance path
	std::string output_name;		// Output path

	// Setting the dataset name
	void set_instance_name(std::string to_parse) { instance_name = to_parse; }

	// Setting the output name
	void set_output_name(std::string to_parse) { output_name = to_parse; }

	// Setting a default output name
	void set_default_output_name(std::string to_parse)
	{
		char caractere1 = '/';
		char caractere2 = '\\';

		int position = (int)to_parse.find_last_of(caractere1);
		int position2 = (int)to_parse.find_last_of(caractere2);
		if (position2 > position) position = position2;

		if (position != -1)
			output_name = to_parse.substr(0, position + 1) + "sol-" + to_parse.substr(position + 1, to_parse.length() - 1);
		else
			output_name = "sol-" + to_parse;
	}

public:

	// Constructor
	Commandline(int argc, char* argv[])
	{
		command_ok = true;
		if (argc % 2 != 0 || argc > 12 || argc < 2)
		{
			std::cout << "ISSUE WITH THE NUMBER OF COMMANDLINE PARAMETERS: " << argc << std::endl;
			command_ok = false;
		}
		else
		{
			// Setting some default values
			set_instance_name(std::string(argv[1]));
			set_default_output_name(std::string(argv[1]));
			cpu_time = 300; // Five minutes of CPU time allowed
			seed = 0;

			for (int i = 2; i < argc; i += 2)
			{
				if (std::string(argv[i]) == "-t")
					cpu_time = atoi(argv[i + 1]);
				else if (std::string(argv[i]) == "-sol")
					set_output_name(std::string(argv[i + 1]));
				else if (std::string(argv[i]) == "-seed")
					seed = atoi(argv[i + 1]);
				else
				{
					std::cout << "----- NON RECOGNIZED ARGUMENT: " << std::string(argv[i]) << std::endl;
					command_ok = false;
				}
			}
		}
	}

	// Getting path to the instance file
	std::string get_path_to_instance() { return instance_name; }

	// Getting path to the solution file
	std::string get_path_to_solution() { return output_name; }

	// Getting allocated CPU time
	int get_cpu_time() { return cpu_time; }

	// Getting the random seed
	int get_seed() { return seed; }

	// Tests whether the commandline parameters are OK
	bool is_valid() { return command_ok; }
};
#endif
