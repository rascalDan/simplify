#include <iostream>
#include <boost/program_options.hpp>
#include "filter.h"

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
	Simplify::FilterOptions filterOptions;
	std::vector<std::string> roots;

	po::options_description opts("Simplifind options");
	opts.add_options()
		("help,h", "Show this help message")
		("exclude,x", po::value(&filterOptions.excludes), "Add a directory to exclusion list")
		("excludemounts,X", po::bool_switch(&filterOptions.excludeMounts)->default_value(false), "Add all mount points to exclusion list")
		("root,r", po::value(&roots), "Add a root to search")
		;

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(opts).run(), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << opts << std::endl;
		return 1;
	}
	for(auto & root : roots) {
		root = std::filesystem::canonical(root).string();
	}

	Simplify::Filter filter;
	filter.initiailize(filterOptions);
	for(const auto & root : roots) {
		filter.find(root, std::cout);
	}

	return 0;
}


