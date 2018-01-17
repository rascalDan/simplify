#include "filter.h"
#include <mntent.h>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/operations.hpp>

namespace Simplify {
	void
	Filter::initiailize(FilterOptions & o)
	{
		if (o.excludeMounts) {
			addMountpoints();
		}
		for(auto & e : o.excludes) {
			if (e.is_relative()) {
				exclude.insert(boost::filesystem::canonical(e, boost::filesystem::current_path()));
			}
			else {
				exclude.insert(e.remove_trailing_separator());
			}
		}
	}

	void
	Filter::addMountpoints()
	{
		auto fh = setmntent("/proc/mounts", "r");
		while (auto ent = getmntent(fh)) {
			exclude.insert(ent->mnt_dir);
		}
		endmntent(fh);
	}

	void
	Filter::filterStream(std::istream & input, std::ostream & output) const
	{
		std::string pathInput;
		while (!std::getline(input, pathInput).eof()) {
			Path path(pathInput);
			if (accept(path.remove_trailing_separator())) {
				output << path.string() << std::endl;
			}
		}
	}

	void
	Filter::find(const Path & root, std::ostream & output) const
	{
		if (exclude.find(root) == exclude.end()) {
			if (!pathPrefixesExclusion(root)) {
				output << root.string() << std::endl;
				return;
			}
			for (boost::filesystem::directory_iterator p(root); p != boost::filesystem::directory_iterator(); p++) {
				auto & ip = *p;
				if (boost::filesystem::is_directory(ip)) {
					find(ip, output);
				}
				else {
					output << ip.path().string() << std::endl;
				}
			}
		}
	}

	bool
	Filter::accept(const Path & path) const
	{
		if (!pathPrefixesExclusion(path) && !pathPrefixesAccepted(path)) {
			accepted.insert(path);
			return true;
		}
		return false;
	}

	bool
	Filter::pathPrefixesExclusion(const Path & path) const
	{
		return std::find_if(exclude.begin(), exclude.end(), [&path](const auto & entity) {
			return boost::algorithm::starts_with(entity, path);
		}) != exclude.end();
	}

	bool
	Filter::pathPrefixesAccepted(const Path & path) const
	{
		return std::find_if(accepted.begin(), accepted.end(), [&path](const auto & entity) {
			return boost::algorithm::starts_with(path, entity);
		}) != accepted.end();
	}
}

