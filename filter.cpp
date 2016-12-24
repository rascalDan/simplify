#include "filter.h"
#include <mntent.h>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>

namespace Simplify {
	void
	Filter::initiailize(const FilterOptions & o)
	{
		if (o.excludeMounts) {
			addMountpoints();
		}
		std::copy(o.excludes.begin(), o.excludes.end(), std::inserter(exclude, exclude.begin()));
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

