#include <iostream>
#include <boost/program_options.hpp>
#include "filter.h"

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
	Simplify::FilterOptions filterOptions;

	po::options_description opts("Simplify options");
	opts.add_options()
		("help,h", "Show this help message")
		("exclude,x", po::value(&filterOptions.excludes), "Add a directory to exclusion list")
		("excludemounts,X", po::bool_switch(&filterOptions.excludeMounts)->default_value(false), "Add all mount points to exclusion list")
		;

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(opts).run(), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << opts << std::endl;
		return 1;
	}

	Simplify::Filter filter;
	filter.initiailize(filterOptions);
	filter.filterStream(std::cin, std::cout);

	return 0;
}

