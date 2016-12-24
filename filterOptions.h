#ifndef SIMPLIFY_FILTEROPTIONS_H
#define SIMPLIFY_FILTEROPTIONS_H

#include <vector>
#include <boost/filesystem/path.hpp>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

namespace Simplify {
	class DLL_PUBLIC FilterOptions {
		public:
			typedef boost::filesystem::path Path;
			typedef std::vector<Path> PathCollection;

			FilterOptions();

			PathCollection excludes;
			bool excludeMounts;
	};
}

#endif

